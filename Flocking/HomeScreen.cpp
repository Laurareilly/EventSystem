#include <RakNet/WindowsIncludes.h>

#include "HomeScreen.h"
#include "UnitManager.h"
#include "Game.h"
#include "InputManager.h"
#include "ActiveGameState.h"
#include "NetworkManager.h"

HomeScreen::HomeScreen()
{
	data->headerMessage[0] = "Welcome! Choose an option:";
	data->headerMessage[1] = " 1: Create Game";
	data->headerMessage[2] = " 2: Join Game";
	data->headerMessage[3] = " 3: Quit";

	data->doesSendData = 0;
	data->doesDisplay = 1;
	data->doesUpdateInput = 1;
	data->doesUpdateState = 1;
	data->doesUpdateNetworking = 0;
}

void HomeScreen::UpdateState()
{
	if (data->doesUpdateState == 0)
	{
		data->doesUpdateState = 1;
		selectedOptionIndex = 0;
		return;
	}

	if (data->enterServer)
	{
		data->enterServer = false;
		waitFrames = 30;
		GoToNextState(gpGame->theGameState);
		return;
	}

	if (tryingToConnect)
	{
		if (successfullyConnectedToServer)
		{
			waitFrames = 30;
			std::cout << "hi" << std::endl;
			GoToNextState(gpGame->theGameState);
		}
		else
		{
			--waitFrames;
			if (waitFrames <= 0)
			{
				waitFrames = 30;
				tryingToConnect = false;
			}
		}
	}
	else
	{
		switch (selectedOptionIndex)
		{
		case 1:
			data->mpNetworkManager->setCurrentDataMethod(DataMethod::DATA_PUSH);
			data->isLocal = 0;
			tryingToConnect = true;
			wantsToBeSever = true;
			data->mpNetworkManager->initServer(data->portNumber);
			GoToNextState(gpGame->theGameState);
			break;
		case 2:
			data->isLocal = 0;
			data->doesUpdateNetworking = 1;
			tryingToConnect = true;
			data->mpNetworkManager->initClient(data->portNumber, data->ipAddress);
			break;
		case 3:
			gpGame->exitGame();
			break;
		default:
			break;
		}

		selectedOptionIndex = 0;
	}
}

void HomeScreen::UpdateInput()
{
	gpGame->getInputManager()->update();
	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::N1))
	{
		selectedOptionIndex = 1;
	}
	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::N2))
	{
		selectedOptionIndex = 2;
	}
	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::N3))
	{
		selectedOptionIndex = 3;
	}

	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::ESCAPE))
	{
		gpGame->exitGame();
	}
}

void HomeScreen::UpdateNetworking()
{
	if (!data->doesUpdateNetworking)
		return;

	data->mpNetworkManager->Update();
}

void HomeScreen::Display()
{
	gpGame->render();
}

void HomeScreen::GoToNextState(ApplicationState * passData)
{
	ApplicationState *tmpData = this;
	gpGame->theState = passData;
	next = gpGame->theState;
	next->OnArriveFromPrevious(tmpData);
}

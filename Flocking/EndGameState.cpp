#include <RakNet/WindowsIncludes.h>

#include "EndGameState.h"
#include "UnitManager.h"
#include "Game.h"
#include "InputManager.h"
#include "ActiveGameState.h"
#include "NetworkManager.h"
#include "MyEvents.h"

EndGameState::EndGameState()
{
	if(playerWinner == 0)
		data->headerMessage[0] = "Bee man wins! Would you like to: ";
	else
		data->headerMessage[0] = "Flower guy wins! Would you like to: ";

	
	data->headerMessage[1] = " 1: Play again";
	data->headerMessage[2] = " 2: Back to Lobby";
	data->headerMessage[3] = " 3: Quit Application";

	data->doesSendData = 0;
	data->doesDisplay = 1;
	data->doesUpdateInput = 1;
	data->doesUpdateState = 1;
	data->doesUpdateNetworking = 0;
}

void EndGameState::UpdateState()
{
	if (data->doesUpdateState == 0)
	{
		data->doesUpdateState = 1;
		selectedOptionIndex = 0;
		return;
	}

	//gpGame->processLoop();
	EventManager::mpInstance->ExecuteAll();

	switch (selectedOptionIndex)
	{
	case 1:
	{
		//replay game
		data->mpNetworkManager->sendReplayGame();
		ReplayGameEvent *replayGame = new ReplayGameEvent(false);
		EventManager::mpInstance->AddEvent(replayGame);
		break;
	}
	case 2:
		//go back to lobby

		break;
	case 3:
		gpGame->exitGame();
		break;
	default:
		break;
	}

	selectedOptionIndex = 0;
}

void EndGameState::UpdateInput()
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

void EndGameState::UpdateNetworking()
{
	if (!data->doesUpdateNetworking)
		return;

	data->mpNetworkManager->Update();
}

void EndGameState::Display()
{
	gpGame->render();
}

void EndGameState::GoToNextState(ApplicationState * passData)
{
	ApplicationState *tmpData = this;
	gpGame->theState = passData;
	next = gpGame->theState;
	next->OnArriveFromPrevious(tmpData);
}


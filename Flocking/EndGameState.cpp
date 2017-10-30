#include <RakNet/WindowsIncludes.h>

#include "EndGameState.h"
#include "UnitManager.h"
#include "Game.h"
#include "InputManager.h"
#include "ActiveGameState.h"
#include "NetworkManager.h"

EndGameState::EndGameState()
{
	data->headerMessage[0] = "Game Over! Would you like to: ";
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

	switch (selectedOptionIndex)
	{
	case 1:
		//replay game
		break;
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
	gpGame->theState = gpGame->theEndState;
	next = gpGame->theState;
	next->OnArriveFromPrevious(passData);
}

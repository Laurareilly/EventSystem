#include "ActiveGameState.h"
#include "UnitManager.h"
#include "Game.h"
#include "HomeScreen.h"
#include "ApplicationState.h"
#include "NetworkManager.h"
#include "InputManager.h"
#include "GameMessageManager.h"	
#include "AddUnitMessage.h"
#include "DeleteUnitMessage.h"

ActiveGameState::ActiveGameState()
{
	data->headerMessage[0] = "";
	data->headerMessage[1] = "";
	data->headerMessage[2] = "";
	data->headerMessage[3] = "";
	data->headerMessage[4] = "";
	data->headerMessage[5] = "";
	data->headerMessage[6] = "";
	data->doesSendData = 0;
	data->doesDisplay = 1;
	data->doesUpdateInput = 1;
	data->doesUpdateState = 1;
	data->doesUpdateNetworking = 1;
	addButtonPressed = deleteButtonPressed = escapePressed = false;
}

void ActiveGameState::UpdateState()
{
	gpGame->processLoop();

	if (escapePressed)
	{
		if (!data->isLocal)
		{
			data->mpNetworkManager->SendDisconnection();
		}
			//send force player to lobby packet
		ForcePlayerToLobby();
	}

	if (addButtonPressed)
	{
		GameMessage* pMessage = new AddUnitMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	
	if (deleteButtonPressed)
	{
		GameMessage* pMessage = new DeleteUnitMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	addButtonPressed = deleteButtonPressed = escapePressed = false;
}

void ActiveGameState::UpdateInput()
{
	if (!data->doesUpdateInput)
	{
		//if push and client return before setting it to 1
		data->doesUpdateInput = 1;
		return;
	}
	gpGame->getInputManager()->update();
	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::A))
	{
		//add boid
		addButtonPressed = true;
	}
	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::D))
	{
		//add boid
		deleteButtonPressed = true;
	}
	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::ESCAPE))
	{
		escapePressed = true;
	}
}

void ActiveGameState::UpdateNetworking()
{
	//send boid data
	//mpNetworkManager->SendBoidData(gpGame->getUnitManager()->getReceivedUnits());
	if (!data->doesUpdateNetworking)
	{
		return;
	}

	data->mpNetworkManager->Update();

	if (!data->mpNetworkManager->mIsServer && data->mpNetworkManager->mCurrentDataMethod == DataMethod::DATA_PUSH) return;
		
	
	data->mpNetworkManager->SendBoidData(gpGame->getUnitManager()->getLocalUnits());
	//update
}

void ActiveGameState::Display()
{
	gpGame->render();
}

void ActiveGameState::ForcePlayerToLobby()
{
	if (!data->isLocal)
	{
		//make a cleanup boids function that clears all the units (unit manager)
		// for every boid in list
		//  ->should be deleted = true
		// deleteIfShouldBeDeleted
		//cleanup networked boid array- delete all pointers
		//send packet that says we left
	}

	gpGame->getUnitManager()->cleanupBoids();

	GoToNextState(this);
}

void ActiveGameState::GoToNextState(ApplicationState * passData)
{
	gpGame->theState = gpGame->theHomeScreen;
	next = gpGame->theState;
	next->OnArriveFromPrevious(passData);
}

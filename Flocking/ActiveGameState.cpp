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
#include "Event.h"
#include "MyEvents.h"

ActiveGameState::ActiveGameState()
{
	data->headerMessage[0] = "";
	data->headerMessage[1] = "";
	data->headerMessage[2] = "";
	data->headerMessage[3] = "";

	data->doesSendData = 0;
	data->doesDisplay = 1;
	data->doesUpdateInput = 1;
	data->doesUpdateState = 1;
	data->doesUpdateNetworking = 1;

	addButtonPressed = deleteButtonPressed = escapePressed = false;
	deltaTime = LOOP_TARGET_TIME / 1000.0f;
	countDown = 1;
}

void ActiveGameState::UpdateState()
{

	std::string typeNames[3] = { "Blue", "Green", "Red" };

	if (!data->mpNetworkManager->mIsServer)
	{
		data->headerMessage[0] = "Place flower with Left Click / Change Flower with A/D: ";
		data->headerMessage[1] = "Flower Power: " + std::to_string(mFlowerPower);
		data->headerMessage[2] = "Flower Cooldown: " + std::to_string(countDown);
		data->headerMessage[3] = "Flower Type: " + typeNames[mCurrentFlowerType];
	}
	else
	{
		data->headerMessage[0] = "Score: " + std::to_string(score);
		data->headerMessage[1] = "You are the bee, go buzz buzz with Left Click";
	}

	if (data->playerIsConnected || data->isLocal) //do this if the game should be running
	{
		countDown -= deltaTime;
		if (countDown < 0)
			countDown = 0;
	}
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

	//Give Bee points every second
	if (data->mpNetworkManager->mIsServer)
	{
		if (countDown <= 0)
		{
			data->mpNetworkManager->sendChangeInScore(1); //sends an int reflective of the difference in score, this CAN be negative to subtract score
			AddToScoreEvent *addScore = new AddToScoreEvent(1);
			EventManager::mpInstance->AddEvent(addScore);

			countDown = 1;
		}
	}

	if (addButtonPressed)
	{
		int currentType = ((int)mCurrentFlowerType + 1) % NUMFLOWERS;
		mCurrentFlowerType = (FlowerType)currentType;
	}
	
	if (deleteButtonPressed)
	{
		int currentType = ((int)mCurrentFlowerType + (NUMFLOWERS - 1)) % NUMFLOWERS;
		mCurrentFlowerType = (FlowerType)currentType;
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

	if (gpGame->getInputManager()->getPressed(InputManager::KeyCode::ESCAPE))
	{
		escapePressed = true;
	}

	if (!data->playerIsConnected && !data->isLocal)
	{
		return;
	}

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
	if (gpGame->getInputManager()->getPressed(InputManager::MouseCode::LEFT))//left mouse click
	{
		Vector2D mousePos = Vector2D(gpGame->getInputManager()->getMouseX(), gpGame->getInputManager()->getMouseY());
		if (data->mpNetworkManager->mIsServer && !data->isLocal)
		{
			data->mpNetworkManager->sendBeeTarget(mousePos);
			MovePlayerEvent *movePlayer = new MovePlayerEvent(gpGame->getPlayer(), mousePos);
			EventManager::mpInstance->AddEvent(movePlayer);
		}
		else
		{
			if (countDown <= 0)
			{
				bool shouldSpawnFlower = true;
				if (mCurrentFlowerType == FlowerType::RED && mFlowerPower < abs(RED_FLOWER_COST))
				{
					shouldSpawnFlower = false;
				}

				if (shouldSpawnFlower)
				{
					data->mpNetworkManager->sendFlower(mousePos, (int)mCurrentFlowerType);
					SpawnFlowerEvent *spawnFlower = new SpawnFlowerEvent((int)mCurrentFlowerType, mousePos);
					EventManager::mpInstance->AddEvent(spawnFlower);
					countDown = 0.75f;

					if (mCurrentFlowerType == FlowerType::RED)
					{
						mFlowerPower += RED_FLOWER_COST;
					}
					else if (mCurrentFlowerType == FlowerType::BLUE)
					{
						mFlowerPower += BLUE_FLOWER_COST;
					}
					else
					{
						mFlowerPower += GREEN_FLOWER_COST;
					}
				}
			}
		}
	}

	//printf("Flower Power: %i\n", mFlowerPower);
}

void ActiveGameState::UpdateNetworking()
{
	if (!data->doesUpdateNetworking)
	{
		return;
	}

	data->mpNetworkManager->Update();

	if (!data->mpNetworkManager->mIsServer && data->mpNetworkManager->mCurrentDataMethod == DataMethod::DATA_PUSH) return;
}

void ActiveGameState::Display()
{
	gpGame->render();
}

void ActiveGameState::ForcePlayerToLobby()
{
	GoToNextState(gpGame->theHomeScreen);
}

int ActiveGameState::AddToScore(int cScore)
{
	score += cScore;

	//check for if the score is greater/equal to the max score or less than/equal to zero
	if (score <= 0)
	{
		//player die 
		data->mpNetworkManager->sendEndGame((int)PLAYER_TWO); //player two is the flower (bee died)
		EndGameEvent *playerDie = new EndGameEvent((int)PLAYER_TWO); 
		EventManager::mpInstance->AddEvent(playerDie);
	}
	else if (score >= MAX_SCORE)
	{
		//player win
		data->mpNetworkManager->sendEndGame((int)PLAYER_ONE); //player 1 is the bee (flower man LOSES)
		EndGameEvent *playerDie = new EndGameEvent(PLAYER_ONE); 
		EventManager::mpInstance->AddEvent(playerDie);
	}

	return score;
}


void ActiveGameState::GoToNextState(ApplicationState * passData)
{
	data->playerIsConnected = false;
	ApplicationState *tmpData = this;
	gpGame->theState = passData;
	next = gpGame->theState;
	next->OnArriveFromPrevious(tmpData);
}

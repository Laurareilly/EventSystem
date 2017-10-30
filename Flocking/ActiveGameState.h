#pragma once

#include "ApplicationState.h"

class ActiveGameState : public ApplicationState
{
public:
	ActiveGameState();
	~ActiveGameState() {};

	virtual void UpdateState();
	virtual void UpdateInput();
	virtual void UpdateNetworking();
	virtual void Display();

	virtual void ForcePlayerToLobby();
	virtual int getIsLocal() { return data->isLocal; }
	virtual void SetClientID(int cID) {};

	virtual void AcceptedToServer() { data->playerIsConnected = true; };
	//virtual SystemAddress GetPeerAddress() { return data->peerSystemAddress; }

	virtual std::string* GetHeaderMessage() { return data->headerMessage; };

	bool isServer;

	virtual int AddToScore(int cScore);
	virtual void setWinner(int playerWinner) {};
	//virtual void ResetGameVals();

	virtual void OnArriveFromPrevious(ApplicationState *passData)
	{
		data->headerMessage[0] = "";
		data->headerMessage[1] = "";
		data->headerMessage[2] = "";
		data->headerMessage[3] = "";


		data->clientID = 0;
		data->ipAddress = passData->data->ipAddress;
		data->isLocal = passData->data->isLocal;
		data->portNumber = passData->data->portNumber;
		data->doesDisplay = 1;
		data->doesUpdateInput = 0;
		data->doesUpdateState = 1;
		data->doesUpdateNetworking = !data->isLocal;
		data->doesSendData = 0;
		data->mpNetworkManager = passData->data->mpNetworkManager;
		data->playerIsConnected = passData->data->playerIsConnected;


		countDown = 1;
		score = 40;
		mFlowerPower = 20;
		mCurrentFlowerType = FlowerType::BLUE;
	}

	virtual void GoToNextState(ApplicationState *passData);

	enum FlowerType
	{
		BLUE,
		GREEN,
		RED,
		NUMFLOWERS
	};

	enum PlayerNum
	{
		PLAYER_ONE,
		PLAYER_TWO
	};

private:

	bool wantsToBeSever = false;
	bool successfullyConnectedToServer = false;
	int waitFrames = 30;
	bool escapePressed;
	bool addButtonPressed;
	bool deleteButtonPressed;
	FlowerType mCurrentFlowerType = FlowerType::BLUE;

	float mFlowerPower = 20;
	int score = 90;
	float countDown;
	float deltaTime;

	const int MAX_SCORE = 100;
	const int MAX_FLOWER_POWER = 100;
	const int BLUE_FLOWER_COST = 5;
	const int GREEN_FLOWER_COST = 15;
	const int RED_FLOWER_COST = -20; //As flower player
	const int RED_FLOWER_DAMAGE = -30; //As bee player
};
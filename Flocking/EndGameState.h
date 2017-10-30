#pragma once
#include "ApplicationState.h"

class EndGameState : public ApplicationState
{
public:
	EndGameState();
	~EndGameState() {};

	virtual void UpdateState();
	virtual void UpdateInput();
	virtual void UpdateNetworking();
	virtual void Display();

	virtual void ForcePlayerToLobby() {};
	virtual int getIsLocal() { return data->isLocal; }
	virtual void SetClientID(int cID) {};
	virtual void AcceptedToServer()
	{
		data->enterServer = 1;
		data->playerIsConnected = true;
	};

	bool isServer;

	virtual int AddToScore(int cScore) { return 0; }; //doesn't have a score

	std::string* GetHeaderMessage() { return data->headerMessage; }

	virtual void OnArriveFromPrevious(ApplicationState *passData)
	{
		data->headerMessage[0] = "Game Over! Would you like to: ";
		data->headerMessage[1] = " 1: Play again";
		data->headerMessage[2] = " 2: Back to Lobby";
		data->headerMessage[3] = " 3: Quit Application";

		data->clientID = 0;
		data->ipAddress = passData->data->ipAddress;
		data->isLocal = passData->data->isLocal;
		data->portNumber = passData->data->portNumber;
		data->doesDisplay = 1;
		data->doesUpdateInput = 1;
		data->doesUpdateState = 1;
		data->doesUpdateNetworking = 0;
		data->doesSendData = 0;
		data->mpNetworkManager = passData->data->mpNetworkManager;
		data->playerIsConnected = false;
	}

	virtual void GoToNextState(ApplicationState *passData);

private:

	int waitFrames = 30;
	int selectedOptionIndex = -1;
};
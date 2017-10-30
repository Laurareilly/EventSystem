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
		printf("On Arrive From Previous");
		if (playerWinner == 0)
			data->headerMessage[0] = "Bee man wins! Would you like to: ";
		else
			data->headerMessage[0] = "Flower guy wins! Would you like to: ";
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
	virtual void setWinner(int winner) { playerWinner = winner; }

private:

	int waitFrames = 30;
	int selectedOptionIndex = -1;
	int playerWinner; //0 for player 1, and then 1 for player 2
};
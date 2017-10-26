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

	virtual void ReceiveBoidData() {};

	virtual void SyncBoidList() {};
	virtual void SendBoidData() {};
	virtual void AcceptedToServer() {};
	//virtual SystemAddress GetPeerAddress() { return data->peerSystemAddress; }

	virtual std::string* GetHeaderMessage() { return data->headerMessage; };

	bool isServer;

	virtual void OnArriveFromPrevious(ApplicationState *passData)
	{
		data->headerMessage[0] = "";
		data->headerMessage[1] = "";
		data->headerMessage[2] = "";
		data->headerMessage[3] = "";
		data->headerMessage[4] = "";
		data->headerMessage[5] = "";
		data->headerMessage[6] = "";

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
	}

	virtual void GoToNextState(ApplicationState *passData);

private:

	bool wantsToBeSever = false;
	bool successfullyConnectedToServer = false;
	int waitFrames = 30;
	bool escapePressed;
	bool addButtonPressed;
	bool deleteButtonPressed;
};
#pragma once
#include "ApplicationState.h"

class HomeScreen : public ApplicationState
{
public:
	HomeScreen();
	~HomeScreen() {};

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

	std::string* GetHeaderMessage() { return data->headerMessage; }

	virtual void OnArriveFromPrevious(ApplicationState *passData)
	{
		data->headerMessage[0] = "Welcome 2 boids! Choose Option:";
		data->headerMessage[1] = " 1: Local Boids";
		data->headerMessage[2] = " 2: Join Networked Boids";
		data->headerMessage[3] = " 3: Create Networked Boids (Data Push)";
		data->headerMessage[4] = " 4: Create Networked Boids (Data Share)";
		data->headerMessage[5] = " 5: Create Networked Boids (Data Couple)";
		data->headerMessage[6] = " 6: Quit";
			
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

	bool wantsToBeSever = false;
	bool successfullyConnectedToServer = false;
	int waitFrames = 30;
	int selectedOptionIndex = -1;
	bool tryingToConnect;
};
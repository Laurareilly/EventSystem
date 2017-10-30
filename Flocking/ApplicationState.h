#ifndef APPLICATION_STATE
#define APPLICATION_STATE


#include <string>

class NetworkManager;

class ApplicationState abstract
{
public:
	friend class HomeScreen;
	friend class ActiveGameState;
	friend class EndGameState;
	friend class MyEvents;
	friend class Event;

	virtual void UpdateInput() = 0;
	virtual void UpdateNetworking() = 0;
	virtual void UpdateState() = 0;
	virtual void Display() = 0;

	virtual void ForcePlayerToLobby() = 0;

	virtual int getIsLocal() = 0;
	virtual void AcceptedToServer() = 0;

	virtual std::string* GetHeaderMessage() = 0;

	virtual int AddToScore(int cScore) = 0;
	virtual void setWinner(int playerWinner) = 0;

	ApplicationState();
	~ApplicationState();

	virtual void GoToNextState(ApplicationState *passData) = 0;
protected:
	ApplicationState *next, *previous;

	struct ApplicationStateData
	{
		//SystemAddress peerSystemAddress;
		unsigned int isLocal = 1; //0 = networked
		unsigned int clientID = 1;
		std::string headerMessage[4];
		char doesUpdateInput;
		char doesUpdateNetworking;
		char doesUpdateState;
		char doesDisplay;
		char doesSendData;
		int portNumber = 5546;
		char *ipAddress = "127.0.0.1";

		char enterServer = 0;
		NetworkManager *mpNetworkManager;
		bool playerIsConnected = false;
	}data[1];

	virtual void OnArriveFromPrevious(ApplicationState *passData) = 0;

};
#endif // APPLICATION_STATE
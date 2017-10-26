#include "NetworkManager.h"
#include "ApplicationState.h"

ApplicationState::ApplicationState()
{
	 data->mpNetworkManager = new NetworkManager();
}

ApplicationState::~ApplicationState()
{
	delete data->mpNetworkManager; data->mpNetworkManager = nullptr;
}

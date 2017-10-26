#include "NetworkManager.h"
#include "UnitManager.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "GraphicsSystem.h"
#include "ApplicationState.h"
#include "MyEvents.h"
#include "Event.h"


void NetworkManager::sendBeeTarget(Vector2D pos)
{
	BeeTarget target[1] = { ID_BEE_TARGET, 0,0 };
	target[0].posX = pos.getX();
	target[0].posY = pos.getY();
	mpPeer->Send((char*)target, sizeof(target), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

NetworkManager::NetworkManager()
{
	mpPeer = RakPeerInterface::GetInstance();
}

NetworkManager::~NetworkManager()
{
	RakPeerInterface::DestroyInstance(mpPeer);
}

void NetworkManager::initServer(int cPort)
{
	maxClients = 1;
	serverPort = cPort;
	sd = new SocketDescriptor(serverPort, 0);
	mpPeer->SetMaximumIncomingConnections(maxClients);
	//mpPeer->SetTimeoutTime(1000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	//mpPeer->SetOccasionalPing(true);
	//mpPeer->SetUnreliableTimeout(1000);
	mpPeer->Startup(maxClients, sd, 1);
	mIsServer = true;
}

void NetworkManager::initClient(int cPort, char * cIP)
{
	mIsServer = false;
	sd = new SocketDescriptor();
	mpPeer->Startup(1, sd, 1);
	//mpPeer->SetOccasionalPing(true);

	if (!mpPeer)
	{
		printf("Peer does not exist");
	}

	serverPort = cPort;
    mpPeer->Connect(cIP, serverPort, 0, 0);
}

void NetworkManager::Update()
{
	mpPeer;
	//std::cout << "networkmanger update" << std::endl;
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPacket = mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{

		std::cout << mpPacket->data[0] << std::endl;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("Client disconnects");
			gpGame->theState->ForcePlayerToLobby();
			mpPeer->CloseConnection(mpPacket->systemAddress, true);
			break;
		case ID_REMOTE_CONNECTION_LOST:
			gpGame->theState->ForcePlayerToLobby();
			mpPeer->CloseConnection(mpPacket->systemAddress, true);
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("Another client has connected.\n");
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A client has connected.\n");
			break;
		case ID_NEW_CLIENT_JOIN:
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
		{
			printf("Our connection request has been accepted.\n");
			gpGame->theState->AcceptedToServer();

			/*char sendBuff[2048];
			unsigned int bytesWritten = 0;
			sendBuff[0] = TEST;
			bytesWritten++;

			mpPeer->Send(sendBuff, bytesWritten, HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);*/
			//mpPeer->Send(sendBuff, bytesWritten, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

			UsernameMessage username[1] = { ID_ASK_FOR_DATA_METHOD,  "", "WHATS UR data method" };
			username[0].messageID;
		
			mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
			//mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, true);
		}
			break;
		case ID_CLIENT_NUMBER:
			break;
		case ID_CLIENT_TO_SERVER:
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			printf("Disconnection");
			gpGame->theState->ForcePlayerToLobby();
			mpPeer->CloseConnection(mpPacket->systemAddress, true);
			break;
		case ID_CONNECTION_LOST:
			gpGame->theState->ForcePlayerToLobby();
			break;
		case ID_PEER_LEAVE:
			gpGame->theState->ForcePlayerToLobby();
			mpPeer->CloseConnection(mpPacket->systemAddress, true);
			break;		
		case ID_ASK_FOR_DATA_METHOD:
		{
			ClientNumberMessage msg[1] = { ID_SEND_DATA_METHOD, 0 };
			msg[0].clientNumber = getCurrentDataMethod();

			gpGame->theState->AcceptedToServer();

			mpPeer->Send((char*)msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
		}
		case ID_SEND_DATA_METHOD:
		{
			ClientNumberMessage *username = (ClientNumberMessage*)mpPacket->data;

			setCurrentDataMethod(username->clientNumber);
		}
		break;
		case ID_BEE_TARGET:
		{
			BeeTarget *target = (BeeTarget*)mpPacket->data;
			Vector2D pos = Vector2D(target->posX, target->posY);
			MovePlayerEvent *movePlayer = new MovePlayerEvent(gpGame->getPlayer(), pos);
			EventManager::mpInstance->AddEvent(movePlayer);
		}
		break;
		default:
		{
			std::cout << "receiving id: " << mpPacket->data[0] << std::endl;
		}
		break;
		}
	}
}

void NetworkManager::SendDisconnection()
{
	ClientNumberMessage clientNumber[1] = { ID_DISCONNECTION_NOTIFICATION, 0 };
	//send
	mpPeer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPeer->GetSystemAddressFromIndex(0), false);
}
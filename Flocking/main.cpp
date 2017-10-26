/*main.cpp
 *	Adapted from: Example program for the Allegro library, by Shawn Hargreaves.
 *	Sets up all systems and runs the main game loop until it is time to quit.
 *
 *	Dean Lawson
 *	Champlain College
 *	2011
 *
 */
#define ALLEGRO_USE_CONSOLE

#include "RakNet\WindowsIncludes.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>

#include "Game.h"
#include "Data.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Unit.h"
#include "Timer.h"
#include "PerformanceTracker.h"
#include "MemoryTracker.h"
#include "NotificationQueue.h"
#include "CircularQueue.h"
#include "ComponentManager.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"

#include "ApplicationState.h"
#include "HomeScreen.h"
#include "ActiveGameState.h"


using namespace std;

//test callback
void testCallback(Notification* pMessage)
{
	cout << "message callback: message id=" << pMessage->getID() << endl;
}

PerformanceTracker* gpPerformanceTracker = NULL;


int main(int argc, char **argv)
{
	gpPerformanceTracker = new PerformanceTracker;
	gpPerformanceTracker->startTracking("init");

	//create the global game object
	gpGame = new Game;
	gpData = new Data;
	//init the game
	bool goodGame = gpGame->init();
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}

	gpPerformanceTracker->stopTracking("init");
	//cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";

	bool shouldExit = false;


	Timer fps;
	fps.start();
	//game loop
	while (!shouldExit)
	{
		gpPerformanceTracker->clearTracker("loop");
		gpPerformanceTracker->startTracking("loop");

		gpGame->beginLoop();

		gpPerformanceTracker->clearTracker("draw");
		gpPerformanceTracker->startTracking("draw");

		gpGame->theState->UpdateInput();
		gpGame->theState->UpdateNetworking();
		gpGame->theState->UpdateState();
		gpGame->theState->Display();
		//gpGame->processLoop();

		gpPerformanceTracker->stopTracking("draw");

		shouldExit = gpGame->endLoop();

		gpPerformanceTracker->stopTracking("loop");

		gpGame->updateTime(fps.getElapsedTime());
		fps.stop();
	}

	gpGame->cleanup();
	delete gpGame;
	gpGame = NULL;

	delete gpData;
	gpData = NULL;

	delete gpPerformanceTracker;
	gMemoryTracker.reportAllocations( cout );
	system("pause");

	return 0;
}
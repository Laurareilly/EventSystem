#include "RakNet\WindowsIncludes.h"
#include <stdio.h>
#include <assert.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <sstream>

#include "Game.h"
#include "Data.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "Font.h"
#include "InputManager.h"
#include "ExitGameMessage.h"
#include "AddUnitMessage.h"
#include "DeleteUnitMessage.h"
#include <time.h>    
#include <stdlib.h>

#include "HomeScreen.h"
#include "ActiveGameState.h"
#include "EndGameState.h"
#include "Event.h"

Game* gpGame = NULL;
EventManager* EventManager::mpInstance = nullptr;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mShouldExit(false)
	,mpFont(NULL)
	,mpSample(NULL)
	,mBackgroundBufferID(INVALID_ID)
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpInputManager(NULL)
{
	theHomeScreen = new HomeScreen();
	theGameState = new ActiveGameState();
	theEndState = new EndGameState();
	theState = theHomeScreen;
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	EventManager::InitInstance();

	srand(time(NULL));
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//startup allegro
	if(!al_init()) 
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return false;
	}

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager();
	mpSpriteManager = new SpriteManager();

	//startup a lot of allegro stuff

	//load image loader addon
	if( !al_init_image_addon() )
	{
		fprintf(stderr, "image addon failed to load!\n");
		return false;
	}

	//install audio stuff
	if( !al_install_audio() )
	{
		fprintf(stderr, "failed to initialize sound!\n");
		return false;
	}

	if(!al_init_acodec_addon())
	{
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return false;
	}
 
	if (!al_reserve_samples(1))
	{
		fprintf(stderr, "failed to reserve samples!\n");
		return false;
	}

	//actually load the font
	mpFont = new Font( "font.ttf", 40 );
	if( mpFont == NULL )
	{
		printf( "ttf font file not loaded properly!\n" ); 
		return false;
	}

	//show the mouse
	/*if( !al_hide_mouse_cursor( mpGraphicsSystem->getDisplay() ) )
	{
		printf( "Mouse cursor not able to be hidden!\n" ); 
		return false;
	}*/

	if( !al_init_primitives_addon() )
	{
		printf( "Primitives addon not added!\n" ); 
		return false;
	}

	//load the sample
	mpSample = al_load_sample( "clapping.wav" );
	if (!mpSample)
	{
		printf( "Audio clip sample not loaded!\n" ); 
		return false;
	}

	mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	//load buffers
	mBackgroundBufferID = mpGraphicsBufferManager->loadBuffer("wallpaper.bmp");
	mPlayerIconBufferID = mpGraphicsBufferManager->loadBuffer("arrow.png");
	mEnemyIconBufferID = mpGraphicsBufferManager->loadBuffer("arrow.png");
	mBlueFlowerID = mpGraphicsBufferManager->loadBuffer("BlueFlower.png");
	mGreenFlowerID = mpGraphicsBufferManager->loadBuffer("YellowFlower.png");
	mRedFlowerID = mpGraphicsBufferManager->loadBuffer("RedFlower.png");
	IDType targetID = mpGraphicsBufferManager->loadBuffer("target.png");
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, pPlayerBuffer->getWidth(), pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, pAIBuffer->getWidth(), pAIBuffer->getHeight());
	}
	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(targetID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, pTargetBuffer->getWidth(), pTargetBuffer->getHeight());
	}

	////////////flowers
	GraphicsBuffer* pBlueFlowerBuffer = mpGraphicsBufferManager->getBuffer(mBlueFlowerID);
	if (pBlueFlowerBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(BLUEFLOWER_SPRITE_ID, pBlueFlowerBuffer, 0, 0, pBlueFlowerBuffer->getWidth(), pBlueFlowerBuffer->getHeight());
	}

	GraphicsBuffer* pGreenFlowerBuffer = mpGraphicsBufferManager->getBuffer(mGreenFlowerID);
	if (pGreenFlowerBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(GREENFLOWER_SPRITE_ID, pGreenFlowerBuffer, 0, 0, pGreenFlowerBuffer->getWidth(), pGreenFlowerBuffer->getHeight());
	}

	GraphicsBuffer* pRedFlowerBuffer = mpGraphicsBufferManager->getBuffer(mRedFlowerID);
	if (pRedFlowerBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(REDFLOWER_SPRITE_ID, pRedFlowerBuffer, 0, 0, pRedFlowerBuffer->getWidth(), pRedFlowerBuffer->getHeight());
	}

	//////////////

	mpInputManager = new InputManager();
	mpInputManager->init();


	mpPlayer = gpGame->getUnitManager()->createPlayerUnit(*gpGame->getSpriteManager()->getSprite(PLAYER_ICON_SPRITE_ID), true, PositionData(Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()), 0.0f));
	mpPlayer->getPositionComponent()->setFacing((rand() % (int)MAX_DEGREES) * 0.0174533);
	mpPlayer->setSteering(Steering::ARRIVE_AND_FACE, ZERO_VECTOR2D, PLAYER_UNIT_ID);


	gpData->loadData();

	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;

	al_destroy_sample(mpSample);
	mpSample = NULL;
	delete mpFont;
	mpFont = NULL;

	delete mpInputManager;
	mpInputManager = NULL;

	//shutdown components
	al_uninstall_audio();
	al_shutdown_image_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_shutdown_primitives_addon();

}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);

	EventManager::mpInstance->ExecuteAll();
	mpMessageManager->processMessagesForThisframe();

	//processInput();
}

void Game:: render()
{
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	mpGraphicsSystem->draw(*(mpGraphicsSystem->getBackBuffer()), *pBackgroundSprite, 0.0f, 0.0f);

	mpGraphicsSystem->drawText(*mpFont, 500, 10, BLACK_COLOR, theState->GetHeaderMessage()[0]);
	mpGraphicsSystem->drawText(*mpFont, 500, 50, BLACK_COLOR, theState->GetHeaderMessage()[1]);
	mpGraphicsSystem->drawText(*mpFont, 500, 100, BLACK_COLOR, theState->GetHeaderMessage()[2]);
	mpGraphicsSystem->drawText(*mpFont, 500, 150, BLACK_COLOR, theState->GetHeaderMessage()[3]);

	mpUnitManager->drawAll();
	mpGraphicsSystem->swap();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

void Game::processInput()
{
}

void Game::updateTime(double time)
{
	mFrameTime = time - mLastUpdateTime;
	mLastUpdateTime = time;
}

void Game::exitGame()
{
	gpData->saveData();
	mShouldExit = true;
}
#pragma once

#include "RakNet\WindowsIncludes.h"
#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <string>

class GraphicsSystem;
class GraphicsBuffer;
class GraphicsBufferManager;
class SpriteManager;
class KinematicUnit;
class GameMessageManager;
class Timer;
class ComponentManager;
class UnitManager;
class Font;
class InputManager;
class HomeScreen;
class ApplicationState;
class ActiveGameState;
class EndGameState;
class Unit;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType BLUEFLOWER_SPRITE_ID = 4;
const IDType GREENFLOWER_SPRITE_ID = 5;
const IDType REDFLOWER_SPRITE_ID = 6;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class Game:public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	void processInput();
	bool endLoop();
	void exitGame();
	void updateTime(double time);
	void render();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline Font* getFont() const { return mpFont; };



	ApplicationState *theState;
	HomeScreen *theHomeScreen;
	EndGameState *theEndState;
	ActiveGameState *theGameState;

	InputManager* getInputManager() { return mpInputManager; }

	Unit* getPlayer() { return mpPlayer; }

private:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	bool mShouldExit;
	Unit* mpPlayer;

	//should be somewhere else
	Font* mpFont;
	InputManager* mpInputManager;
	ALLEGRO_SAMPLE* mpSample;
	IDType mBackgroundBufferID;
	IDType mPlayerIconBufferID;
	IDType mEnemyIconBufferID;
	IDType mGreenFlowerID;
	IDType mBlueFlowerID;
	IDType mRedFlowerID;


	double mFrameTime;
	double mLastUpdateTime;

};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;


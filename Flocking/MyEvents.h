#ifndef MYEVENTS
#define MYEVENTS


#include "Event.h"
#include <iostream>
#include "Game.h"
#include "UnitManager.h"
#include "Vector2D.h"
#include "Flower.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "ApplicationState.h"

class MovePlayerEvent : public Event
{
public:
	MovePlayerEvent(Unit* player, Vector2D pos)
	{
		mpPlayer = player;
		mMousePos = pos;
	}

	~MovePlayerEvent() { };

	virtual int Execute()
	{
		mpPlayer->setSteering(Steering::ARRIVE_AND_FACE, mMousePos);
		std::cout << "Move Player" << std::endl;
		return 0;
	}
private:
	Unit* mpPlayer;
	Vector2D mMousePos;
};

class ChangeFlowerEvent : public Event
{
	virtual int Execute()
	{
		std::cout << "Change Flower" << std::endl;
		return 0;
	}
};

class SpawnFlowerEvent : public Event
{
public:
	SpawnFlowerEvent(int flowerType, Vector2D pos)
	{
		mFlowerType = flowerType;
		mPosition = pos;
	}

	~SpawnFlowerEvent() { };

	virtual int Execute()
	{
		std::cout << "Spawn Flower" << std::endl;
		Flower *newFlower;
		switch (mFlowerType)
		{
		case 0: //blue
			newFlower = (Flower*)gpGame->getUnitManager()->createUnit(true, *gpGame->getSpriteManager()->getSprite(BLUEFLOWER_SPRITE_ID), true, PositionData(mPosition, 0), ZERO_PHYSICS_DATA);
			newFlower->setFlowerType(mFlowerType);
			break;
		case 1: //green 
			newFlower = (Flower*)gpGame->getUnitManager()->createUnit(true, *gpGame->getSpriteManager()->getSprite(GREENFLOWER_SPRITE_ID), true, PositionData(mPosition, 0), ZERO_PHYSICS_DATA);
			newFlower->setFlowerType(mFlowerType);
			break;
		case 2: //red
			newFlower = (Flower*)gpGame->getUnitManager()->createUnit(true, *gpGame->getSpriteManager()->getSprite(REDFLOWER_SPRITE_ID), true, PositionData(mPosition, 0), ZERO_PHYSICS_DATA);
			newFlower->setFlowerType(mFlowerType);
			break;
		}

		return 0;
	}
private:
	int mFlowerType;
	Vector2D mPosition;
};

class AddToScoreEvent : public Event
{
public:
	AddToScoreEvent(int cScore)
	{
		deltaScore = cScore;
	}

	~AddToScoreEvent() {};

	virtual int Execute()
	{
		std::cout << "Add to Score" << std::endl;
		gpGame->theState->AddToScore(deltaScore);
		return 0;
	}
private:
	int deltaScore = 0;
};

class EndGameEvent : public Event
{
public:
	EndGameEvent(int playerWinner)
	{
		mPlayerWinner = playerWinner;
	}

	~EndGameEvent() {}
	virtual int Execute()
	{
		std::cout << "Game over" << std::endl;	
		//tell the end screen who won so we can adjust the header text
		gpGame->theState->setWinner(mPlayerWinner);
		//clear all the units left in game
		gpGame->getUnitManager()->deleteAllUnits();
		//go to the end state
		gpGame->theState->GoToNextState((ApplicationState*)gpGame->theGameState);
		return 0;
	}
private:
	int mPlayerWinner;
};



#endif
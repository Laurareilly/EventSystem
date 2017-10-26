#ifndef MYEVENTS
#define MYEVENTS


#include "Event.h"
#include <iostream>
#include "Game.h"
#include "UnitManager.h"
#include "Vector2D.h"

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
	virtual int Execute()
	{
		std::cout << "Spawn Flower" << std::endl;
		return 0;
	}
};

class AddToScoreEvent : public Event
{
	virtual int Execute()
	{
		std::cout << "Add to Score" << std::endl;
		return 0;
	}
};

class PlayerDieEvent : public Event
{
	virtual int Execute()
	{
		std::cout << "Player died" << std::endl;
		return 0;
	}
};


#endif
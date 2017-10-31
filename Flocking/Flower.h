#pragma once

/*
Lab 3- Event Manager
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- 0935286

We certify that this work is entirely our own.
The assessor of this project may reproduce this
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.”
*/

#include "Unit.h"
#include "Game.h"

class Flower : public Unit
{
	friend class UnitManager;

public:
	Flower(const Sprite *aSprite) :Unit(*aSprite) { mpPlayer = gpGame->getPlayer(); }
	~Flower() {};

	bool update();

	void draw();

	void setFlowerType(int type);


private:
	int mFlowerType;
	Unit* mpPlayer;
	const int collisionRadius = 32;
	int pointWorth = 0;
};
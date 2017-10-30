#pragma once

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
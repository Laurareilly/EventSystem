#pragma once

#include "Unit.h"

class Flower : public Unit
{
	friend class UnitManager;

public:
	Flower(const Sprite *aSprite) :Unit(*aSprite) {}
	~Flower() {};

	void update()
	{
		//do collision checks here
		//make sure when u delete, u reference it by id and not random unit
	}

	void draw()
	{
		Unit::draw();
	}

	void setFlowerType(int type) { mFlowerType = type; }

private:
	int mFlowerType;
	const int collisionRadius = 32;
};
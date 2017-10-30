#pragma once

#include "Unit.h"

class Flower : public Unit
{
	friend class UnitManager;

public:
	Flower(const Sprite *aSprite) :Unit(*aSprite) { mpPlayer = gpGame->getPlayer(); }
	~Flower() {};

	void update()
	{
		//do collision checks here
		//make sure when u delete, u reference it by id and not random unit
		if (mpPlayer != nullptr)
		{
			float diff = (mpPlayer->getPositionComponent()->getPosition() - Unit::getPositionComponent()->getPosition()).getLength();
			if (diff <= collisionRadius)
			{
				printf("COLLISION DETECTED!\n");
			}
		}
	}

	void draw()
	{
		Unit::draw();
	}

	void setFlowerType(int type) { mFlowerType = type; }

private:
	int mFlowerType;
	Unit* mpPlayer;
	const int collisionRadius = 32;
};
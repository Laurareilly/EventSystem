#include "Flower.h"
#include "Event.h"
#include "MyEvents.h"


bool Flower::update()
{
	//do collision checks here
	//make sure when u delete, u reference it by id and not random unit
	//printf("UPDATING FLOWER!\n");
	if (mpPlayer != nullptr)
	{
		float diff = (mpPlayer->getPositionComponent()->getPosition() - Unit::getPositionComponent()->getPosition()).getLength();
		if (diff <= collisionRadius)
		{
			printf("COLLISION DETECTED!\n");
			AddToScoreEvent *scoreEvent = new AddToScoreEvent(pointWorth);
			EventManager::mpInstance->AddEvent(scoreEvent);
			gpGame->getUnitManager()->deleteUnit(Unit::getID());
			return true;
		}
	}
	return false;
}

void Flower::draw()
{
	Unit::draw();
}

void Flower::setFlowerType(int type)
{
	mFlowerType = type;
	if (type == 0)
		pointWorth = 5;
	if (type == 1)
		pointWorth = 15;
	if (type == 2)
		pointWorth = -30;
}

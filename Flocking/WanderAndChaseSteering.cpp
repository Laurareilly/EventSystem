#include "WanderAndChaseSteering.h"
#include "UnitManager.h"
#include "Game.h"

WanderAndChaseSteering::WanderAndChaseSteering(const UnitID& ownerID, Vector2D targetLoc, float targetRadius, const UnitID& targetID): Steering(WANDER_AND_CHASE)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	mTargetRadius = targetRadius;
	//mpArriveAndFace = new ArriveAndFaceSteering(ownerID, targetLoc);
	mpWander = new WanderSteering(ownerID, 3000, 150, targetID);
	mpSeekAndFace = new SeekAndFace(ownerID, targetLoc);
}

WanderAndChaseSteering::~WanderAndChaseSteering()
{
	delete mpSeekAndFace;
	mpSeekAndFace = NULL;

	delete mpWander;
	mpWander = NULL;
}

Steering* WanderAndChaseSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPosition = pOwner->getPositionComponent()->getPosition();


	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		
		if (pTarget != NULL)
		{
			mTargetLoc = pTarget->getPositionComponent()->getPosition();
		}
	}

	Vector2D difference = mTargetLoc - ownerPosition;
	float distance = difference.getLength();
	
	//if we're in radius of the target, chase the target
	if (distance <= mTargetRadius)
	{
		Steering* pSteer = mpSeekAndFace->getSteering();
		if (pSteer != NULL)
		{
			this->mData = pSteer->getData();
		}
		delete mpSeekAndFace;
		mpSeekAndFace = new SeekAndFace(mOwnerID, mTargetLoc);
	}
	//otherwise we just want to wander
	else 
	{
		Steering* pSteer = mpWander->getSteering();
		if (pSteer != NULL)
		{
			this->mData = pSteer->getData();
		}
	}

	return this;
}

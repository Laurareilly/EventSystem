#include "WanderSteering.h"
#include "UnitManager.h"
#include "Game.h"
#include "GraphicsSystem.h"

WanderSteering::WanderSteering(const UnitID& ownerID, float delay, float targetRadius, const UnitID& targetID, WanderType type) : Steering(WANDER)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	Vector2D randomTarget =  Vector2D( static_cast<int>(rand() % gpGame->getGraphicsSystem()->getWidth()), static_cast<int>(rand() % gpGame->getGraphicsSystem()->getHeight()) );
	setTargetLoc(randomTarget);
	mSwitchTime = gpGame->getCurrentTime();
	mDelay = delay;
	mTargetRadius = targetRadius;
	mpSeekAndFace = new SeekAndFace(ownerID, randomTarget);
	mWanderType = type;
}

Steering* WanderSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPosition = pOwner->getPositionComponent()->getPosition();

	Vector2D difference = mTargetLoc - ownerPosition;
	float distance = difference.getLength();
	
	switch (mWanderType)
	{
		//if we're in radius of a random target, choose a new random target and go to that one
	case LOCATION_WANDER:
		if (distance <=  mTargetRadius)
		{
			float randomX = rand() % gpGame->getGraphicsSystem()->getWidth();
			float randomY = rand() % gpGame->getGraphicsSystem()->getHeight();
			mTargetLoc = Vector2D(randomX, randomY);

			delete mpSeekAndFace;
			mpSeekAndFace = new SeekAndFace(mOwnerID, mTargetLoc);
		}
		else //if we aren't in our target radius, we just want to seek the same pos
		{
			Steering* pSteer = mpSeekAndFace->getSteering();
			if (pSteer != NULL)
			{
				this->mData = pSteer->getData();
			}
		}
		break;
		//choose a new random target every x seconds
	case TIMED_WANDER:
		//i looked at Tyler's to see how he got the time
		if (gpGame->getCurrentTime() >= mSwitchTime)
		{
			mSwitchTime = gpGame->getCurrentTime() + mDelay;
			float randomX = rand() % gpGame->getGraphicsSystem()->getWidth();
			float randomY = rand() % gpGame->getGraphicsSystem()->getHeight();
			mTargetLoc = Vector2D(randomX, randomY);

			mSwitchTime = gpGame->getCurrentTime() + mDelay;

			delete mpSeekAndFace;
			mpSeekAndFace = new SeekAndFace(mOwnerID, mTargetLoc);
		}
		//if we aren't greater than the switch time then we just want to seek the same pos
		else
		{
			Steering* pSteer = mpSeekAndFace->getSteering();
			if (pSteer != NULL)
			{
				this->mData = pSteer->getData();
			}
		}
		break;
	}
	return this;
}


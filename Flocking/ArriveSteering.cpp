#include "ArriveSteering.h"
#include "UnitManager.h"
#include "Game.h"

ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, float slowRadius, float timeToTarget, const UnitID& targetID): Steering(ARRIVE)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	mTimeToTarget = timeToTarget;
	mSlowRadius = slowRadius;
}

Steering* ArriveSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPosition = pOwner->getPositionComponent()->getPosition();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();


	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL); //if statement that breaks if false
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	//direction to target
	Vector2D direction = mTargetLoc - ownerPosition;
	float distance = direction.getLength();

	//are we within the radius?
	if (distance < mTargetRadius)
	{
		return this;
	}

	//go at the max speed if we aren't in the slow down zone
	if (distance > mSlowRadius)
	{
		mTargetSpeed = data.maxSpeed;
	}
	else
	{
		mTargetSpeed = data.maxSpeed * distance / mSlowRadius;
	}

	//the target velocity combines the speed and direction (AI for games)
	Vector2D targetVelocity = direction;
	targetVelocity.normalize();
	targetVelocity *= mTargetSpeed;

	direction = targetVelocity - data.vel;
	direction /= mTimeToTarget;

	//cap it so we don't go too fast
	if(direction.getLength() > pOwner->getMaxAcc())
	{
		direction.normalize();
		direction *= pOwner->getMaxAcc();
	}

	data.acc = direction;
	this->mData = data;
	return this;
}




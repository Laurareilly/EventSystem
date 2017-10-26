#include "FaceSteering.h"
#include "UnitManager.h"
#include "Game.h"

FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, float slowRadius, const UnitID& targetID) : Steering(FACE)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc); 
	mSlowRadius = slowRadius;
}

Steering* FaceSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPosition = pOwner->getPositionComponent()->getPosition();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D rightVector = { 1,0 };
	float targetRotation;
	float rotationSize;

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL); //if statement that breaks if false
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	Vector2D difference = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	//if(difference.getLength() >= 0)
	//{
	//	return this;
	//}
	
	//check for a zero direction- make no change if so (from AI for Games book)
	if (difference.getLength() == 0)
	{
		return this;
	}

	// http://math.stackexchange.com/questions/878785/how-to-find-an-angle-in-range0-360-between-2-vectors/879474#879474 
	float dot = (rightVector.getX() * difference.getX() + rightVector.getY() * difference.getY()); //need the dot product to find the angle between the owner and target
	float det = (rightVector.getX() * difference.getY()) - (rightVector.getY() * difference.getX()); //need the determinant to find the angle between the owner and target
	float angle = atan2(det, dot); //the angle between the owner and target
	float angleDifference = angle - pOwner->getFacing(); //difference between angle between the owner and target and whatever direction the owner is facing

	angleDifference = wrapRange(angleDifference);
	
	rotationSize = abs(angleDifference);

	/*
	if (rotationSize < mTargetRadius)
	{
		return this;
	}*/

	if (rotationSize > mSlowRadius)
	{
		targetRotation = pOwner->getMaxRotAcc();
	}
	else
	{
		targetRotation = pOwner->getMaxRotAcc() * rotationSize / mSlowRadius;
	}

	targetRotation *= angleDifference / rotationSize;

	data.rotAcc = targetRotation - data.rotVel;
	data.rotAcc /= mTimeToTarget;

	this->mData = data;
	return this;
}

float FaceSteering::wrapRange(float angle)
{
	//http://stackoverflow.com/questions/11980292/how-to-wrap-around-a-range
	float PI = 3.14159265358979323846;

	while (angle > PI)
	{
		angle -= 2 * PI;
	}
	while (angle < -PI)
	{
		angle += 2 * PI;
	}
	return angle;
}

/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 3- Flocking
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#include "SeparationSteering.h"
#include "UnitManager.h"
#include "Game.h"
#include "Data.h"

SeparationSteering::SeparationSteering(const UnitID& ownerID, float threshold) : Steering(SEPARATION)
{
	setOwnerID(ownerID);
	mThreshold = threshold;
}

Steering* SeparationSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPos = pOwner->getPositionComponent()->getPosition();
	PhysicsData ownerData = pOwner->getPhysicsComponent()->getData();

	std::map<UnitID, Unit*> potentialTargets = gpGame->getUnitManager()->mUnitMap;

	float maxAcceleration = ownerData.maxSpeed;

	for (auto it = potentialTargets.begin(); it != potentialTargets.end(); ++it)
	{
		if (it->second == pOwner || it->second->isReceived != pOwner->isReceived)
		{
			continue;
		}

		Vector2D direction = ownerPos - it->second->getPositionComponent()->getPosition();

		float distance = direction.getLengthSquared();

		if (distance < gpData->getSeparationRadius() * gpData->getSeparationRadius()) //if its in the range
		{
			//using linear separation because the other didn't work
			mStrength = pOwner->getMaxAcc() * (gpData->getSeparationRadius() - direction.getLength()) / gpData->getSeparationRadius();
			direction.normalize();
			ownerData.acc += direction * mStrength;
		}
	}

	this->mData = ownerData;
	return this;
}

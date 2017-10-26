/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 3- Flocking
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#include "CohesionSteering.h"
#include "UnitManager.h"
#include "Game.h"
#include "Data.h"

CohesionSteering::CohesionSteering(const UnitID& ownerID, float threshold) : Steering(COHESION)
{
	setOwnerID(ownerID);
	mThreshold = threshold;
}

Steering* CohesionSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPos = pOwner->getPositionComponent()->getPosition();
	PhysicsData ownerData = pOwner->getPhysicsComponent()->getData();

	std::map<UnitID, Unit*> potentialTargets = gpGame->getUnitManager()->mUnitMap;

	float maxAcceleration = ownerData.maxSpeed;
	Vector2D averagePosition = Vector2D(0, 0);
	int counter = 0;

	for (auto it = potentialTargets.begin(); it != potentialTargets.end(); ++it)
	{
		if (it->second == pOwner || it->second->isReceived != pOwner->isReceived)
		{
			continue;
		}

		Vector2D direction = ownerPos - it->second->getPositionComponent()->getPosition();

		float distance = direction.getLengthSquared();


		if (distance < gpData->getCohesionRadius() * gpData->getCohesionRadius()) //if its in the range
		{
			averagePosition += it->second->getPositionComponent()->getPosition();
			counter++;
		}
	}
	
	if (counter == 0)
	{
		ownerData.acc = Vector2D(0, 0);
		this->mData = ownerData;
		return this;
	}

	averagePosition /= counter;
	assert(averagePosition.getX() == averagePosition.getX());
	Vector2D averageDirection = averagePosition - ownerPos;
	averageDirection.normalize();
	averageDirection *= maxAcceleration;

	ownerData.acc = averageDirection;

	this->mData = ownerData;
	return this;
}

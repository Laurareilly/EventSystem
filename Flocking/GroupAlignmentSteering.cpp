#include "GroupAlignmentSteering.h"
#include "UnitManager.h"
#include "Game.h"
#include "Data.h"

GroupAlignmentSteering::GroupAlignmentSteering(const UnitID& ownerID, float radius) : Steering(GROUP_ALIGNMENT)
{
	setOwnerID(ownerID);
	mRadius = radius;
}

Steering* GroupAlignmentSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D ownerPosition = pOwner->getPositionComponent()->getPosition();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	std::map<UnitID, Unit*> potentialTargets = gpGame->getUnitManager()->mUnitMap;

	Vector2D velocitiesInRange = Vector2D(0, 0);
	int counter = 0;

	//going through all the units on screen
	for (auto it = potentialTargets.begin(); it != potentialTargets.end(); ++it)
	{
		if (it->second != pOwner && it->second->isReceived == pOwner->isReceived)
		{
			Vector2D direction = ownerPosition - it->second->getPositionComponent()->getPosition();

			float distance = direction.getLengthSquared();

			//out of all the units on screen which ones are actually in range?
			if (distance < gpData->getAlignRadius() * gpData->getAlignRadius())
			{
				velocitiesInRange += it->second->getPhysicsComponent()->getVelocity();
				counter++;
			}
		}
	}

	if (counter == 0)
	{
		Vector2D ownerFacing;
		float face = pOwner->getFacing();
		ownerFacing = Vector2D(cos(face), sin(face));
		ownerFacing.normalize();
		ownerFacing *= pOwner->getMaxSpeed();
		data.acc = ownerFacing;
		this->mData = data;
		return this;
	}

	velocitiesInRange /= counter;
	velocitiesInRange.normalize();

	velocitiesInRange *= pOwner->getMaxAcc();

	data.acc = velocitiesInRange;

	this->mData = data;
	return this;
}

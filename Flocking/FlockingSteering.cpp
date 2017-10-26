#include "FlockingSteering.h"
#include "Game.h"
#include "UnitManager.h"

FlockingSteering::FlockingSteering(const UnitID& ownerID, const Vector2D& targetLoc) : Steering(FLOCKING)
{
	setOwnerID(ownerID);
	mpSeparation = new SeparationSteering(ownerID, gpData->getSeparationRadius()); 
	mpCohesion = new CohesionSteering(ownerID, gpData->getCohesionRadius());
	mpAlign = new GroupAlignmentSteering(ownerID, gpData->getAlignRadius());
	mpFace = new FaceSteering(ownerID, targetLoc);
}

FlockingSteering::~FlockingSteering()
{
	delete mpSeparation;
	mpSeparation = NULL;
	delete mpCohesion;
	mpCohesion = NULL;
	delete mpAlign;
	mpAlign = NULL;
	delete mpFace;
	mpFace = NULL;
}

Steering* FlockingSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	mData = pOwner->getPhysicsComponent()->getData();

	PhysicsData separationData = mpSeparation->getSteering()->getData();
	PhysicsData cohesionData = mpCohesion->getSteering()->getData();
	PhysicsData alignData = mpAlign->getSteering()->getData();

	mData.acc = separationData.acc * gpData->getSeparationWeight() + cohesionData.acc * gpData->getCohesionWeight() + alignData.acc * gpData->getAlighnWeight();
	mpFace->setTargetLoc(mData.vel + pOwner->getPositionComponent()->getPosition());
	mData.rotAcc = mpFace->getSteering()->getData().rotAcc;

	return this;
}



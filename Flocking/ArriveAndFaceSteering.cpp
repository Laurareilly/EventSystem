#include "ArriveAndFaceSteering.h"
#include "UnitManager.h"
#include "Game.h"

ArriveAndFaceSteering::ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, float slowRadius, float timeToTarget, const UnitID& targetID): Steering(ARRIVE_AND_FACE)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mpArrive = new ArriveSteering(ownerID, targetLoc, 150, .1, targetID);
	mpFace = new FaceSteering(ownerID, targetLoc);
}

ArriveAndFaceSteering::~ArriveAndFaceSteering()
{
	delete mpArrive;
	mpArrive = NULL;

	delete mpFace;
	mpFace = NULL;
}

Steering* ArriveAndFaceSteering::getSteering()
{
	//just combines the data we need from arrive steering and face steering
	mData = gpGame->getUnitManager()->getUnit(mOwnerID)->getPhysicsComponent()->getData();
	mData.vel = mpArrive->getSteering()->getData().vel;
	mData.acc = mpArrive->getSteering()->getData().acc;
	mData.rotAcc = mpFace->getSteering()->getData().rotAcc;
	return this;
}


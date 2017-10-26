#include "SeekAndFace.h"
#include "UnitManager.h"
#include "Game.h"

SeekAndFace::SeekAndFace(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID): Steering(SEEK_AND_FACE)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	mpFace = new FaceSteering(ownerID, targetLoc);
	mpSeek = new SeekSteering(ownerID ,targetLoc, targetID, false);
}

SeekAndFace::~SeekAndFace()
{
	delete mpFace;
	mpFace = NULL;

	delete mpSeek;
	mpSeek = NULL;
}

Steering* SeekAndFace::getSteering()
{
	//just combines the data we need from seek steering and face steering
	mData = gpGame->getUnitManager()->getUnit(mOwnerID)->getPhysicsComponent()->getData();
	mData.acc = mpSeek->getSteering()->getData().acc;
	mData.rotAcc = mpFace->getSteering()->getData().rotAcc;
	return this;
}


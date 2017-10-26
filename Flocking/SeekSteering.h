#pragma once

#include <Trackable.h>
#include "Steering.h"

class SeekSteering : public Steering
{
	//so that wandersteering and seekandface steering can use our seek steering
	friend class WanderSteering;
	friend class SeekAndFace;

public:
	SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	//void setLocation(Vector2D targetPos); //for the wander steering
protected:
	virtual Steering* getSteering();
};
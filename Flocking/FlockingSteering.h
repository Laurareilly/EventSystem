#pragma once
#include "Steering.h"
#include "GroupAlignmentSteering.h"
#include "CohesionSteering.h"
#include "SeparationSteering.h"
#include "FaceSteering.h"
#include "Data.h"

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UnitID& ownerID, const Vector2D& targetLoc);
	~FlockingSteering();

protected:
	Steering* getSteering();

private:
	SeparationSteering* mpSeparation;
	CohesionSteering* mpCohesion;
	GroupAlignmentSteering* mpAlign;
	FaceSteering* mpFace;
};
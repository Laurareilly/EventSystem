#pragma once
#include "Steering.h"

class GroupAlignmentSteering : public Steering
{
	friend class FlockingSteering;

public:
	GroupAlignmentSteering(const UnitID& ownerID, float radius);
	~GroupAlignmentSteering() {};

protected:
	Steering* getSteering();
private:
	float mRadius; 
};
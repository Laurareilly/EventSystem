#pragma once
#include "Steering.h"

class SeparationSteering : public Steering
{
	friend class FlockingSteering;

public:
	SeparationSteering(const UnitID& ownerID, float threshold); //, float decayCoefficient);
	~SeparationSteering() {};
protected:
	Steering* getSteering();
private:
	float mThreshold;
	float mDecayCoefficient;
	float mStrength;
};
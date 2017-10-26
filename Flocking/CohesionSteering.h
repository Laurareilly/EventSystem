/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 3- Flocking
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "Steering.h"

class CohesionSteering : public Steering
{
	friend class FlockingSteering;

public:
	CohesionSteering(const UnitID& ownerID, float threshold);
	~CohesionSteering() {};
protected:
	Steering* getSteering();
private:
	float mThreshold;
	float mStrength;
	float mAttractionForce;
};
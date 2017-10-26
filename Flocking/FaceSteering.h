/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "Steering.h"

class FaceSteering: public Steering
{
	friend class ArriveAndFaceSteering;
	friend class SeekAndFace;
	friend class FlockingSteering;

public:
	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, float slowRadius = .5235, const UnitID& targetID = INVALID_UNIT_ID);
	~FaceSteering(){};
	float wrapRange(float angle);
private:
	float mTimeToTarget = 0.01f; 
protected:
	Steering* getSteering();
	float mSlowRadius; //the radius for beginning to slow down

};

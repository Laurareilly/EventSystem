/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "Steering.h"

class ArriveSteering: public Steering
{
	//so arrive and face can use our arrive steering
	friend class ArriveAndFaceSteering;

public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, float slowRadius, float timeToTarget, const UnitID& targetID = INVALID_UNIT_ID);
	~ArriveSteering() {}
protected:
	Steering* getSteering();
private:
	float mTimeToTarget;
	float mTargetRadius;
	float mSlowRadius;
	float mTargetSpeed;
};

/* 
Author: Laura Reilly 
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "ArriveSteering.h"
#include "FaceSteering.h"

class ArriveAndFaceSteering: public Steering
{
	//so that wander and chase can use our getSteering()
	//friend class WanderAndChaseSteering;
public:
	ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, float slowRadius = 50, float timeToTarget = .1, const UnitID& targetID = INVALID_UNIT_ID);
	~ArriveAndFaceSteering();

protected:
	Steering* getSteering();
	ArriveSteering* mpArrive;
	FaceSteering* mpFace;
};

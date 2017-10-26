/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "Steering.h"
//#include "ArriveAndFaceSteering.h"
#include "WanderSteering.h"
#include "SeekAndFace.h"

class WanderAndChaseSteering: public Steering
{
public:
	WanderAndChaseSteering(const UnitID& ownerID, Vector2D targetLoc, float targetRadius, const UnitID& targetID = INVALID_UNIT_ID);
	~WanderAndChaseSteering();
protected:
	Steering* getSteering();
	//ArriveAndFaceSteering* mpArriveAndFace;
	SeekAndFace* mpSeekAndFace;
	WanderSteering* mpWander;
private:
	float mTargetRadius;
};
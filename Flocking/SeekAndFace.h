/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "FaceSteering.h"
#include "SeekSteering.h"

class SeekAndFace: public Steering
{
	//so that wander steering can use our seek and face steering
	friend class WanderSteering;
	friend class WanderAndChaseSteering;

public:
	SeekAndFace(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	~SeekAndFace();

protected:
	Steering* getSteering() override;

	FaceSteering* mpFace;
	SeekSteering* mpSeek;
};
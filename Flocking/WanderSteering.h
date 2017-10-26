/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#pragma once
#include "SeekAndFace.h"

class WanderSteering: public Steering
{
public:
	//so that wanderandchase steering can use our wander steering
	friend class WanderAndChaseSteering;
	
	enum WanderType
	{
		TIMED_WANDER,
		LOCATION_WANDER
	};

	WanderSteering(const UnitID& ownerID, float delay, float targetRadius, const UnitID& targetID = INVALID_UNIT_ID, WanderType type = LOCATION_WANDER);
	~WanderSteering() { delete mpSeekAndFace; mpSeekAndFace = NULL; };

protected:
	Steering* getSteering();
private:
	float mSwitchTime;
	float mDelay;
	float mTargetRadius;
	WanderType mWanderType;
	SeekAndFace* mpSeekAndFace;
};
#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "SeekSteering.h"
#include "ArriveAndFaceSteering.h"
#include "WanderSteering.h"
#include "SeekAndFace.h"
#include "WanderAndChaseSteering.h"

//flocking
#include "SeparationSteering.h"
#include "GroupAlignmentSteering.h"
#include "CohesionSteering.h"
#include "FlockingSteering.h"

SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) 
	:Component(id)
	, mPhysicsComponentID(physicsComponentID)
	, mpSteering(NULL)
{
}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

void SteeringComponent::setData(const SteeringData& data)
{
	mData = data;

	switch (data.type)
	{
		case Steering::SEEK:
		{
			//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			//create new steering
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, false);
			break;
		}
		case Steering::FLEE:
		{
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, true);
			break;
		}
		case Steering::ARRIVE:
		{
			delete mpSteering;
			mpSteering = new ArriveSteering(data.ownerID, data.targetLoc, 150 , .1, data.targetID);
			break;
		}
		case Steering::FACE:
		{
			mpSteering = new FaceSteering(data.ownerID, data.targetLoc, .5235, data.targetID);
			break;
		}
		case Steering::ARRIVE_AND_FACE:
		{
			delete mpSteering;
			mpSteering = new ArriveAndFaceSteering(data.ownerID, data.targetLoc, 50, .1, data.targetID);
			break;
		}
		case Steering::WANDER:
		{
			delete mpSteering;
			mpSteering = new WanderSteering(data.ownerID, 3000, 150, data.targetID, (WanderSteering::WanderType)(rand() % 2));
			break;
		}
		case Steering::SEEK_AND_FACE:
		{
			delete mpSteering;
			mpSteering = new SeekAndFace(data.ownerID, data.targetID, data.targetID);
			break;
		}
		case Steering::WANDER_AND_CHASE:
		{
			delete mpSteering;
			mpSteering = new WanderAndChaseSteering(data.ownerID, data.targetID, 150, data.targetID);
			break;
		}
		//flocking
		case Steering::SEPARATION:
		{
			delete mpSteering;
			mpSteering = new SeparationSteering(data.ownerID, 100);
			break;
		}
		case Steering::COHESION:
		{
			delete mpSteering;
			mpSteering = new CohesionSteering(data.ownerID, 100);
			break;
		}
		case Steering::GROUP_ALIGNMENT:
		{
			delete mpSteering;
			mpSteering = new GroupAlignmentSteering(data.ownerID, 100);
			break;
		}
		case Steering::FLOCKING:
		{
			delete mpSteering;
			mpSteering = new FlockingSteering(data.ownerID, data.targetLoc);
			break;
		}
		default:
		{

		}
	};
}


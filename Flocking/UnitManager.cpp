#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;


int UnitManager::mBoidsOnScreen;


using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{

	//for (unsigned int i = 0; i < 15; ++i)
	//{
	//	mReceivedUnits[i] = NULL;
	//}
}

Unit* UnitManager::createUnit(bool addOnlyToReceived, const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		//if (addOnlyToReceived) 
		//	mReceivedUnits[theID] = pUnit;
		//else
		//	mUnitMap[theID] = pUnit;

		pUnit->isReceived = addOnlyToReceived;
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pUnit->mPositionComponentID = pComponentManager->allocatePositionComponent(posData, shouldWrap);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);

		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(false, sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{

	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	Unit* pUnit = createUnit(false, sprite, false, PositionData(Vector2D(posX, posY), 0), PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f));
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(Steering::SEEK, Vector2D(gpGame->getGraphicsSystem()->getWidth() / 2, gpGame->getGraphicsSystem()->getHeight() / 2));
	}
	return pUnit;
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

								 //remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);


		msNextUnitID--; //only works if we're taking from the end of the map

						//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() < 1) //make sure we dont try to delete any enemies if there aren't any
	{
		return;
	}

	Uint32 target = rand() % mUnitMap.size();
	Uint32 cnt = 0;

	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
	{
		if (cnt == target)
		{
			//if (it->first == PLAYER_UNIT_ID) //skip over the player unit
			//{
			//	deleteRandomUnit();
			//	return;
			//}
			deleteUnit(it->first);
			break;
		}
	}
}

void UnitManager::deleteIfShouldBeDeleted()
{
	//clean networked array?

	bool running = true;
	while (running)
	{
		running = false;
		for (auto it = mUnitMap.rbegin(); it != mUnitMap.rend(); ++it)
		{
			if (it->second->getShouldBeDeleted())
			{
				deleteUnit(it->first);
				running = true;
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}

	//for (unsigned int i = 0; i < 15; ++i)
	//{
	//	if(mReceivedUnits[i] != NULL) mReceivedUnits[i]->draw();
	//}

	for (auto it = mReceivedUnits.begin(); it != mReceivedUnits.end(); ++it)
	{
		if (it->second != NULL) it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}

	for (auto it = mReceivedUnits.begin(); it != mReceivedUnits.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}


void UnitManager::updateAll(bool shouldDelete)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		//if(!it->second->isReceived)
			it->second->setShouldBeDeleted(shouldDelete);
	}

	for (auto it = mReceivedUnits.begin(); it != mReceivedUnits.end(); ++it)
	{
		it->second->setShouldBeDeleted(shouldDelete);
	}
}

void UnitManager::cleanupBoids()
{
	updateAll(true); //this is really good (DELIGHTER)
	deleteIfShouldBeDeleted();
}

std::map<UnitID, Unit*> UnitManager::getLocalUnits(bool cReceived)
{
	return mUnitMap;
	//std::map<UnitID, Unit*> tmpMap;

	//for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	//{
	//	it->second->isReceived == cReceived;
	//	tmpMap[]
	//}
}


#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "Flower.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;


int UnitManager::mBoidsOnScreen;


using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{

}

Unit* UnitManager::createUnit(bool isFlower, const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	if (isFlower)
	{
		Flower *pFlower = new Flower(&sprite);

		//Byte* ptr = mPool.allocateObject();
		//if (ptr != NULL)
		//{
			//create unit
			//pFlower = new (ptr)Unit(sprite);//placement new

			UnitID theID = id;
			if (theID == INVALID_UNIT_ID)
			{
				theID = msNextUnitID;
				msNextUnitID++;
			}

			mUnitMap[theID] = pFlower;

			//assign id and increment nextID counter
			pFlower->mID = theID;

			//create some components
			ComponentManager* pComponentManager = gpGame->getComponentManager();
			pFlower->mPositionComponentID = pComponentManager->allocatePositionComponent(posData, shouldWrap);
			pFlower->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pFlower->mPositionComponentID, physicsData);
			pFlower->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pFlower->mPhysicsComponentID);

			//set max's
			pFlower->mMaxSpeed = MAX_SPEED;
			pFlower->mMaxAcc = MAX_ACC;
			pFlower->mMaxRotAcc = MAX_ROT_ACC;
			pFlower->mMaxRotVel = MAX_ROT_VEL;
		//}

		return pFlower;
	}
	else
	{
		Unit* pUnit = NULL;

		//Byte* ptr = mPool.allocateObject();
		//if (ptr != NULL)
		//{
			//create unit
			pUnit = new Unit(sprite);//placement new

			UnitID theID = id;
			if (theID == INVALID_UNIT_ID)
			{
				theID = msNextUnitID;
				msNextUnitID++;
			}

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
		//}

		return pUnit;
	}	
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


		//msNextUnitID--; //only works if we're taking from the end of the map

						//call destructor
		delete pUnit;
		pUnit = nullptr;
		return;

		//free the object in the pool
		/*mPool.freeObject((Byte*)pUnit);
		pUnit->~Unit();*/
	}
} //crashes here sometimes, probably flower related

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

void UnitManager::deleteAllUnits()
{
	bool shouldRestartLoop = false;
	for (auto it = mUnitMap.rbegin(); it != mUnitMap.rend(); ++it)
	{
		if (shouldRestartLoop)
		{
			it = mUnitMap.rbegin(); //it will delete the first one in the map every time
		}
		shouldRestartLoop = false;

		if (it->first == PLAYER_UNIT_ID)
		{
			continue;
		}
		deleteUnit(it->second->getID());
		shouldRestartLoop = true;
	}

	//mUnitMap.clear();
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.rbegin(); it != mUnitMap.rend(); ++it)
	{
		it->second->draw();
	}

	for (auto it = mReceivedUnits.begin(); it != mReceivedUnits.end(); ++it)
	{
		if (it->second != NULL) it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	bool alreadyUpdatedPlayer = false;
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		if (it->first == PLAYER_UNIT_ID && !alreadyUpdatedPlayer)
		{
			it->second->update(elapsedTime);
			alreadyUpdatedPlayer = true;
		}
		else
		{
			if (dynamic_cast<Flower*>(it->second)->update()) //I fucked up the polymorphism so I have to dynamic cast oops, should've just lined up the params correctly
				it = mUnitMap.begin(); //we're deleting DURING this loop. In the future, mark it for delete and clean it up after -- this was my oversight -rob
		}
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


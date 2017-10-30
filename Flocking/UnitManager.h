#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"

class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;


class UnitManager : public Trackable
{
	friend class SeparationSteering;
	friend class CohesionSteering;
	friend class GroupAlignmentSteering;

public:
	UnitManager(Uint32 maxSize);
	~UnitManager() {};

	Unit* createUnit(bool isFlower, const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Unit* createRandomUnit(const Sprite& sprite);


	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();
	void deleteIfShouldBeDeleted();
	void deleteAllUnits();

	void drawAll() const;
	void updateAll(float elapsedTime);
	void updateAll(bool shouldDelete);
	void cleanupBoids();

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	static int mBoidsOnScreen;

	//Unit** getUnitArray();

	std::map<UnitID, Unit*> getReceivedUnits() { return mReceivedUnits; }
	std::map<UnitID, Unit*> getLocalUnits(bool cReceived = true);


private:
	static UnitID msNextUnitID;
	MemoryPool mPool;
	std::map<UnitID, Unit*> mUnitMap;
	std::map<UnitID, Unit*> mFlowers;
	std::map<UnitID, Unit*> mReceivedUnits;
	//Unit* mReceivedUnits[15]; //15 max boids for each player
};


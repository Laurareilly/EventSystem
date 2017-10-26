/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 3- Flocking
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#include "Game.h"
#include "GameMessageManager.h"
#include "AddUnitMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "PositionComponent.h"
#include "GraphicsSystem.h"

AddUnitMessage::AddUnitMessage()
	:GameMessage(ADD_UNIT_MESSAGE)
{
}

AddUnitMessage::~AddUnitMessage()
{
}

void AddUnitMessage::process()
{
	UnitManager::mBoidsOnScreen++;
	Unit* pUnit = gpGame->getUnitManager()->createUnit(false, *gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID), true, PositionData(Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()), 0.0f));
	pUnit->getPositionComponent()->setFacing((rand() % (int)MAX_DEGREES) * 0.0174533);
	pUnit->setSteering(Steering::FLOCKING, ZERO_VECTOR2D, PLAYER_UNIT_ID);
}


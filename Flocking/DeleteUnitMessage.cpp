/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#include "Game.h"
#include "GameMessageManager.h"
#include "DeleteUnitMessage.h"
#include "UnitManager.h"

DeleteUnitMessage::DeleteUnitMessage()
	:GameMessage(DELETE_UNIT_MESSAGE)
{
}

DeleteUnitMessage::~DeleteUnitMessage()
{
}

void DeleteUnitMessage::process()
{
	//delete unit here
	UnitManager::mBoidsOnScreen--;
	gpGame->getUnitManager()->deleteRandomUnit();
	//gpGame->getUnitManager()->deleteUnit(UnitManager::mBoidsOnScreen);  //deleteRandomUnit();
}


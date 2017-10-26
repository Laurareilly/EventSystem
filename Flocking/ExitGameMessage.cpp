/*
Author: Laura Reilly
Class: EGP 410-02
Assignment: Assignment 2- Steering behaviors
Certification of Authenticity: I certify that this assignment is entirely my own work.
*/

#include "Game.h"
#include "GameMessageManager.h"
#include "ExitGameMessage.h"

ExitGameMessage::ExitGameMessage()
	:GameMessage(EXIT_GAME_MESSAGE)
{
}

ExitGameMessage::~ExitGameMessage()
{
}

void ExitGameMessage::process()
{
	//exit game
	gpGame->exitGame();
}


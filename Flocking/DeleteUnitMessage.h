#pragma once

#include "GameMessage.h"

class DeleteUnitMessage : public GameMessage
{
public:
	DeleteUnitMessage();
	~DeleteUnitMessage();

	void process();

};
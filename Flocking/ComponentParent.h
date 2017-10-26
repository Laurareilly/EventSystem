#pragma once

#include <Trackable.h>
#include "TransactionProcessor.h"

class Component;
class TransactionProcessor;

class ComponentParent : public Trackable
{
public:
	ComponentParent();
	virtual ~ComponentParent();

private:
	TransactionProcessor mProcessor;
};
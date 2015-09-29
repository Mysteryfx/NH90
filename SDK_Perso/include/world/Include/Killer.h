#pragma once

#include "world.h"

class WORLD_API Suicide
{
	friend class wTime;
	bool bAdjudged;
	void Adjudge(bool b = true);
public:
	Suicide();
	virtual ~Suicide(); 
	bool Adjudged();
};

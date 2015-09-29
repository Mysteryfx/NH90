#pragma once
#include "WorldManager.h"
#define REGISTER_DYNAMIC_FACTORY(FullName, ShortName) REGISTER_WORLD_FACTORY_EX(new WorldFactory<FullName>, ShortName);

#pragma once
#include "Transport.h"

struct lua_State;

namespace CarsCommands
{
	TRANSPORT_API void regLua(lua_State * L);
}
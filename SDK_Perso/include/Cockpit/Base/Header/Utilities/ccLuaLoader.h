#ifndef  _ccLuaLoader_h
#define  _ccLuaLoader_h

#include "CockpitBase.h"
#include <ed/string.h>

struct lua_State;

#ifndef WELL_DONE
#define WELL_DONE(error_code) ((error_code) == 0)
#endif

namespace cockpit
{

enum sharedStatesEnum
{
	SHARED_STATE_INDICATION_PAGES,
	SHARED_STATE_MISC,

	MAXIMUM_SHARED_STATES = 4
};

class COCKPITBASE_API ccLuaLoader
{
    ccLuaLoader();
   ~ccLuaLoader();

    static lua_State * sharedStates_[MAXIMUM_SHARED_STATES];
public:
	static	lua_State *	querySharedState(sharedStatesEnum id,bool * created = NULL);
	static		void	freeSharedStates();

	static      lua_State * open(const char* nameForLog = NULL, bool doNotBindOptions = false);
	static int              LuaDofile(lua_State*, const ed::string&,bool optional = false);//при успехе возвращает 0
    static void             bind_options(lua_State* L);

    static bool l_check_metatable(lua_State *Lua_s,const char * meta_name);
};

}

#endif //_ccLuaLoader_h

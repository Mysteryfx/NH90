#pragma once;
#include "ed_lua.h"
#include "CockpitBase.h"


#define DEF_S1( L ) #L
#define DEF_S2( M, L ) M(L)
#define DEF_S3 DEF_S2( DEF_S1, __LINE__ )
#define FILE_AND_LINE __FILE__ "_" DEF_S3 "_"
/*
#define DECLARE_L_REGISTER_FUNCTIONS() \
    virtual const char * l_metatable_name() const  { return FILE_AND_LINE; };\
    virtual void		 l_register_functions(lua_State *Lua_s,int index)
*/

//проверку наличие глобальной метаблицы убрал
//#define  _USE_NAMED_METATABLE_

namespace cockpit 
{
    class COCKPITBASE_API avLuaRegistrable
    { 
    protected:
        template <class T> 
        static  inline T*    l_get_device_from_meta(lua_State*	L_state)
        {
            lua_getfield(L_state,1,"link");
            T  *dev = static_cast<T*>((avLuaRegistrable*)lua_touserdata(L_state,-1));
	        lua_pop(L_state,1);
            return dev;
        };
      	void	             set_metatable(lua_State *Lua_s);
        virtual void		 l_register_functions(lua_State *Lua_s,int index) = 0;
#ifdef _USE_NAMED_METATABLE_
    private:
        virtual const char * l_metatable_name() const = 0;
#endif
    };
}
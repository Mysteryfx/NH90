#include "CockpitBase.h"
#include "wTime.h"
#include "ed_lua.h"

namespace cockpit
{
    // Деятельность Lua, реализуемая в скрипте сопрограммами
    class COCKPITBASE_API LuaCoroutine : public wTime
    {
    public:
        LuaCoroutine(lua_State * L_,
                     unsigned int index_);
        virtual ~LuaCoroutine();
            
        static void  register_in_script(lua_State *Ls);

        virtual void NextEvent();
    protected:
        static int   create(lua_State *Ls);
        static int   l_kill_me(lua_State *Ls);
        static int   l_start(lua_State *Ls);
        static int   l_get_model_time(lua_State *Ls);
        static void  l_set_metatable(lua_State *Ls);

        static LuaCoroutine * l_get_self(lua_State *Ls);


        unsigned int index;	// Индекс в таблице сопрограмм Lua
        lua_State    *L;
    private:

        static bool is_allive(LuaCoroutine * routine);
        static ed::set<LuaCoroutine*> & allive();
    };
}

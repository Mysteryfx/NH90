#pragma  once
#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "CockpitBase.h"

namespace cockpit
{
    class COCKPITBASE_API avLuaDevice : public avDevice,
										public avBreakable
    {
		int have_update;
		int have_animation_check;
    public:
        avLuaDevice():have_update(-1),have_animation_check(-1){}
       ~avLuaDevice(){}
        void release();
        void SetCommand(int command, float value = 0);
        void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
		void update();

		bool checkAnimationConditions(int arg_num, int command, float& value);
		void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name)
		{
			avDevice::initialize(ID,Name,script_name);
			avBreakable::load_from_state(ensured_Lua());
			avBreakable::register_as_breakable();
		}

		void repair();
		bool set_damage(unsigned Failure_ID, const bool can_be_rejected);
	protected:
		virtual void l_register_functions(lua_State *Lua_s,int index);
    private:
		void register_in_script(lua_State *Lua_s); 
		static int l_make_default_activity(lua_State *Lua_s);
		static int l_set_damage(lua_State *Lua_s);
		
		void set_timer(double dt);
     };
}
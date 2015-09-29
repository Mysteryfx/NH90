#pragma  once
#include "CockpitBase.h"
#include <ed/string.h>
#include <ed/map.h>

namespace Lua 
{ 
    class Config;
};

namespace cockpit
{ 
    struct Device_Mode;
    class COCKPITBASE_API avDeviceModeNames
    {
        bool initialized;
        ed::map<unsigned,ed::string> names;
    public:
        avDeviceModeNames()
        { 
            initialized = 0;
        }
        void         initialize(Lua::Config & config);
        const char * get_name(const Device_Mode & device_mode);
    };
}
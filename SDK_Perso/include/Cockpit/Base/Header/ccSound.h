#pragma once

// TODO: to delete eventually
#include "WorldSound.h"
#include "CockpitBase.h"

#include <ed/unordered_map.h>
#include <ed/vector.h>
//#include <ed/SounderAPI.h>

struct lua_State;

namespace cockpit {

typedef ed::unordered_map<ed::string, Sound::Host*> SoundHostsMap;

class COCKPITBASE_API ccSound
{ 
private:
     ccSound() {}
     ccSound(const ccSound&); 
public:
    static ccSound& cockpit::ccSound::instance()
    {
        static ccSound inst;
        return inst;
    };

	void loadSounds(lua_State* L, const ed::string& script); /*custom clickable elements sounds loading*/

    void init();
    void release();
    
	void createHost(Sound::ContextID context, const char* name, Sound::HostParams* params);
	Sound::Host& getHost(const char* name);

    Sound::Source clickButton;
    Sound::Source clickButtonOff;
    Sound::Source clickTumbler;

	bool getSoundPos(const char* connectorName, Sound::HostParams& h_params);

	void play_switch_sound(const Vector3& pos, int sound_id);

	static void register_in_script(lua_State * Lua_s);

	// new sound
	//void setSounder(ed::SounderRef sndrIn);
	//ed::SounderRef& getSounder();

private:

	void update_sw_host(const Vector3& pos);

	SoundHostsMap hosts;

	Sound::Host nullHost;

	struct SoundInfo
	{
		ed::string path_;
		Sound::Source sound_;
	};

	ed::vector<SoundInfo*> sounds_;

	// new sound
	//ed::SounderRef sounder;
};

}

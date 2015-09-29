#pragma once
#include "WorldSound.h"

namespace Sound {

// if modeltime sets pitch and timestamp fields
WORLDGENERAL_API void set_listener(ContextID context, /*non-const!*/SND_ListenerParams *params);

// system api
WORLDGENERAL_API void init();
WORLDGENERAL_API void init_userdirs();
WORLDGENERAL_API void add_sounddir(const char* soundDir);
WORLDGENERAL_API void process();
WORLDGENERAL_API void enter_interface(bool gui_mode);
WORLDGENERAL_API void enter_simulation();
WORLDGENERAL_API void reset_simulation();
WORLDGENERAL_API void update_listeners();
WORLDGENERAL_API void exit();

WORLDGENERAL_API void set_mute(bool mute);
WORLDGENERAL_API bool get_mute();

WORLDGENERAL_API float gain_to_dB(float gain);
WORLDGENERAL_API float dB_to_gain(float dB);

} // namespace Sound

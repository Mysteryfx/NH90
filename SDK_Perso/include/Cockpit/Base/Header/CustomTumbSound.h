#ifndef CUSTOM_TUMB_SOUND_H
#define CUSTOM_TUMB_SOUND_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "WorldSound.h"

namespace cockpit {

class avDevice;

class COCKPITBASE_API CustomTumbSound
{
public:
	~CustomTumbSound();

	void init(avDevice& parent, int command, const char* sound_path);
	void play_once();

private:
	Sound::Source sound_;
	Vector3 position_;
};

#define PLAY_CYCLED_ROTARY_CW_CCW(Cur_v,Next_v,Min_,Max_,Sw_cw,Sw_ccw)\
	if((Cur_v == Min_) && (Next_v == Max_))Sw_ccw.play_once(); \
	else if((Cur_v == Max_) && (Next_v == Min_))Sw_cw.play_once();\
	else (Cur_v < Next_v) ? Sw_cw.play_once() : Sw_ccw.play_once(); 

#define PLAY_SWITCH(open,Sw_open,Sw_close)\
	(open) ? Sw_open.play_once() : Sw_close.play_once(); 

}
#endif
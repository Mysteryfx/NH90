#pragma once

#include "CockpitBase.h"
#include "Avionics\avDevice.h"

namespace cockpit
{

class OSB_status
{
public:
	OSB_status() : pressed(false), locked(false),
		last_time(0.0), pressed_time(0.0) {}

	bool check()
	{
		pressed = !pressed;
		if (pressed)
		{
			pressed_time = 0; 
		}
		else
			pressed_time = wTime::GetModelTime() - last_time; 

		last_time = wTime::GetModelTime();
		return !locked;
	};
	void    lock()	 {locked = true;}
	void    unlock() {locked = false;}
	
	bool    locked, pressed;
	wModelTime last_time, pressed_time;
};

class avIPageHandler;

typedef ed::map<unsigned int, avIPageHandler*> page_handler_storage;

class avIPageHandler
{
public:
	avIPageHandler() : active(false) {}
	virtual ~avIPageHandler() {}
	virtual void on_activate() {active = true;}
	virtual bool on_command(int command, float value) = 0;
	virtual void on_deactivate() {active = false;}
	virtual void update() {}
	virtual void initialize(lua_State *L) {}
	virtual void release() {}
protected:
	bool      active;
};

template<class T> class avPageHandler : public avIPageHandler
{
public:
	avPageHandler() : avIPageHandler(), parent(0) {}
	avPageHandler(T* p) : avIPageHandler(), parent(p) {}

	T* parent;
};

class delayedAction
{
public:
	delayedAction() : Busy(false), Interruptable(true) {}

	bool getBusy() {return Busy;}


	bool Busy;
	bool Interruptable;
};

}

#pragma once

#include "AI/AI_Factory.h"

#include "wMovingObject.h"

namespace Lua {class Config;}

namespace gunner_AI {

class AI_Gunner;

//////////////////////////////////////////////////////////////////////////
class State
{
	friend class AI_Gunner;

public:

	virtual void l_read(Lua::Config& config) {}
	virtual bool interruptable() const { return false; }

protected:

	State() : host_obj_(nullptr){}
	virtual ~State(){}

	void set_host(AI_Gunner* host_obj) { host_obj_ = host_obj; }

	virtual void on_start() {}
	virtual void on_finish() {}

	virtual void simulate(double dt){}

	AI_Gunner* host_obj_;
};

typedef AIFactory<State>		AIStateFactory;
AIStateFactory::Fmap AIStateFactory::fabric;

#define REGISTER_STATE_AI_FACTORY(class_n) static AICreator<State,class_n,AIStateFactory> reg##class_n(#class_n);

}
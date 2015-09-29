#pragma once

#include "AI/AI_Factory.h"
#include "wMovingObject.h"

namespace Lua {
	class Config;
}

class IwoLA;

namespace gunner_AI {

class AI_Gunner;
class ISightAI;

class IGunHandler : public Mem::Shared
{
public:

	virtual ~IGunHandler(){}

	virtual void l_read(Lua::Config& config, IwoLA* la) = 0;
	virtual void on_start(AI_Gunner* host_){};
	virtual void on_end(AI_Gunner* host_){};
	virtual void fire(bool on) = 0;
	virtual float get_ammo_rest() const = 0;

protected:

	char pylon_;
	char gun_num_;
};

typedef Mem::Ptr<IGunHandler> IGunHandler_Ptr;

typedef AIFactory<IGunHandler>	IGunHandlerFactory;
IGunHandlerFactory::Fmap IGunHandlerFactory::fabric;

#define REGISTER_GUN_HAND_AI_FACTORY(class_n) static AICreator<IGunHandler,##class_n##,IGunHandlerFactory> reg##class_n(#class_n);

}
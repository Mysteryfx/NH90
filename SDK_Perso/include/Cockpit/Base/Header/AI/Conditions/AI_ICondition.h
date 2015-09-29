#pragma once

#include "AI/AI_Factory.h"
#include "AI/AI_Common.h"

namespace Lua { class Config; }

namespace gunner_AI
{
	class Condition
	{
	public: 
		virtual bool operator()(const AI_Gunner* host) const = 0;
		virtual bool l_read(Lua::Config& config){ return true; }
		virtual ~Condition(){}
	};

	typedef AIFactory<Condition>	AIConditionFactory;
	AIConditionFactory::Fmap AIConditionFactory::fabric;
	 
	#define REGISTER_COND_AI_FACTORY(class_n) static AICreator<Condition,##class_n##,AIConditionFactory> reg##class_n(#class_n);
}
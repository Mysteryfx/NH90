#pragma once
#include "CockpitBase.h"
#include "AI/wPlayerTaskIndicator.h"
namespace cockpit
{
	class ccCockpitContext;

	class COCKPITBASE_API avPlayerTaskHandler : public AI::wPlayerTaskIndicator
	{
		void raise_event(const char* name,void* content);
	public:
		virtual ~avPlayerTaskHandler(){};
		//all tasks will be redirected as cockpit event , you can catch them in onCockpitEvent (subscribe first)
		void setTask(AI::AttackGroup		*attackGroup)		{	raise_event("AI::AttackGroup"		,attackGroup);		}
		void setTask(AI::AttackUnit			*attackUnit)		{	raise_event("AI::AttackUnit"		,attackUnit);		}
		void setTask(AI::Bombing			*bombing)			{	raise_event("AI::Bombing"			,bombing);			}
		void setTask(AI::AttackMapObject	*attackMapObject)	{	raise_event("AI::AttackMapObject"	,attackMapObject);	}
		void setTask(AI::BombingRunway		*bombingRunway)		{	raise_event("AI::BombingRunway"		,bombingRunway);	}
		void setTask(AI::Orbit				*orbit)				{	raise_event("AI::Orbit"				,orbit);			}
		void setTask(AI::Land				*land)				{	raise_event("AI::Land"				,land);				}
		void setTask(AI::Refueling			*refueling)			{	raise_event("AI::Refueling"			,refueling);		}
		void setTask(AI::Follow				*follow)			{	raise_event("AI::Follow"			,follow);			}
		void setTask(AI::Escort				*escort)			{	raise_event("AI::Escort"			,escort);			}
		void setTask(AI::Task				*task)				{	raise_event("AI::Task"				,task);				}
		void resetTask(AI::Task				*task)				{	raise_event("AI::resetTask"			,task);				}
	};
}
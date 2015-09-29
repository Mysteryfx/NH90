#pragma once

#ifdef _ED_HIDDEN_DEBUG 
#define _AI_GUN_DEBUG_DRAW
#endif

#ifdef _AI_GUN_DEBUG_DRAW
#include "Renderer/DbgGraph.h"
#endif

#include "wMovingObject.h"
#include "AI/Detection/wTargetDetectionStatus.h"

#include "Utilities/ccPolarPosition.h"

using cockpit::PolarPosition;

namespace gunner_AI
{
	static const wModelTime TARGET_CHECK_TIME = 2.0; //s

	class AI_Gunner;

	enum AI_ROE
	{
		ROE_UNDEF = 0,
		ROE_HOLD,
		ROE_RET_FIRE,
		ROE_FREE_FIRE,
		ROE_PLAYER,
		ROE_DEAD
	};

	enum AI_Burst
	{
		BURST_UNDEF = 0,
		BURST_SHORT,
		BURST_LONG
	};

	#define INIT_STATE int(0)

	struct TargetCommonPriority
	{
		TargetCommonPriority(unsigned int cfg_priority)
			: visible_(false), active_(false), cfg_priority_(cfg_priority)
		{}

		bool			visible_;
		bool			active_;
		unsigned int	cfg_priority_;  //priority from config

		bool operator<(const TargetCommonPriority& prio)
		{
			if(active_ == prio.active_)
			{
				return cfg_priority_ > prio.cfg_priority_;
			}
			return active_ < prio.active_;
		}

		bool operator==(const TargetCommonPriority& prio)
		{
			return active_ == prio.active_
				&& cfg_priority_ == prio.cfg_priority_
				&& visible_ == prio.visible_;
		}
	};

	struct TargetState : public Mem::SharedLite
	{
		TargetState(woPointer ptr, TargetCommonPriority priority);

		bool assigned_;
		bool valid_;
		double assign_time_;

		float armor_;

		woPointer				ptr_;
		wTargetDetectionStatus	status_;
		PolarPosition			tgt_polar_pos_; //current target position related to gunner

		ed::set<AI_Gunner*> gunner_ptrs_;

		TargetCommonPriority priority_;

#ifdef _ED_HIDDEN_DEBUG 
		int ID_;
#endif

		bool operator==(const TargetState& t_state) const
		{
			return ptr_ == t_state.ptr_;
		}
	};

	typedef Mem::Ptr<TargetState> TargetStatePtr;

	class Condition;
	typedef ed::vector<Condition*> ConditionVector;

	struct ArgumentData
	{
		int num_;
		float value_;
		char  pylon_;
	};

	typedef ed::vector<ArgumentData> ArgsVector;

	static const float cmp_eps = float(1E-7);
	static const float UNDEF_VALUE = std::numeric_limits<float>::min();
}

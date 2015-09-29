#pragma once

#include "ed/map.h"

#include "CockpitBase.h"

#include "AI/AI_Common.h"
#include "AI/AI_Gun_Controller.h"

#include "Utilities/ccUtilities.h"

class IwoLA;

using cockpit::limits_f;

namespace gunner_AI
{

class State;
class Condition;
class IGunController;
class AIGunnersManager;

class COCKPITBASE_API AI_Gunner
{
	friend class CannonState;
	friend class GunController;
public:

	AI_Gunner(IwoLA* host);
	~AI_Gunner();

	void l_read(Lua::Config& config);
	
	void reload(bool disable /*true - reset do 'diabled' state; reset to 'enabled' state*/);
	
	void make_dead();

	bool disabled() const;
	bool loadout_present() const { return !not_present_; }
	void set_not_present();

	bool try_assign_target(TargetStatePtr& check_tgt, TargetStatePtr& current_tgt, bool by_priority);
	void assign_target(TargetStatePtr& t_state_new, double time);
	void update_time(double time);

	void update(double dt);

	TargetStatePtr get_target() const { return tgt_; }

	void clear_target();

	const ed::string& get_command() const { return cur_command_; }
	void set_command(ed::string& command)  { cur_command_ = command; }

	const AI_ROE get_ROE() const { return ROE_; }
	void set_ROE(AI_ROE ROE)  { if(!disabled_) ROE_ = ROE; }

	bool dead() const { return ROE_ == gunner_AI::ROE_DEAD; }

	const AI_Burst get_burst() const { return burst_; }
	void set_burst(AI_Burst burst)  { burst_ = burst; }

	float get_ammo_rest() const;
	float get_eff_dist() const;

	IwoLA* get_host() const { return la_; }
	
	void set_custom_handler(IGunHandler_Ptr handler);
	void reset_custom_handler();

	void set_sight(ISightAI* new_sight);

	ISightAI* swap_sights(ISightAI* new_sight);

	ISightAI* get_sight() const { return g_ctl_.sight_; }

	void set_manager_ptr(AIGunnersManager* manager_ptr);

	void set_skill(float skill) { Skill_ = skill; }
	float get_skill() const { return Skill_; }

	float get_aiming_T() const { return d_Tau_; }

#ifdef _AI_GUN_DEBUG_DRAW
	void external_parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);
#endif

private:

	void l_load_states(Lua::Config& config);
	void l_load_state_matrix(Lua::Config& config);

	inline PolarPosition target_polar_pos(woPointer tgt_) const;
	inline bool target_in_fov(woPointer tgt_) const;
	inline bool target_in_fov(const PolarPosition& tgt_pos) const;

	bool disabled_;
	bool not_present_;

	woPointer	host_;
	IwoLA*		la_;
	AIGunnersManager* gunner_manager_;

	cVector local_pos_;

	TargetStatePtr	tgt_;			//assigned target

	limits_f az_;
	limits_f el_;

	float Skill_; /*0....1*/
	float d_Tau_;				//aiming lag

#ifdef _AI_GUN_DEBUG_DRAW
	dbg_line*   dbgLineVisuals;
#endif

	float max_armor_w_;
	//////////////////////////////////////////////////////////////////////////
	//states
	typedef ed::vector<State*> States;

	typedef ed::vector<Condition*> Conditions;
	typedef ed::vector<Conditions> ConditionsVector;
	typedef ed::vector<ConditionsVector> ConditionsMatrix;

	ConditionsMatrix conditions_;
	States			 states_;

	int cur_state_;
	int wait_state_;

	AI_ROE ROE_;

	ed::string cur_command_;

	GunController	g_ctl_;
	float			ammo_rest_;	//ammo_rest in percents
	AI_Burst		burst_;
	//////////////////////////////////////////////////////////////////////////
};

}
#pragma once

#include "CockpitBase.h"

#include "wControl.h"

#include "AI/AI_Common.h"
#include "AI/AI_Gun_Handler.h"

#include "Randomizer/RandomObject.h"

class wDetector;
class ListTargetManager;
class IWing;
class IwoLA;

namespace Lua
{
	class Config;
}

namespace gunner_AI
{

class AI_Gunner;
class ISightAI;

class TargetsCfgPriorities
{
public:
	typedef ed::map<wsType, unsigned int> PriorityMap;

	TargetsCfgPriorities() :
		prior_counter_(0) {} 

	void l_read(Lua::Config& config);

	unsigned int get_priority(const wsType& type);

private:

	PriorityMap priority_;
	unsigned int prior_counter_;
};

class COCKPITBASE_API AIGunnersManager : public wControl /*not fully inherited; check code for full wControl functionality*/
{
public:

	AIGunnersManager(woPointer unit, IWing* wing);
	~AIGunnersManager();

	void l_read(Lua::Config& config, IwoLA* LA);

	void append_gunner(unsigned char gunner_ID, AI_Gunner* gunner_ptr);

	AI_Gunner* get_guner(unsigned char gunner_ID) const;

	void reload(unsigned char gunner_ID, bool disable);

	void set_gunner_handler(unsigned char gunner_ID, IGunHandler_Ptr handler);
	void reset_gunner_handler(unsigned char gunner_ID);

	bool gunner_disabled(unsigned char gunner_ID) const;
	
	void send_command(unsigned char gunner_ID, ed::string& command);

	void set_ROE(unsigned char gunner_ID, AI_ROE ROE);
	AI_ROE get_ROE(unsigned char gunner_ID) const;

	float get_ammo_rest(unsigned char gunner_ID) const; /*returns ammo rest in percents or -1*/
	
	void set_burst(unsigned char gunner_ID, AI_Burst burst);
	AI_Burst get_burst(unsigned char gunner_ID) const; /*returns burst duration*/

	void set_skill(float AI_skill /*0...1*/);

	void update(double dt);

	//wControl
	virtual void	onShoot(wcCoalitionName coalition, ObjectID shooterID, const wsType & weaponType, void * weaponData, ObjectID weaponID, ObjectID targetID, const cPoint & targetPoint, wModelTime impactTime);
	virtual void	onDamage(wcCoalitionName coalition, ObjectID shooterID, ObjectID weaponID, ObjectID targetID, float deltaLife);
	//wControl

	//wListener
	virtual	void	onDestroy(Registered *obj);
	//wListener

	void on_target_unassigned(TargetStatePtr tgt, AI_Gunner* gunner);

#ifdef _AI_GUN_DEBUG_DRAW
	void external_parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);
#endif

private:

	void target_check();
	bool is_target_valid(woPointer t_id) const; //check potential target

	void assign_targets();

	void target_valid();
	
	inline void unassign_target(TargetStatePtr t_state);
	inline void remove_target(TargetStatePtr t_state);
	inline void target_activated(TargetStatePtr t_state);


	void append_visible_target(TargetStatePtr t_state);
	void remove_visible_target(const TargetStatePtr t_state);

	wDetector* detector_;
	woPointer host_;

	const ListTargetManager* tgt_manager_;

	ed::map<ObjectID,TargetStatePtr> tgts_;

	typedef ed::unordered_map<unsigned char, AI_Gunner*> GunnersContainer;
	GunnersContainer gunners_;

	typedef ed::list<TargetStatePtr> ActualTargets;
	ActualTargets visible_targets_;

	TargetsCfgPriorities cfg_priority_;

	wModelTime last_check_time_;
	wModelTime last_assign_way_time_;

	bool extra_tgt_assign_;	

	bool as_by_priority_;
	static RandomObject rand_assign_; // < 0.5 by priority : >= 0.5 by type

};
}
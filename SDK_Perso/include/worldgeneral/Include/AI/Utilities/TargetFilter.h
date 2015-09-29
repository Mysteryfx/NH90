#ifndef __TargetFilter__
#define __TargetFilter__

//Target filter is used to filter targets which are valid for attack and select attack parameters

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "AI/AttackParam.h"

namespace AI
{

enum
{
	TARGET_HIGHEST_PRIORITY = 0,
	TARGET_LOWEST_PRIORITY = MAXINT
};

struct TargetFilterState
{
	enum AttackState { ATTACK_STATE_NO, ATTACK_STATE_TARGET, ATTACK_STATE_THREAT };
	AttackState attackState;
	wModelTime & time;
};

//Visitor that retrives target priority, taret attack parameters and threat flag
struct WORLDGENERAL_API TargetFilterVisitor
{
	TargetFilterVisitor();
	bool				result; //Is target valid
	unsigned int		priority; //Target priority, lower - higher
	AttackParam			attackParam; //Target Attack parameters
	bool				threatFlag; //Is target a threat
	struct WORLDGENERAL_API Threat //Threat parameters
	{
		Threat();
		bool				isAttacking; //Is threat attacking the object now
		wModelTime			timeToObjectKilledByThreat;
		wModelTime			timeToThreatKilledByMe;
		bool				objectIsInWEZOfThreat;
		woPointer			pProtectingObject; //The object to protect from the threat
	} threat;
};

//Target filter interface
class WORLDGENERAL_API TargetFilter
{
public:
	typedef TargetFilterVisitor Visitor;
	typedef TargetFilterState State;
	TargetFilter();
	TargetFilter(int priorityIn);
	virtual ~TargetFilter();
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const = 0;
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state, Visitor & visitor) const = 0;
	virtual bool	empty() const;
	virtual bool	isThreatFilter() const;
	virtual AttackParam::WeaponType getWeaponType() const = 0;
	int				priority;
};

//Target filter base implementation
class WORLDGENERAL_API TargetFilterBase : public TargetFilter
{
public:
	TargetFilterBase();
	TargetFilterBase(int priorityIn, const AttackParam & attackParamIn);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const = 0;
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state, Visitor & visitor) const;
	virtual AttackParam::WeaponType getWeaponType() const;
	AttackParam		attackParam;
};

}

#endif __TargetFilter__
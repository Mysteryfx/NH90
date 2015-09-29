#ifndef __TargetFilters__
#define __TargetFilters__

#include "AI/Utilities/TargetFilter.h"

#include "wControl.h"
#include <ed/list.h>

struct Waypoint;

namespace AI
{

//Returns false
class WORLDGENERAL_API NegativeFilter : public TargetFilterBase
{
public:
	NegativeFilter(int priority_, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
};

//Returns true
class WORLDGENERAL_API PositiveFilter : public TargetFilterBase
{
public:
	PositiveFilter(int priority_, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
};

//Filter for the given target
class WORLDGENERAL_API ObjectIDFilter : public TargetFilterBase
{
public:
	ObjectIDFilter();
	ObjectIDFilter(ObjectID id_, int priority_, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
private:
	ObjectID id;
};

//Filter for targets of the group
class WORLDGENERAL_API GroupFilter : public TargetFilterBase
{
public:
	GroupFilter();
	GroupFilter(wcPointer pGroup, int priority_, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
private:
	wcPointer pGroup;
};

//Filter for target which have the required attributes (wAttributeOwner)
class WORLDGENERAL_API TypeFilter : public TargetFilterBase
{
public:
	TypeFilter();
	typedef ed::list<ed::string> Types;
	TypeFilter(const Types & types_, int priority_, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
private:
	Types types;
};

//Filter for targets withing the distance from the route
class WORLDGENERAL_API RouteZoneFilter : public TargetFilterBase
{
public:
	RouteZoneFilter();
	RouteZoneFilter(float maxDistance_, ::Waypoint * route_, unsigned int routeSize_, int startWpt_, int priority, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
private:
	bool		isPointTooFarFromRoute_(const cPoint & point3D) const;
	float		maxDistance2;
	::Waypoint*	route;
	unsigned int routeSize;
	int			startWpt;
};

//Filter for targets inside the circle zone
class WORLDGENERAL_API CircleZoneFilter : public TargetFilterBase
{
public:
	CircleZoneFilter();
	CircleZoneFilter(const E2::Vector & point_, float radius_, int priority, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
private:
	E2::Vector	point;
	float		radius2;
};

//Filter for targets withing the circle zone around the given object
class WORLDGENERAL_API DynamicCircleZoneFilter : public TargetFilterBase
{
public:
	DynamicCircleZoneFilter();
	DynamicCircleZoneFilter(const woPointer & pUnit_, float radius_, int priority, const AttackParam & attackParam_);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
private:
	woPointer	pUnit;
	float		radius2;
};

struct CompareTargetFilterPriority
{
	inline bool operator()(const TargetFilter * left, const TargetFilter * right) const
	{
		if(left->isThreatFilter() == right->isThreatFilter())
			return left->priority < right->priority;
		else
			return left->isThreatFilter();
	}
};

//Filter with nested filters
class WORLDGENERAL_API ComboFilter: public TargetFilter
{
public:
	ComboFilter();
	void clear();
	virtual void add(TargetFilter * pTargetFilter);
	virtual void remove(TargetFilter * pTargetFilter);
	virtual bool isThreatFilter() const;
	virtual bool empty() const;
	AttackParam::WeaponType getWeaponType() const;
protected:
	bool isThreat;
	typedef ed::list<TargetFilter*> Filters;
	Filters filters;
	AttackParam::WeaponType weaponType;
};

//Operation OR. Attack parameters are take from first of the filters with highest priority
class WORLDGENERAL_API FilterOR : public ComboFilter
{
public:
	FilterOR();
	virtual void add(TargetFilter * pTargetFilter);
	virtual void remove(TargetFilter * pTargetFilter);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state, Visitor & visitor) const;
};

//Operation AND. Attack parameters and priority are minimized by all the filters
class WORLDGENERAL_API FilterAND : public ComboFilter
{
public:
	FilterAND();
	virtual void add(TargetFilter * pTargetFilter);
	virtual void remove(TargetFilter * pTargetFilter);
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state) const;
	virtual bool operator()(const woPointer & pTarget, const woPointer & pAttacker, State & state, Visitor & visitor) const;
};

}

#endif __TargetFilters__
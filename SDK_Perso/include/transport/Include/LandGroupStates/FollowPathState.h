#pragma once

#include "LandGroupStates/LandGroupState.h"
#include "CarsEnums.h"
#include <ed/vector.h>

class LeaderVehicle;
class wcRoute;
struct RoutePoint;
class MovingVehicle;

class FollowPathState: public LandGroupState
{
public:
	explicit FollowPathState(LandGroup *host);

	void update(double currentTime, double elapsedTime);
	LandGroupState *checkNextState() const;
	LandGroupStateType type() const {return gtFollowPath;}

	void onRemoveVehicle(const MovingVehicle* removed);
	void updateOnFormation();
	void cutPath(double endPar);

private:

	typedef std::pair<double, double> RPMove;
	typedef ed::vector<RPMove> MovesMap;
	MovesMap _leaderMoves;
	MovesMap::const_iterator _nextMove;

	typedef std::pair<const MovingVehicle*, double> ZeroDistCheck;
	typedef ed::list<ZeroDistCheck> ZeroDistChecks;
	ZeroDistChecks _zeroDistChecks;

	LeaderVehicle *_leader;
	/*const */wcRoute* _path;
	const RoutePoint *_nextCheck;

	double outstrech(const MovingVehicle& veh, double currentTime);
	double getPointSpeed(const RoutePoint* p) const;
	void initMoveMap();

	friend struct equalVeh;
	friend bool lessPar(const FollowPathState::RPMove&, const FollowPathState::RPMove&);
};

class FollowToLeaderState: public LandGroupState
{
public:
	explicit FollowToLeaderState(LandGroup *host);

	void update(double currentTime, double elapsedTime);
	LandGroupState *checkNextState() const;
	LandGroupStateType type() const {return gtFollowPath;}

	void onRemoveVehicle(const MovingVehicle* removed);
	void updateOnFormation();

private:

	LeaderVehicle *_leader;

	double outstrech(const MovingVehicle& veh, double currentTime);
};
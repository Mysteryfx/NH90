#pragma  once

#include <ed/vector.h>
#include <ed/string.h>
#include "TrailLink.h"
#include "Mem\Shared.h"
#include "GroupFormations/Formation.h"

class MovingVehicle;
class wcRoute;
class LeaderVehicle;
class LandGroupState;
struct RoutePoint;

class Igroup
{
public:
    virtual LandGroupStateType state() const = 0;
};

class /*TRANSPORT_API*/ LandGroup: public Igroup
{
public:
	typedef ed::vector<MovingVehicle*> MovingVehicles;

	LandGroup(wcRoute* path = 0);
	~LandGroup();

	void add(MovingVehicle* vehicle);
	void insert(MovingVehicle* vehicle, int index);
	void remove(MovingVehicle* vehicle);

	void reset();
	void setPath(wcRoute* route);
	void cutPath(double endPar);
	void clearPath();
	void savePath();
	void restorePath();
	void setNullPath();

	void disperse(double stopDelay);
	void fallInTemplate(const ed::string& templId);
	void stop();
	void wait(double time);
	void reform();
	
	virtual LandGroupStateType state() const;
	bool goingToDisperse() const;
	bool isLeaderOnRoad() const;
	
	//учитывает tacticalDir и tacticalSpeed 
	//возвращают максимальную установленную скорость
	double tacticalSpeed() const; 

	void update(double currentTime, double elapsedTime);

	double outstretchedPar() const
	{
		return _outstretchedPar;
	}

	const LeaderVehicle* leader()  const {return _leader;}

	VehicleLink getParentLink(int index) const;

	double speed() const {return _speed;}
	double maxPower() const {return _maxPower;}
	double breakingForce() const {return _breakingForce;}
	double turnR() const {return _turnR;}
	double radius() const {return _radius;}
	double acc() const {return _acc;}
	double width() const {return _width;}
	double mass() const {return _mass;}
	double maxSlopeAngle() const {return _maxSlopeAngle;}
	bool canForceRiver() const {return _canForceRiver;}

	double targetSpeed() const;
	void setTargetSpeed(double value);

	wcRoute *path() const {return _path;}
	const MovingVehicles& cars() const {return _cars;}
	const MovingVehicle* car(int index) const;

	osg::Vec2d formationOffset(int groupIndex) const;
	Mem::Ptr<Formation> currentFormation() const {return _currentFormation;}
	void setFormationInPath(Mem::Ptr<Formation> form, bool moveLeader, bool changeIfEqual, bool canUseDeferredChange);//смена формации в пути
	void setFormationOrder(Mem::Ptr<Formation> form, bool moveLeader, bool checkPathForm);//принудительная смена формации (по комманде пользователя)

	LandGroupState *getCurrentState( ) { return static_cast<LandGroupState*>(_currentState.get()); }
	void isLeaderHuman( bool bValue ) { _bLeaderHuman = bValue; }
	bool isLeaderHuman() { return _bLeaderHuman; }
	LandGroupState *setFollowingCurrentState();
private:
	void updateGroupParams(const MovingVehicle* car);
	void updateLeadersParams();
	void updateTrailsHistory();
	bool checkNearLeaderPos(const MovingVehicle* veh, osg::Vec2d& forationPos, osg::Vec2d& formationDir) const;
	void removeTrails();
	void tryDisperse();
	void setLeaderOnFirstVehicle();

private:
	MovingVehicles _cars;

	double _speed;
	double _maxPower;
	double _mass;
	double _acc;
	double _turnR;
	double _width;
	double _radius;
	double _breakingForce;
	double  _maxSlopeAngle; 
	bool   _canForceRiver;
	bool   _bLeaderHuman;

	osg::Vec2d _lastCheckDispersePos;
	bool _needDisperse;
	double _disperseStopDelay;

	Mem::Ptr<Formation> _currentFormation;
	Mem::Ptr<LandGroupState> _currentState;

	double _outstretchedPar;
	double _targetSpeed;

	LeaderVehicle *_leader;
	wcRoute* _path;
	wcRoute* _pathOld;

	friend class FollowToLeaderState;
	friend class FollowPathState;
	friend class ArriveState;
	friend class DisperseState;
	friend class StopState;
	friend class ArriveCustomFormState;
	friend class WaitState;
};
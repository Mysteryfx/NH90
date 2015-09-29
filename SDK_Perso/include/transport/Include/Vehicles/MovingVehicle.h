#pragma once

#include "TrailLink.h"
#include "TrailVehicle.h"
#include "Obstacle.h"
#include <memory>

class LandGroup;
class FormationState;

class MovingVehicle;
class Vehicles: public ed::vector<MovingVehicle*>{};

class /*TRANSPORT_API*/ MovingVehicle : public TrailVehicle
{
public:
	// constructor
	MovingVehicle();
	void reset();
	~MovingVehicle();

	void updateParentsLinks();
	void updateFormationState();
	
	void arriveVector(const osg::Vec2d& pos, const osg::Vec2d& dir);
	void arrivePosition(const osg::Vec2d& pos);
	void stop();
	void fastStop();
	void startAgony();
	void wander();

	double tacticalSpeed() const {return _tacticalSpeed;};
	double setTacticalSpeed(double speed) {return _tacticalSpeed = speed;}
	bool tacticalSpeedFixed() const;

	double setSurfaceRestrinctions(double maxSpeed) {return _surfaceSpeedRestriction = maxSpeed;};
	double surfaceSpeedRestriction() const {return _surfaceSpeedRestriction;};
	bool surfaceSpeedFixed() const;

	const osg::Vec2d& tacticalDir() const {return _tacticalDir;};
	const osg::Vec2d& setTacticalDir(const osg::Vec2d& dir);
	bool tacticalDirFixed() const;
	bool tacticalDirArrived() const;
	bool tacticalParamsFixed() const;


	double vertObstacle() const {return _vertObstacle;};
	double setVertObstacle(double height) {return _vertObstacle = height;}
	double maxSlopeAngle() const {return _maxSlopeAngle;}
	double setMaxSlopeAngle(double maxAngle) {return _maxSlopeAngle = maxAngle;}
	bool canForceRiver() const {return _canForceRiver;}
	bool setCanForceRiver(bool canForce) {return _canForceRiver = canForce;}

	const FormationState* formationState() const {return _currentFormationState;}
	void changeFormation();
	void deferredChangeFormation();
	void finishDeferredChangingForm();

	double desiredSpeed() const;
	// per frame simulation update
	void update (const double currentTime, const double elapsedTime);
	// apply a given steering force to our momentum,
	// adjusting our orientation to maintain velocity-alignment.
	void applyForce (const osg::Vec2d& force, double breakingForce, const double deltaTime);

	const VehicleLink& parentLink() const {return _parentLink;}
	VehicleLink& parentLink() {return _parentLink;}

	const LandGroup* group() const {return _group;}
	int groupIndex() const {return _groupIndex;}
	void setGroup(const LandGroup *group, int index)
	{
		_group = group;
		_groupIndex = index;
	}
	void setGroupIndex(int index){_groupIndex = index;}

	double pathParam() const;
	void idealFormationPos(osg::Vec2d& pos, osg::Vec2d& dir) const;

	double groupPathParam() const {return _groupPathParam;};
	void   resetGroupPathParam() {_groupPathParam = 0.;}

	osg::Vec2d predictCurvePosition(double dist) const;
	osg::Vec2d predictCurvePosition(double dist, const osg::Vec2d& offset) const;

	bool isOnRoad() const {return _isOnRoad;}
	void setOnRoad(bool onRoad);

	bool isObsAvoid() const {return _isObsAvoid;}
	bool needChangeFormation() const { return _needChangeFormation; }

	const ObstacleGroup& obstacles() const {return _obstacles;}
	const RideOnObstacles& rideOnObstacles() const {return _rideOnObs;}
	void updateObstacles();

	//Исключение убиенной машинки из списка препятствий
	void updateNeighbours(const MovingVehicle* removedVeh);
	//вызываеся при уничтожении мостов
	//по-идее должно вызываться также для площадок подскока, но их считаем неубиваемыми
	void updateRideOnObstacles(const viObjectShape* removedObs);

protected:
	bool AccumulateForce(const osg::Vec2d& additionalForce, osg::Vec2d& force);
	bool AccumulateBreaking(double additionalBreaking, double& breaking);
	
	void setFormationState(FormationState *state);
	bool checkedUpdateParentsList();

	bool setEndPath();
	bool isEndOfPath() const;
	void setObsAvoid(bool isAvoid) {_isObsAvoid = isAvoid;}

	void determineForce(osg::Vec2d &steeringForce, double &breakingForce);

private:
	FormationState* _currentFormationState;
	const LandGroup * _group;
	int _groupIndex;
	bool _needChangeFormation;
	bool _isOnRoad;
	bool _isObsAvoid;

	ObstacleGroup _obstacles;
	Vehicles _neighbors;
	RideOnObstacles _rideOnObs;

	osg::Vec2d _lastCheckObsPos, _lastCheckNeighbPos, 
		_lastChangeOnRoad, _lastChangeGroupPathParam;
	
	VehicleLink _parentLink;

	// максимальная скорость, выставляемая для стрельбы
	double _tacticalSpeed;
	// максимальная скорость возможная на данном рельефе
	//TODO всё это нужно делать не только на уровне скорости, 
	//должна меняться разгонная динамика, при езде по склону и т.п.
	double _surfaceSpeedRestriction;

	//Путевой параметр на пути группы, не точно соответствует текущей проекции на путь группы
	//обновляется раз в несколько метров
	double _groupPathParam;

	// направление, выставляемое для стрельбы, 
	osg::Vec2d _tacticalDir;

	// Размер вертикального барьера, м. Для препятствий которые машинка может не объезжать
	double _vertObstacle; 
	// Максимальный угол поверхности по которому может проехать машинка
	double  _maxSlopeAngle; 
	// Форсирование рек
	bool   _canForceRiver;
};
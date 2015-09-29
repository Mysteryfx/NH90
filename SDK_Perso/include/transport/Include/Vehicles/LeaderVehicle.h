#pragma once
#include "TrailVehicle.h"
#include "landscape3/lPathFile.h"

class wcRoute;
class LandGroup;

class LeaderVehicle: public TrailVehicle
{
public:
	// constructor
	LeaderVehicle(const LandGroup* group);
	LeaderVehicle();
	~LeaderVehicle();

	void reset();

	void update (const double currentTime, const double elapsedTime);

	virtual void move(double elapsedTime);
	virtual double desiredSpeed() const {return _desiredSpeed;}

	virtual bool isStopped() const {return speed()<0.1f;}
	virtual bool isEndOfPath() const;
	bool isOnRoad() const;

	const LandGroup* group() const {return _group;}
	virtual osg::Vec2d predictPosition(double predictionTime) const;

	void moveTo(double pathParam);
	double pathPar() const;
	const NM_ROAD::PointOnRoad& pathPoint() const {return path_point;};

private:
	void controlVelocity(double elapsedTime);
	double updateDesiredSpeed(double elapsedTime);
	double distToEnd() const;
	double maxOnPathSpeed(double pathPar) const;

	bool _finished;
	bool _inObstacle;
	osg::Vec2d _lastCheckObstacles;
	const LandGroup * _group;
	wcRoute* _path;
//	double v_for_turn;
	double _desiredSpeed;
	double _maxPossibleSpeed;
	double _rideFromObsDistance;
	NM_ROAD::PointOnRoad path_point; //точка, в которой находимся
};

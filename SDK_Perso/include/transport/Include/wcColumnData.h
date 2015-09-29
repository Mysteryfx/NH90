#pragma once

#include "CarsEnums.h"
#include "cLinear.h"
#include "Transport.h"
#include "wConst.h"
#include <osg/Vec2d>
#include "AStarSearch.h"
#include "Registry/Registered.h"

struct ColumnStatus 
{
	ColumnStatus()
		:formation(fNotDefined), routeSpeed(0.), state(gtNotDefined), firstCarPathPar(0.), 
		roe(ROE_OPEN_FIRE), groupDataChanged(true), alarmState(ALARM_STATE_AUTO)
	{}

	ColumnStatus(CarFormationType formation, double routeSpeed, 
		LandGroupStateType state, double firstCarPathPar, ROEType roe, GTMAlarmType alarmState, 
		bool groupDataChanged)
		:formation(formation), routeSpeed(routeSpeed), state(state), firstCarPathPar(firstCarPathPar), 
		alarmState(alarmState), roe(roe), groupDataChanged(groupDataChanged)
	{}

	double firstCarPathPar;
	bool groupDataChanged;
	CarFormationType formation;
	ROEType roe;
	GTMAlarmType alarmState;
	double routeSpeed;
	LandGroupStateType state;
};

struct TRANSPORT_API PathOptions
{
	struct ControlPoint
	{
		ControlPoint()
			:isOnRoad(false)
		{}

		ControlPoint(const osg::Vec2d& pos,	bool isOnRoad)
			:pos(pos), isOnRoad(isOnRoad)
		{}

		osg::Vec2d pos;
		bool isOnRoad;
	};
	typedef ed::vector<ControlPoint> ControlPoints;

	PathOptions()
		:formType(fNotDefined), speed(0.), requestLocalId(0)
	{}
	PathOptions(const ControlPoints& controlPoints, CarFormationType formType, 
		double speed, int requestLocalId);

	ControlPoints controlPoints;
	CarFormationType formType;
	double speed;
	int requestLocalId;
};

struct PathQueryParams
{
	ObjectID columnId;
	NavMeshData::SearchParams params;
	PathOptions::ControlPoints controlPoints;
	double groupWidth, speed;
	CarFormationType formType;
	int requestLocalId;
	bool isHumanColumn;
};

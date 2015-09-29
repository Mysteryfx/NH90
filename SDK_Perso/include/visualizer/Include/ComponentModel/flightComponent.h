#pragma once
#include "entity.h"
#include "splines/splineObject.h"

class TransformComponent;

//управляет позицией объекта по сплайну, имитируя полет.
//контролирует крен и тангаж с учетом угловой скорости сплайна
class FlightComponent: public EntityComponent
{
	Spline3d	spline;	
	bool		dirty;
	float		speed;
	float		distToTarget;
	float		lookAtOffset;//расстояние в у.е. по сплайну до точки, на которую будет смотреть камера
	float		flightDuration;//время полета при заданной скорости speed
	ed::vector<osg::Vec3f> knots;

public:

	TransformComponent *transform;

	FlightComponent();
	virtual bool init();
	//расстояние в метрах по сплайну до точки, на которую будет смотреть камера
	void	setDistToTraget(float dist);
	void	setSpeed(float spd);
	void	addKnot(const osg::Vec3f &p);
	const osg::Vec3f &getKnot(unsigned id);
	void	update(double curTime);
};


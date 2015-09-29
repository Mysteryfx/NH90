#pragma once
#include "entity.h"
#include "splines/splineObject.h"

class TransformComponent;

//��������� �������� ������� �� �������, �������� �����.
//������������ ���� � ������ � ������ ������� �������� �������
class FlightComponent: public EntityComponent
{
	Spline3d	spline;	
	bool		dirty;
	float		speed;
	float		distToTarget;
	float		lookAtOffset;//���������� � �.�. �� ������� �� �����, �� ������� ����� �������� ������
	float		flightDuration;//����� ������ ��� �������� �������� speed
	ed::vector<osg::Vec3f> knots;

public:

	TransformComponent *transform;

	FlightComponent();
	virtual bool init();
	//���������� � ������ �� ������� �� �����, �� ������� ����� �������� ������
	void	setDistToTraget(float dist);
	void	setSpeed(float spd);
	void	addKnot(const osg::Vec3f &p);
	const osg::Vec3f &getKnot(unsigned id);
	void	update(double curTime);
};


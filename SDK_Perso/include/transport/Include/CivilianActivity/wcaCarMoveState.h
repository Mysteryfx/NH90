#pragma once
#include "CivilianActivity/wcaCarState.h"
#include "Paths/lPath.h"

class wcaMirageActivity;

class wcaCarMoveState: public wcaCarState
{
public:
	wcaCarMoveState(wcaCar* host);
	virtual ~wcaCarMoveState();

	cPosition &Position(wModelTime t);

	float ScalarVelocity() const;
	const cVector &VectorVelocity() const;
	const float Omy();
	double GetPathPar(wModelTime t) const;

	IntersectionType TestVolume(ClipVolume *volume);
	void Parse(Graphics::ModelParser& parser, 
		const Position3& pos, Graphics::effectState* effects);

	void update();

protected:
	// получить свой путевой параметр
	virtual void GetPathPoint(double par, NM_ROAD::PointOnRoad& pt) const;
	virtual void GetPathPoint(double par, NM_ROAD::PointOnRoadH& pt) const;
	cPosition PositionByParam(double pathPar);

	// не пересчитываем позицию при дельте времени меньше этого
	wModelTime t_tolerance;
	mutable wModelTime t_pos;
	mutable NM_ROAD::PointOnRoadH path_point;

	wcaMirageActivity *_route;	// мирная деятельность
	double _initpar;		// начальный сегментный параметр на маршруте
};
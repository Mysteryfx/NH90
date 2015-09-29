#pragma once
#include "Inter.h"
#include "Math\Vector.h"

class IHelExtCargoSystem
{
public:
	virtual const bool					getNarrowShow()=0;
	virtual const bool					getCircleShow()=0;
	virtual const bool					getNarrowHeightShow()=0;
	virtual const bool					get_is_cargo_indicator()=0;
	virtual const double &				getCargoNarrow_x()=0;
	virtual const double &				getCargoNarrow_y()=0;
    virtual const double        &       getCargoHeightNarrow()=0;
	virtual const Math::Vec3d	 &		getCircle()=0;
	virtual const double &				getHel_pos_w_camera()=0;
};

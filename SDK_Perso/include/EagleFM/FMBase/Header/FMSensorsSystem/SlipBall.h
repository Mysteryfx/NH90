#pragma once

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class DynamicBody;

class FMBASE_API SlipBall
{
public:
	SlipBall(DynamicBody*);
	virtual ~SlipBall(){};
	void			simulate(double dt);
	double			getBallPos()	const { return BallPos; }

	Vec3			Pos_l;
	double			InitXRot; // кабина может быть наклонена относительно строительной горизонтали
	double			TubeLength;
	double			TubeSector;
	double			BallDampingFactor;
protected:

	virtual	Vec3    get_G_local();
	virtual Vec3    get_Speed_local();
	virtual Vec3    get_Acceleration_local();



private:
	DynamicBody		*pDBody;

	double			BallPos;
	double			BallSpeed; 
};


}
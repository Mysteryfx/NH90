#ifndef __Gravitation_h__
#define __Gravitation_h__

#include "Base.h"
#include "FMMathematics/math.h"

namespace EagleFM
{

class FMBASE_API Gravitation
{

public:
	Gravitation();

	// Вычисление ускорения свободного падения
	static Vec3 getGravitation_w(Vec3 Position_w);

};

}

#endif
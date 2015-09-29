#ifndef __WI_PATH__
#define __WI_PATH__

#include "ed/common_vector_map.h"
#include "cLinear.h"

//typedef ed::vector_map_interpolator<float, cPoint> wiPath;

class wiPath : public ed::vector_map_interpolator<float, cPoint>
{

};

#endif
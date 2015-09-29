#ifndef ED_GRAPH_MATH_UTILS
#define ED_GRAPH_MATH_UTILS

#include "MathExport.h"

class Vector3;
class Position3;

MATH_API bool CheckTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, 
				   const Vector3& a, const Vector3& b,
				   float &par, Vector3 *normal);

#endif

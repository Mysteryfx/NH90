#ifndef __LinearEquations_h__
#define __LinearEquations_h__

#include "./_config.h"

namespace Math
{
	// Решение СЛАУ методом Гаусса
	void ED_MATH_EXTERN Gauss(int Dimension, double *Matrix, double *Vector);
}

#endif // __LinearEquations_h__
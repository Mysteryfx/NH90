#ifndef _ED_Math_Equation_h_
#define _ED_Math_Equation_h_

namespace Math {

template<typename T>
unsigned int solveSquareEquation(T a, T b, T c, T (&x)[2])
{
	const T D = b * b - 4 * a * c;
	if(D < (T)0)
		return 0;
	else if(D == (T)0)
	{
		x[0] = -b / (2 * a);
		return 1;
	}
	else
	{
		x[0] = -b + sqrt(D) / (2 * a);
		x[1] = -b - sqrt(D) / (2 * a);
		return 2;
	}
}

template<class Func, class Deriv, typename T>
T solveNewton(	const Func & func, const Deriv & deriv,
				T x0, T epsilon, unsigned int iterMax)
{
	T x = x0;
	T dx;
	unsigned int i = 0;
	do
	{
		++i;
		dx = - func(x) / deriv(x);
		x += dx;
	}
	while(fabs(dx) > epsilon && i < iterMax);
	return x;
}

template<class Func, typename T>
T solveNewton(	const Func & func,
				T x0, T dxf,
				T epsilon, unsigned int iterMax)
{
	T x = x0;
	T dx;
	unsigned int i = 0;
	do
	{
		++i;
		const T y = func(x);
		const T d = (func(x + dxf) - y) / dxf;
		dx = - y / d;
		x += dx;
	}
	while(fabs(dx) > epsilon && i < iterMax);
	return x;
}

template<class Func, typename T>
T solveSecant(	const Func & func,
				T Xmin, T Xmax,
				T epsilon, unsigned int iterMax)
{
	unsigned int i = 0u;
	while(fabs(Xmax - Xmin) > epsilon && i < iterMax)
	{
		++i;
		const T yMax = func(Xmax);
		const T yMin = func(Xmin);
		Xmin = Xmax - (Xmax - Xmin) * yMax / (yMax - yMin);
		Xmax = Xmin - (Xmin - Xmax) * yMin / (yMin - yMax);
	}
	return Xmax;
}

template<class Func, typename T>
bool solveMedian(	const Func & func,
					T Xmin, T Xmax, T dx,
					T & x)
{
	T x1 = Xmin;
	T x2 = Xmin;
	T y1, y2;
	do
	{
		x1 = x2;
		x2 += dx;
		y1 = func(x1);
		y2 = func(x2);
	}
	while(y1 > 0.0f == y2 > 0.0f && x2 < Xmax);
	if(y1 > 0.0f != y2 > 0.0f)
	{
		x = x1 - y1 * dx / (y2 - y1);
		return true;
	}
	else
		return false;
}

}

#endif _ED_Math_Equation_h_
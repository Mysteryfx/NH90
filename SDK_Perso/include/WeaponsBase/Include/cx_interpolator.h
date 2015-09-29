#ifndef __CX_INTERPOLATOR_H__
#define __CX_INTERPOLATOR_H__

#include "WeaponsBase.h"

// Интерполяция Cx от числа Маха
class WEAPONSBASE_API CxInterpolator
{
public:
	CxInterpolator();
	CxInterpolator(const double* coeffs);
	CxInterpolator(double c0, double c1, double c2, double c3, double c4);
	~CxInterpolator();

	void init(const double* coeffs);

	double calcCx(double mach) const;
	const double* getRawCx() const { return c; }

private:
	double c[5];
	double p[5];
};

#endif

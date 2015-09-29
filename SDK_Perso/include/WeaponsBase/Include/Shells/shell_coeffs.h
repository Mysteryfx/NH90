#ifndef __SHELL_COEFFS_H__
#define __SHELL_COEFFS_H__

struct wShellCoefficients
{
	wShellCoefficients() : angleDeviation(1.0), initialVelocity(1.0), cxFactor(1.0)
	{
	}
	double angleDeviation;
	double initialVelocity;
	double cxFactor;
};

#endif
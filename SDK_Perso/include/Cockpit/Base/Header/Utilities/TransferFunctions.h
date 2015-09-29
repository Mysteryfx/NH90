#ifndef   _TRANSFERFUNCTIONS_H
#define   _TRANSFERFUNCTIONS_H

//#pragma once

#include "CockpitBase.h"

namespace cockpit
{

class COCKPITBASE_API TransferFunction
{
public:
	TransferFunction();
	virtual ~TransferFunction(){};

	void init(const double step_, const double K_, const double time_constant1_, const double time_constant2_);
	void reset();

	void setTimeConstant1(const double val){time_constant1 = val;};
	void setTimeConstant2(const double val){time_constant2 = val;};
	void setK(const double val){K = val;};

	virtual const double	update(const double dt, const double argument);
	const double	getFunctionValue()const{return function_value;};

protected:
	virtual const double GetA()const=0;
	virtual const double GetB()const=0;
	virtual const double GetGamma()const=0;

	double	step;
	double	function_value;
	double	prev_function_value;
	double	argument_value;
	double	prev_argument_value;
	double	time_constant1;
	double	time_constant2;
	double	K;
};

class COCKPITBASE_API TransferFunctionIntegral : public TransferFunction
{
public:
	TransferFunctionIntegral(){};

private:
	virtual const double GetA()const{return step/(2.0*time_constant1);};
	virtual const double GetB()const{return step/(2.0*time_constant1);};
	virtual const double GetGamma()const{return 1.0;};
};

class COCKPITBASE_API TransferFunctionIntegralLimited : public TransferFunctionIntegral
{
public:
	TransferFunctionIntegralLimited(){};

	virtual const double	update(const double dt, const double argument);

	void		setMinMaxValue(const double min_, const double max_);

private:
	double		min_value;
	double		max_value;
};

class COCKPITBASE_API TransferFunctionAperiodic : public TransferFunction
{
public:
	TransferFunctionAperiodic(){};

private:
	virtual const double GetA()const;
	virtual const double GetB()const;
	virtual const double GetGamma()const;
};

class COCKPITBASE_API TransferFunctionDifferential : public TransferFunction
{
public:
	TransferFunctionDifferential(){};

private:
	virtual const double GetA()const{return time_constant1/step;};
	virtual const double GetB()const{return -time_constant1/step;};
	virtual const double GetGamma()const{return 0.0;};
};

class COCKPITBASE_API TransferFunctionRealDifferential : public TransferFunction
{
public:
	TransferFunctionRealDifferential(){};

private:
	virtual const double GetA()const;
	virtual const double GetB()const;
	virtual const double GetGamma()const;
};

class COCKPITBASE_API TransferFunctionForce : public TransferFunction
{
public:
	TransferFunctionForce(){};

private:
	virtual const double GetA()const;
	virtual const double GetB()const;
	virtual const double GetGamma()const{return 0.0;};
};

}

#endif // _TRANSFERFUNCTIONS_H

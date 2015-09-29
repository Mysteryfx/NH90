#pragma once
#include "CockpitBase.h"

#define USE_LOCK_ON_RANDOMATOR

#ifdef USE_LOCK_ON_RANDOMATOR
class RandomObject;
#endif

class COCKPITBASE_API GaussProcess
{
public:
	GaussProcess();
	GaussProcess(double sx_, double Tx_);
	void clear();
	void start();
	void set_Tx(double Tx_);
	void set_sx(double sx_);	
	double process(double dt);
	double get_Tx() const {return Tx;}
	double get_sx() const {return sx;}
	double get() const {return ex;}
#ifndef USE_LOCK_ON_RANDOMATOR
	static double gauss();
#else
	static double gauss();
	static RandomObject * randomator;
#endif
private:
	double Tx;
	double ex;
	double sx;
};
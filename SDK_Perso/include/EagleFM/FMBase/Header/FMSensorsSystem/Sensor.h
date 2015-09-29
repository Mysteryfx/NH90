#pragma once

#include "Base.h"
#include "FMMathematics/math.h"
#include <algorithm>

namespace EagleFM
{


enum SensorModelingType
{
	SMT_Direct = 0,
	SMT_Aperiodic,
	SMT_Oscillation
};

class FMBASE_API Sensor
{
public:
	Sensor();
	virtual ~Sensor() {}

	static void			setFlagImmortal(bool Flag) { FlagImmortal = Flag; }

	void				init(double InMin, double InMax, double OutMin, double OutMax);

	void				setDirectType();
	void				setAperiodicType(double TimeFactor = 0.1);
	void				setOscillationType(double SignalFactor = 100.0, double SpeedFactor = 10.0);

	void				setIF(double _IF) { IF = __max(0.0, __min(_IF, 1.0)); }

	void				setInput(double _Input);

	virtual void		simulate(double dt);

	double				getOutput() const { return Output; }

	double				getIF() const { return IF; }

private:
	double				Input;
	double				Output;

	double				IF;

	double				Speed;			// Скорость изменения выходного сигнала

	//		TTX
	double				InputMin;
	double				InputMax;
	double				OutputMin;
	double				OutputMax;
	int					Type;
	double				TimeFactor;		// Постоянная времени (для апериодического типа)
	double				SignalFactor;	// Коэффициент усиления (для колеб.)
	double				SpeedFactor;	// Коэффициент демпфирования (для колеб.)

	static bool			FlagImmortal;
};


}
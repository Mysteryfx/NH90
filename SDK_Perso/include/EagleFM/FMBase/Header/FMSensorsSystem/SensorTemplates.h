#pragma once

#include "Sensor.h"


namespace EagleFM
{

class FMBASE_API Tachometer : public Sensor
{
public:
	Tachometer();
};

class FMBASE_API Thermometer : public Sensor
{
public:
	Thermometer();
};

class FMBASE_API Manometer : public Sensor
{
public:
	Manometer();
};

class FMBASE_API Speedometer : public Sensor
{
public:
	Speedometer();
};

class FMBASE_API Machometer : public Sensor
{
public:
	Machometer();
};

class FMBASE_API Variometer : public Sensor
{
public:
	Variometer();
};

class FMBASE_API Altimeter : public Sensor
{
public:
	Altimeter();
};


class FMBASE_API Volumeter : public Sensor
{
public:
	Volumeter();
};

class FMBASE_API Accelerometer : public Sensor
{
public:
	Accelerometer();
};

class FMBASE_API AngularRatemeter : public Sensor
{
public:
	AngularRatemeter();
};

class FMBASE_API LiquidFlowmeter : public Sensor
{
public:
	LiquidFlowmeter();
};

}

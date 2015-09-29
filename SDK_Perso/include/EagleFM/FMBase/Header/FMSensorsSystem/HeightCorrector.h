#ifndef HEIGHT_CORRECTOR_H
#define HEIGHT_CORRECTOR_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Base.h"
#include "FMSensorsSystem/SensorTemplates.h"
#include "FMSensorsSystem/BarometricAltimeter.h"
#include "FMSensorsSystem/IPito.h"

namespace EagleFM {

class DynamicBody;

class FMBASE_API AutopilotHeightCorrector
{
	class HCorrectorTube : public IPitotStatic
	{
	public:
		HCorrectorTube(const DynamicBody* const dBody);
		virtual double	getAtmoPressure() const;

	private:
		const DynamicBody* const dBody_;
	};

public:
	AutopilotHeightCorrector(const DynamicBody* const dBody);

	void	set_base();
	void	simulate();
	Sensor& get_sensor() { return sensor_; }

private:

	Sensor sensor_;
	BarometricAltimeter altimeter_;
	HCorrectorTube pressure_inp_;

	double set_H_;
	double current_H_;
};

}

#endif



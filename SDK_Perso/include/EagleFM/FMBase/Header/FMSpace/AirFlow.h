#pragma once

#include "FMMathematics/Constants.h"

namespace EagleFM {

	struct AirFlow //airflow properties
	{
		AirFlow() : Mass(0.0), Temperature(288.15), Pressure(PressureMCA_SL)
		{ }

		double Mass;
		double Temperature;
		double Pressure;
	};
}
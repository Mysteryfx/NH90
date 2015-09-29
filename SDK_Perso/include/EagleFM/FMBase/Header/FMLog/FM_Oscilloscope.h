#pragma once

#include <functional>
#include <ed/vector.h>

#include "Base.h"

#include "../../Cockpit/Base/Header/Indicators/IcChart.h"

class IcChart;

namespace Lua { class Config; }

namespace EagleFM {

class FMBASE_API FM_Oscilloscope
{
public:

	FM_Oscilloscope();

	void initialize(IcChart      * oscilloscope);
	void release();
	
	void registerLambda(const ed::string& param_name, const ChartParamFunc param_f);
	
	IcChart      * oscilloscope_;// только для отладочной отрисовки !!!!

	ed::vector<ed::string> registered_f_;
};

}
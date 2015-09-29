#pragma once

namespace EagleFM
{
	enum PistonEngineOutParam
	{
		EOP_SHAFT_RPM = 0,
		EOP_SHAFT_N,
		EOP_PROP_N,	//TEMP TODO: считать в винте по кольцам с ABS!!!!
        EOP_P2,
		EOP_T2,
		EOP_AMF,
		EOP_ALPHA,
		EOP_GAIN,
		EOP_IGN_STABLE,	//стабильность сгорания топлива
		EOP_STARTER_M,
		EOP_SHAFT_CYCLE,
		EOP_CARB_ALPHA,
		EOP_MANIFOLD_ALPHA,
		EOP_BLOWER_PWR,
		EOP_AF_PRESS,
		EOP_FUEL_FLOW,
		EOP_FUEL_REQ,
		EOP_MAX
		//EOP_T3,
		//EOP_T4,
		//EOP_FLAME,
		//EOP_PI_TGG,
		//EOP_NU_TGG,
	};
}
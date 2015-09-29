#ifndef FLUID_STATE_H
#define FLUID_STATE_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

namespace EagleFM
{
	struct FluidState
	{
		FluidState()
			: c_w(4.2), fluidFlow(1.0)
		{}

		double c_w;			//удельная теплоемкость жидкости в системе
		double fluidFlow;	//расход охл.жидкости
	};
}

#endif

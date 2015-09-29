#pragma once

namespace EagleFM
{ 
#ifndef _FINAL_VERSION
	struct LogOptions
	{
		LogOptions()
			: engine_hp_(false),
			start_alt_(0.0),
			th_pos_(-10.0),
			RPM_(0.0),
			Pres_hold_(0.0),
			BoostCooling_(0.0),
			Pres_max_(0.0)
		{}

		bool	engine_hp_;
		double	start_alt_;
		double	th_pos_;
		double	RPM_;
		double	Pres_hold_;
		double	BoostCooling_;
		double	Pres_max_;
	};
#endif
}
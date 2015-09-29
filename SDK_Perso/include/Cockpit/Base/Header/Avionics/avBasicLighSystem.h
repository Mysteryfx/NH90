#pragma once

#include "CockpitBase.h"
#include "Avionics/avDevice.h"

namespace cockpit {

	enum used_light_scheme_
	{ 
		LIGHT_SCHEME_BASIC,
		LIGHT_SCHEME_IMPROVED,
	};

	class COCKPITBASE_API avBasicLightSystem : public avDevice
	{
	public:
		unsigned      get_light_scheme() const { return light_scheme; }
		virtual		  ~avBasicLightSystem(){};

		virtual float get_summary_light_intensity() = 0;
		
		bool get_night() const;

	protected:
		avBasicLightSystem();
		//avDevice interface
		virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
		//end of interface
		
	protected:
		unsigned	light_scheme; // dependendant on graphics options
		wModelTime	night_start_time_;
		wModelTime	night_end_time_;
	};
}

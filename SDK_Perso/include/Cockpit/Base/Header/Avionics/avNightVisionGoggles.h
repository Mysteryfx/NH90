#pragma once

#include "avhelmet.h"

namespace cockpit
{
	class COCKPITBASE_API avNightVisionGoggles : public avHelmet
	{
	public:
		avNightVisionGoggles(void);
		~avNightVisionGoggles(void){};
		virtual void SetCommand(int command, float value = 0);
        virtual void setElecPower(bool p) { avBasicElectric::setElecPower(p); }
		virtual inline unsigned char helmet_type () const { return HELMETTYPE_NVG;}
		virtual void initialize(unsigned char id, const ed::string& name, const ed::string& script);

	};
};
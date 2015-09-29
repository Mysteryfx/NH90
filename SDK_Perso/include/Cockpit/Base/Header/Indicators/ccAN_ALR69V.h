#pragma once

#include "Indicators/ccIndicator.h"
#include "Avionics/RWR/avAN_ALR69V.h"
#include "Utilities/ccSimpleOctTree.h"
#include "Lua/Config.h"


namespace cockpit {

class COCKPITBASE_API ccAN_ALR69V : public ccIndicator
{
public:
	ccAN_ALR69V();
	virtual ~ccAN_ALR69V();
	
	virtual void initialize(avDevice*, unsigned char, const ed::string&);
	virtual void create_elements_controllers_map();
	virtual void control();

	static bool isBlink(float BlinkTime);

	static void ThreatPos(Element*, ccDrawable*, const DParamList&);
	static void ThreatSymbol(Element*, ccDrawable*, const DParamList&);
	static void AirborneSymbol(Element*, ccDrawable*, const DParamList&);
	static void PrimaryThreatSymbol(Element*, ccDrawable*, const DParamList&);
	static void NewGuySymbol(Element*, ccDrawable*, const DParamList&);
	static void LaunchSymbol(Element*, ccDrawable*, const DParamList&);
protected:
	static double PositionLimit; // limit for symbol indication on RWR

private:
	Device_Mode LastDevMode;
};

}

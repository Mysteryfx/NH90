#ifndef _avHUD_h
#define _avHUD_h

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "ICommandsCockpitDevice.h"
#include "cAlgorithm.h"

namespace cockpit
{
	
enum enum_HUD_commands
{		
	HUD_Command_Brightness = iCommandCockpitDevice_Action_1
};

class COCKPITBASE_API avHUD :   public avDevice,
								public avBreakable, 
								public avBasicElectric
{
public:
	avHUD();
	virtual ~avHUD();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual bool checkAnimationConditions(int arg_num, int command, float& value);
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer) {}
	//end of interface

	float get_brightness() const {return MAX(brightness - BrightnessToSubstract, 0.0f);}
	void  set_brightness(float b) {brightness = MIN_MAX(0.0f, b, 1.5f);}
	float getBrightnessToSubstract() {return BrightnessToSubstract;}
	void  setBrightnessToSubstract(float b) {BrightnessToSubstract = b;}
	void  change_brightness();

	bool getIsInitialized() {return Initialized;}
	void setIsInitialized(bool ValIn) {Initialized = ValIn;}

private:
	float brightness;
	float BrightnessToSubstract;

	bool Initialized;
};

}

#endif // _avHUD_h

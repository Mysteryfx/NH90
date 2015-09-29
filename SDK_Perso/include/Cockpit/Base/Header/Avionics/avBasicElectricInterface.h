#pragma once

#include "Avionics/avDevice.h"
#include "FMElectricSystems/CallBackSignal.h"
#include "WorldSound.h"

#include "FMElectricSystems/Items/ItemBase.h"

namespace cockpit {

class COCKPITBASE_API avBasicElectricInterface : public avDevice
{
public:
	avBasicElectricInterface() : AC_SourcesCount(0), AC_IsOn(false) {}
	virtual ~avBasicElectricInterface() {}
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void update();
	//end of interface

protected:
	void OnAC_BusPowerOn(const lwl::CSignalInfo* pInfo);
	void OnAC_BusPowerOff(const lwl::CSignalInfo* pInfo);

	void setPowerOnOffHandlersForAC_Bus(EagleFM::Elec::ItemBase* Bus);

	virtual void initSound();
	virtual Sound::Host* initSoundHost(); 

	int AC_SourcesCount;
	bool AC_IsOn;

    struct
	{
        Sound::Source AC_400Hz;
    } sound;

	ed::string ac_400HzSoundName;
};

}

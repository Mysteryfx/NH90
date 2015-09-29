#ifndef _AV_ELECTRICALLY_HELD_SWITCH_H
#define _AV_ELECTRICALLY_HELD_SWITCH_H

#include "CockpitBase.h"

#include "FMElectricSystems/Items/Wire.h"
#include "FMElectricSystems/Items/Switch.h"

namespace EagleFM { namespace Elec {

class Relay;

} }

namespace cockpit
{

class avDevice;

class COCKPITBASE_API avElectricallyHeldSwitch
{
public:
	avElectricallyHeldSwitch(avDevice* device, int CommPush, int CommRel);
	virtual ~avElectricallyHeldSwitch();

	bool getIsOn() const;
	void setOnOff(bool flag_in);

	void addDisconnectWire(EagleFM::Elec::ItemBase* WireIn);
		
	virtual void connect(EagleFM::Elec::ItemBase& Input) {Input >> Input_;}
	virtual EagleFM::Elec::ItemBase& getOutput() {return Switcher_;}
	virtual const EagleFM::Elec::ItemBase& getOutputConst() const {return Switcher_;}

	void checkPower();

protected:
	virtual void OnPowerOff(const lwl::CSignalInfo* pInfo);

	EagleFM::Elec::Wire Input_;
	EagleFM::Elec::Switch Switcher_;

private:
	int CommandPush;
	avDevice* Device;

	// Input_ >> ControlRelays[0] >> ControlRelays[1] >> .. >> Switcher_
	ed::vector<EagleFM::Elec::Relay*> ControlRelays;
	ed::vector<EagleFM::Elec::Wire*> AUX_Wires; //appended for a delegates removing on a cockpit reloading 
												//when the addDisconnectWire function gets a pointer to a system outside the cockpit
};

}

#endif // _AV_ELECTRICALLY_HELD_SWITCH_H

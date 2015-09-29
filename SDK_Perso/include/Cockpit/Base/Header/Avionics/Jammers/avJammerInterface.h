#pragma once

#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"
#include "LoadableEquipment\Jammers\eqJammer.h"

namespace cockpit
{
	
class COCKPITBASE_API avJammerInterface : public avDevice,
										  public avBreakable, 
										  public avBasicElectric
{
public:
	avJammerInterface(): IsActive(false) {}
	virtual ~avJammerInterface();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void release();
	virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
	//end of interface

	bool isAvailable() {return !Jammers.empty();} // returns true if ECM payload is available
	bool isJammerActive() const {return IsActive;}// returns true when jammer is working
	
	void startJammer();                       // try to turn on ECM
	void stopJammer();
	
protected:
	ed::list<eqJammer*> Jammers;

	void initialize_jammers();
	void release_jammers();
	void on_weapon_rearm();

private:

	bool IsActive;
	
};

}

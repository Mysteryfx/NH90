#pragma once

#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{

class COCKPITBASE_API avTransponder :   public avDevice,
										public avBreakable, 
										public avBasicElectric
{
public:
	avTransponder();
	virtual ~avTransponder();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	//end of interface

private:
};

}

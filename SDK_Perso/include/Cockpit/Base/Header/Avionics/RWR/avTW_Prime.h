#pragma once

#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{
	
class COCKPITBASE_API avTW_Prime :  public avDevice,
									public avBreakable, 
									public avBasicElectric
{
public:
	avTW_Prime() {}
	virtual ~avTW_Prime();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void release();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer) {}
	//end of interface
	
private:
	
};

}

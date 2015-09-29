#pragma once
#include "Avionics/avDevice.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{

class COCKPITBASE_API avMechanicClock : public avDevice, public avBreakable
{
public:
	avMechanicClock();
	virtual ~avMechanicClock();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void SetCommand(int, float value = 0){};
	virtual void serialize(Serializer &serializer){};
	//end of interface
	
	double get_currtime() const {return currtime;};
	
protected:
	
	wModelTime currtime, currtime_delta_time;
	bool clock_stopped;
	wModelTime constant_dbias;
};

}

#ifndef _avAHRS_h
#define _avAHRS_h
#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{

// Attitude and Heading Reference System
class COCKPITBASE_API avAHRS: public avDevice,
							  public avBreakable, 
							  public avBasicElectric
{
public:
	avAHRS();
	virtual ~avAHRS();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer){};
	//end of interface

private:

};

}

#endif // _avAHRS_h

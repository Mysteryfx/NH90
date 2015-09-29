#ifndef _avElectroMagneticDetector_h
#define _avElectroMagneticDetector_h

#include "avMagneticCompass.h"
#include "Avionics\avBasicElectric.h"

namespace cockpit
{

class COCKPITBASE_API avElectroMagneticDetector: public avMagneticCompass, public avBasicElectric
{
public:
	avElectroMagneticDetector();
	virtual ~avElectroMagneticDetector();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void release();
	virtual void SetCommand(int, float value = 0){};
	virtual void serialize(Serializer &serializer){};
	virtual void update();
	//end of interface

	float get_dbg_pitch() const {return dbg_pitch;};
	float get_dbg_bank() const {return dbg_bank;};

private:
	double max_pitch_bank;
	double dbg_pitch, dbg_bank;

};

}

#endif // _avElectroMagneticDetector_h

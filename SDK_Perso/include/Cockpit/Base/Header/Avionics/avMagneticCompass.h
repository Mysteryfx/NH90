#ifndef _avMagneticCompass_h
#define _avMagneticCompass_h

#include "Avionics\avDevice.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{

class COCKPITBASE_API avMagneticCompass : public avDevice, public avBreakable
{
public:
	avMagneticCompass();
	virtual ~avMagneticCompass();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	//end of interface

	float get_heading() const {return magnetic_heading_bframe;};
	
	float get_deviation() const {return deviation_iframe;};
	float get_mag_decl() const {return mag_decl_iframe;};
	float get_mag_incl() const {return mag_incl_iframe;};
	    
protected:
	dPosition device_pos_iframe;

	double magnetic_heading_bframe;
	double deviation_iframe, mag_decl_iframe, mag_incl_iframe;

	dVector get_magnetic_north(dPosition&, double&, double&, double&);
	dVector get_reverse_magnetic_north(dPosition&, double&, double&, double&);

private:
	
	dVector get_m_north(dPosition&, int, double&, double&, double&);

	Graphics::DParamList nodes;
	Graphics::DParamList deviation;
};

}

#endif // _avMagneticCompass_h

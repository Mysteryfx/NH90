#ifndef _avMechCompass_h
#define _avMechCompass_h

#include "avMagneticCompass.h"

namespace cockpit
{

class ccPanelGauge;

class COCKPITBASE_API avMechCompass : public avMagneticCompass
{
    ccPanelGauge *   pitch_arg_;
    ccPanelGauge *    bank_arg_;
    ccPanelGauge * heading_arg_;

public:
	avMechCompass();
	virtual ~avMechCompass();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
    virtual void post_initialize();
	virtual void SetCommand(int, float value = 0){};
	virtual void serialize(Serializer &serializer){};
	virtual void update();
    virtual void release();
	//end of interface

	float get_pitch() const {return magnetic_pitch_bframe;}
	float get_bank() const {return magnetic_bank_bframe;}
	float get_compass_card_heading() const {return compass_card_heading_bframe;}

private:
	double heading_threshold,
		max_pitch_bank,
		force_K,
		damper_K;

	double magnetic_pitch_bframe, magnetic_bank_bframe;
	double prev_magnetic_heading_bframe;
	float compass_card_heading_bframe;

	wModelTime prev_t; // для расчета dt
	double w;

	// для расчета полного ускорения
    dVector deviceLocalPos; // смещение прибора относительно центра модели
	dVector prev_p_iframe;
	dVector prev_V_iframe;
	dVector prev_acceleration_iframe;
	dVector prev_dadt;
};

}

#endif // _avMechCompass_h

#ifndef _avDNS_h
#define _avDNS_h

#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{

enum avDNS_modes
{
	DNS_OFF,			// выключена
	DNS_SELFTEST,		// самоконтроль
	DNS_STANDBY,		// не излучает
	DNS_OPERATE,		// рабочий режим
	DNS_MEMORY,			// выдача последних значений W и drift из памяти
};

// Doppler Navigation System (ДИСС)
class COCKPITBASE_API avDNS :   public avDevice,
								public avBreakable, 
								public avBasicElectric
{
public:
	avDNS();
	virtual ~avDNS();
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
    virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	//end of interface

	virtual bool getElecPower() const {return power;}
	
	const dVector& get_W_vec_ltp() const {return W_sframe;}
	const dVector& get_W_vec_bframe() const {return W_bframe;}
	double get_W_scalar() const {return W_scalar;}
	double get_rr() const {return rr;} // DBG
	double get_drift_angle() const {return drift_angle_bframe;}

	avDNS_modes get_mode() {return mode;}

	double get_d1() {return d1;}
	double get_d2() {return d2;}
	double get_d3() {return d3;}
	double get_d4() {return d4;}
	
    double d1, d2, d3, d4;

    double getTimeOfModeStart() const { return time_of_mode_start; }

protected:
	void zeroizeOutput();
	void ChangeMode(avDNS_modes ModeIn);

	avDNS_modes mode;
	
	wModelTime time_to_power_up,
		power_up_timer,
		time_to_wave_search,
		wave_search_timer;
	
	bool transmitter_ready, transmitter_transition, wave_found, wave_search;

	////////////////////////////////////////////
	// характеристики
	//
	double max_drift_angle, max_pitch_bank;

	////////////////////////////////////////////
	// выходные сигналы
	//
	dVector W_bframe; // путевая скорость (с.с.к.)
	dVector W_sframe; // путевая скорость (з.с.к. или с.с.к.)
	double W_scalar;
	double rr; // DBG
	double drift_angle_bframe;

protected:
	void	     l_register_functions(lua_State *Lua_s,int index);
	const char * l_metatable_name() const { return "dns_meta";}

	static int l_get_w_vector(lua_State * Lua_s);
	static int l_get_drift_angle(lua_State * Lua_s);

private:
    wModelTime time_of_mode_start;
	
	// для расчета dt
	wModelTime prev_t; 

	////////////////////////////////////////////
	// характеристики
	//
	float min_beam_travel, max_beam_travel;
	double beam_side_angle, beam_elev;
	double min_doppler_frequency, max_doppler_frequency;
	double wave_angleK;

	////////////////////////////////////////////
	// предвычисленные параметры
	//
	double l0;
	dVector e1_0, e2_0, e3_0;
	double _2_l0, _1_k1, _2_k2, _3_k3;
	
	dVector device_pos; // смещение прибора относительно центра модели
	
};

}

#endif // _avDNS_h

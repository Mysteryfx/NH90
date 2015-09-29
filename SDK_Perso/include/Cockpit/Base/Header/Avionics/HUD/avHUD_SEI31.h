#ifndef _avHUD_SEI31_h
#define _avHUD_SEI31_h

#include "avHUD.h"

#include "Avionics/avMechanics.h"

namespace cockpit
{

enum enum_HUD_SEI31_commands
{		
	HUD_SEI31_Command_Grid = iCommandCockpitDevice_Action_1 + 1,
	HUD_SEI31_Command_Test,
	HUD_SEI31_Command_Filter
};
enum enum_HUD_SEI31_mode
{		
	HUD_SEI31_DAY,
	HUD_SEI31_GRID,
	HUD_SEI31_NIGHT
};
class avSEI_Timer;

class COCKPITBASE_API avHUD_SEI31 : public avHUD
{
public:
	avHUD_SEI31();
	virtual ~avHUD_SEI31();

	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void SetCommand(int, float value = 0);
	virtual void update();

	void control();
	bool is_grid() { return indication_mode == HUD_SEI31_GRID;}

	int   get_indication_mode() const { return indication_mode; }
	
	float get_filter_stage() {return filterStage.get_value_out();}
	
protected:
	int  indication_mode;
    bool test;

	void powerOn();
	void powerOff();

private:
	wModelTime animation_timer_dt;
    avSEI_Timer* p_animation_timer;

	// светофильтр
	bool flag_filter;
	
	gauge_linear filterStage;

	double DeviceState, IndicatorState,
		DeviceDStateToOn, IndicatorDStateToOn,
		DeviceDStateToOff, IndicatorDStateToOff;

	double InitIndicatorState;
};

class avSEI_Timer : public avBasicTimer<avHUD_SEI31>
{
public:
    avSEI_Timer(avHUD_SEI31* _parent, wModelTime _dtime):avBasicTimer<avHUD_SEI31>(_parent,_dtime){}
	void NextEvent();
};

}

#endif // _avHUD_SEI31_h

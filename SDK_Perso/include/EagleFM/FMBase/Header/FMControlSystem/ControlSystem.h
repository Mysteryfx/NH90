#ifndef __ControlSystem_h__
#define __ControlSystem_h__

#include "Base.h"

#include "FMControlSystem/ControlStick.h"
#include "FMControlSystem/ControlRudder.h"
#include "PropertyTable.h"

enum ControlAxisDir
{
	CAD_NEUTRAL = 0,
	CAD_PITCH_UP,
	CAD_PITCH_DN,
	CAD_ROLL_LT,
	CAD_ROLL_RT,
	CAD_RUDDER_LT,
	CAD_RUDDER_RT,
};

namespace EagleFM
{

enum EngineNumber
{
	ALL_ENGINES = -1, /*unknown*/
	LEFT_ENGINE = 1,
	RIGNT_ENGINE = 2,
};

class FMBASE_API ControlSystem
{
public:
	ControlSystem();
	virtual ~ControlSystem() {}

	virtual void		setJoystickFFB(bool OnOff) { JoystickFFB_On = OnOff; }

	virtual void		setControlAxisDir(ControlAxisDir Axis, bool OnOff) {}//нажать/отжать перемещение контрола с указанием оси
	
	virtual void		setThrottle(double, int EngineNum = -1) {}
	virtual void		setThrottleDir(int Dir, int EngineNum = -1) {}  //задать движение РУД (-1 - уменьшение оборотов, 1 - увеличение, 0 - стоп)
	virtual void		increaseEngineRegime(int EngineNum = -1) {}
	virtual void		decreaseEngineRegime(int EngineNum = -1) {}
	virtual void		setEngineStop(bool, int EngineNum = -1) {}

	virtual void		setWheelBrake(bool, int WheelNum = -1) {}		//разовая команда вкл/выкл тормоз колес
	virtual void		setWheelBrake(double, int WheelNum = -1) {}		//ось 0..1 тормоза колес
	virtual void		setParkingBrake(bool, int WheelNum = -1) {}		//вкл/выкл стояночный тормоз колес

	virtual void		setTrimmer(bool OnOff)	{}						//нажать/отжать триммер
	virtual void		setTrimmer(ControlAxisDir Axis) {}				//нажать триммер с указанием оси
	virtual void		resetTrimmer() {}

	virtual void		setAutopilotMode(int Mode) {}					//учтановить режим автопилота
	virtual	void		setAutopilotParam(int Param, double Value) {}	//установить значение параметра выдерживания (стабилизации) автопилота


	//	доступ
	Stick*				getStick() { return &stick; }
	Pedals*				getPedals() { return &pedals; }
	const Stick*		getStick() const { return &stick; }
	const Pedals*		getPedals() const { return &pedals; }
	virtual double		getThrottlePos(int Num = -1) const { return 0.0; }
	virtual double		getWheelBrakePos(int WheelNum = -1) const { return 0.0; }
	virtual bool		getParkingBrake(int WheelNum = -1) const { return false; }
	virtual double		getAutopilotParam(int Param) const { return 0.0; }	

	void				getJoystickFFB_Neutral(float *Nx, float *Ny) { *Nx = JoystickFFB_NeutralX; *Ny = JoystickFFB_NeutralY; }
	void				getJoystickFFB_Factor(float *Fx, float *Fy) { *Fx = JoystickFFB_FactorX; *Fy = JoystickFFB_FactorY; }
	void				getJoystickFFB_ShakeX(float *Ax, float *Fx) { *Ax = JoystickFFB_AmplitudeX; *Fx = JoystickFFB_FrequencyX; }
	void				getJoystickFFB_ShakeY(float *Ay, float *Fy) { *Ay = JoystickFFB_AmplitudeY; *Fy = JoystickFFB_FrequencyY; }

	PropertyTable&		getDataTable() { return data_table_; }

protected:
	//Хозяин
	Stick				stick;
	Pedals				pedals;

	PropertyTable		data_table_; // table for data exchanging

	//Характеристики ForceFeedBack
	bool				JoystickFFB_On; //присутствует
	float				JoystickFFB_NeutralX;
	float				JoystickFFB_NeutralY;
	float				JoystickFFB_FactorX;
	float				JoystickFFB_FactorY;
	float				JoystickFFB_AmplitudeX;
	float				JoystickFFB_FrequencyX;
	float				JoystickFFB_AmplitudeY;
	float				JoystickFFB_FrequencyY;
};


}

#endif
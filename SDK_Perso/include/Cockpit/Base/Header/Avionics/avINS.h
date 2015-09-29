#ifndef _avINS_h
#define _avINS_h

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"

namespace cockpit
{

// Inertial Navigation System
class COCKPITBASE_API avINS:		public avDevice,
									public avBreakable, 
									public avBasicElectric
{
public:
	avINS();
	virtual ~avINS();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
    virtual void post_initialize();
	virtual void SetCommand(int, float value = 0);
	virtual void serialize(Serializer &serializer) {}
	virtual void update();
	//end of interface
	
	const dVector& get_total_acceleration() const {return total_acceleration_sframe;}
	//double getVerticalAcceleration() const {return (total_acceleration_sframe.y + gs) / gs;}
	
	const dPosition& get_pframe() const {return pframe;}
	double get_gyro_bank() const {return output_orient_pframe.x;}
	double get_gyro_pitch() const {return output_orient_pframe.y;}
	double get_gyro_heading() const {return output_orient_pframe.z;}

	bool getGyroOrientValid() const {return GyroBankValid && GyroPitchValid && GyroHeadingValid;}
	bool getGyroPitchBankValid() const {return GyroBankValid && GyroPitchValid;}
	bool getGyroBankValid() const {return GyroBankValid;}
	bool getGyroPitchValid() const {return GyroPitchValid;}
	bool getGyroHeadingValid() const {return GyroHeadingValid;}

	double get_platform_bank_error() const {return pframe_dynamic_bias.x;}
	double get_platform_pitch_error() const {return pframe_dynamic_bias.y;}
	double get_platform_rheading_error() const {return pframe_dynamic_bias.z;}

	void   getGyroOrient(dPosition & pos) const;
protected:
	void calculate_error_const();

	double err_const;

	void find_platform_angles(dPosition&, dVector&);
	void main_simulation(wModelTime);
		
	wModelTime prev_t; // для расчета dt
	bool first_step; // признак первого цикла
	// служебная, отладка
	bool model_accelerometers_error;
	
////////////////////////////////////////////
// гироскопы
//

	double gyro_bias_K;
	double angular_rate_error_K;
	double acceleration_error_K;

////////////////////////////////////////////
// курсовой гироскоп
//
	double course_gyro_accuracy_K;

	// ориентация гироскопа
	dPosition course_gframe;
	// часовой увод в радианах
	dVector course_gyro_bias_YZ;
    // широта
	// ???
	// магнитное склонение
	double magvar;

////////////////////////////////////////////
// вертикаль
//
	double attitude_gyro_accuracy_K;

	// ориентация гироскопа
	dPosition attitude_gframe;
	// часовой увод в радианах
	dVector attitude_gyro_bias_ZX;
	
////////////////////////////////////////////
// платформа
//
	// текущая ориентация платформы
	dPosition pframe;

	// характеристики следящих электродвигателей
	// максимальные угловые скорости
	dVector w_K;
	// максимальные угловые ускорения
	dVector w_dt_K;
	// предыдущие угловые скорости
	dVector prev_w;

	// коэффициенты усиления
	dVector gyro_platform_force_K;
	// коэффициенты демфирования
	dVector gyro_platform_damper_K;

	// вектор предыдущей ориентации гироплатформы (с учетом угловых скоростей и т.д.)
	// (bank, pitch, yaw) (рад.)
	dVector pframe_prev_angles;

	// сигнал рассогласования снимаемый с СКТ, предыдущее значение,
	// угловая скорость
	dVector pframe_dynamic_bias, pframe_prev_dynamic_bias, pframe_dynamic_bias_w;

	dPosition pframe_corrected;

////////////////////////////////////////////
// акселерометры
//
	// характеристики
	// максимальная перегрузка (g)
	double accelerometers_a_max;
	// статическая и динамическая характеристики
	double accelerometers_threshold, accelerometers_K_dyn;

	// служебные
	dVector prev_pos_true;
	dVector prev_V_iframe;

	// для расчета динамической ошибки
	// вектор ускорений на предыдущем шаге
	dVector prev_total_acceleration_sframe;

	// текущее ускорение с датчиков
	dVector total_acceleration_sframe;

////////////////////////////////////////////
// выходные сигналы
//
	// Сигналы крена, тангажа и курса (с гироплатформы)
	dVector output_orient_pframe;

	bool GyroBankValid;
	bool GyroPitchValid;
	bool GyroHeadingValid;

	//DBG
	//FILE* file;
};

}

#endif // _avINS_h

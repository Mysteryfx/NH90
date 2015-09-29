#ifndef _avRollPitchGyro_h
#define _avRollPitchGyro_h

#include "wTime.h"
#include "Avionics/Sensors/avBaseGyro.h"
#include "Utilities/MathDefs.h"
#include "Randomizer/RandomObject.h"
#include "Lua/Config.h"

namespace EagleFM
{
	class DynamicBody;
}

namespace cockpit
{

class COCKPITBASE_API avRollPitchGyro : public avBaseGyro
{
public:
	avRollPitchGyro(GyroPowerSource* power_source);
	virtual ~avRollPitchGyro();

	virtual void initialize(Lua::Config& config) override;
	virtual void simulate(double dt, bool NoDynamicUpd = false) override;
	virtual void cage(bool flag);
	virtual void RepairReset();

	bool getIsCage() const {return IsCage;}

	// мировой вектор направления гл. оси гироскопа
	const Vec3d& getGyroMainAxisIFrame() const {return GyroMainAxisIFrame;}
	// гл. ось гироскопа в с.с.к.
	const Vec3d& getGyroMainAxisBFrame() const {return GyroMainAxisBFrame;}

	void	turn_off_correction(bool off) { correction_off_ = off; }

protected:
	EagleFM::DynamicBody* pDBody;

	// Gyro cage is in progress
	bool IsCage;

	///////////////////////////////////////////////
	// Constants

	// Gyro cage rate
	double CageRate;
	
	// Масса гироскопа, кг
	double Mass;
	// Осевой момент инерции вокруг главной оси гироскопа, Н * м * с * с
	double Jz;
	// Осевые моменты инерции вокруг осей X, Y (оси Резаля), Н * м * с * с
	double Jxy;
	// Friction coefficients along X, Y axes
	double KfrX, KfrY;

	// Mass displacement constant scale along X gyro axis
	double DmassX_const_scale;
	// Mass displacement constant scale along Y gyro axis
	double DmassY_const_scale;
	// Mass displacement constant scale along X gyro axis
	double DmassZ_const_scale;

	// Mass displacement random scale along X gyro axis
	double DmassX_rnd_scale;
	// Mass displacement random scale along Y gyro axis
	double DmassY_rnd_scale;
	// Mass displacement random scale along X gyro axis
	double DmassZ_rnd_scale;

	// Масштабные коэфф. постоянных дрейфов рад/сек
	double ConstDriftYRate_Scale, ConstDriftXRate_Scale;
	// Угловые скорости постоянных дрейфов рад/сек
	double ConstDriftXRate, ConstDriftZRate;

	// Max deflection of gyro main axis from vertical
	double MaxDeflection;
	// Correction rate with gyro axis deflected on 8 deg from vertical
	double CorrectionRate;
	// Minimum correction rate
	double MinCorrectionRate;

	// Время корреляции случ. процессов угл. скоростей случ. дрейфов
	double T_Corr;
	// Среднеквадратическое откл. случ. процессов угл. скоростей случ. дрейфов
	double E;

	///////////////////////////////////////////////
	// Variables

	// мировой вектор направления гл. оси гироскопа
	Vec3d GyroMainAxisIFrame;
	// гл. ось гироскопа в с.с.к.
	Vec3d GyroMainAxisBFrame;

	// Угловые скорости случ. дрейфов
	double RndDriftX, RndDriftZ;

	// Угловые скорости вокруг осей X, Y (Резаля)
	double Wx, Wy;

	// Смещение массы вдоль осей X, Y, Z (оси Резаля) - метры
	double DmassX, DmassY, DmassZ;

	bool correction_off_;
};

}

#endif // _avRollPitchGyro_h
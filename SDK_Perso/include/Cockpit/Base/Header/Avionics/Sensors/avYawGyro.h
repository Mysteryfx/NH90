#ifndef _avYawGyro_h
#define _avYawGyro_h

#include "CockpitBase.h"

#include "Avionics/Sensors/avGyroPowerSource.h"
#include "Avionics/Sensors/avBaseGyro.h"

#include "Lua/Config.h"
#include "Utilities/MathDefs.h"
#include "Randomizer/RandomObject.h"

namespace EagleFM
{
	class DynamicBody;
}

namespace cockpit
{
	class COCKPITBASE_API avYawGyro : public avBaseGyro
	{
	public:
		avYawGyro(GyroPowerSource* power_source);
		virtual ~avYawGyro();

		double get_heading() const {return heading;}

		virtual void initialize(Lua::Config& config) override;
		void         simulate(double dt, bool NoDynamicUpd = false) override;
		virtual void cage(bool flag);

	protected:
		double heading;

		double PitchTransducer;

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

		// Постоянные времени эд. картушки и следящей рамы крена
		double T_Pitch, T_Bank;

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

		// Крен следящей рамы в с.с.к
		double TFrameBank;
		// Крен гироузла с датчика карданной рамы
		double GyroBankOGFrame;
		// Указатели тангажа и крена
		double IndicatorPitchDelta, IndicatorBank;

		// Угловые скорости вокруг осей X, Y (Резаля)
		double Wx, Wy;

		// Смещение массы вдоль осей X, Y, Z (оси Резаля) - метры
		double DmassX, DmassY, DmassZ;
	};
}

#endif // _avYawGyro_h
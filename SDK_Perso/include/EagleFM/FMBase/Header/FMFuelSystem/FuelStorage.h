#ifndef __FuelStorage_h__
#define __FuelStorage_h__

#include "FMFuelSystem/IFuelElement.h"

namespace EagleFM
{

class DynamicBody;
class FuelStorage;


//топливный бак, представляет собой большую топливную магистраль
//пересчитывает массу и моменты инерции тела.
class FMBASE_API FuelStorage : public IFuelElement
{
public:	
	FuelStorage(DynamicBody *pDB, Vec3 pos = Vec3(0.0), double V = 100.0) : 
		IFuelElement(V), pDBody(pDB), pos_l(pos), VolumeOld(0.0), 
		Ny(1.0), VolumeNyLimit(0.999999*V), VolumeNegativeMax(0.0), VolumePositive(0.0), DeliveryNegative(1.0) {}

	void			setVolumeNyLimit(double V) { VolumeNyLimit = __max(0.0, __min(V, OutputLine.getVolumeMax())); }
	void			setVolumeNegativeMax(double V) { VolumeNegativeMax = __max(0.0, __min(V, VolumeNyLimit)); }
	void			setDeliveryNegative(double D) { DeliveryNegative = __max(0.0, D); }

	virtual void	simulate(double dt);

	double			getVolume()				const { return OutputLine.getVolume() + VolumePositive; }

	Vec3			getPos()				const { return pos_l; }

	double			getNy()					const { return Ny; }
	double			getVolumePositive()		const { return VolumePositive; }
	
	//TODO: заправка, слив и сброс бака

private:
	DynamicBody		*pDBody; 
	Vec3			pos_l;
	double			VolumeOld;
	double			Ny;					//для сглаживания текущей перегрузки
	double			VolumeNyLimit;		//ниже этого значения при отрицательной перегрузке основное топливо не доступно
	double			VolumeNegativeMax;	//объем отсека для отрицательной перегрузки
	double			VolumePositive;		//кол-во топлива в недоступном при отр.перегрузке отсеке
	double			DeliveryNegative;	//скорость перекачки в отсек отр.перегрузки

};

//Модель повреждений
struct FMBASE_API storageLeakage
{
	storageLeakage()
	{
		speed         = 0;
		desired_speed = 0;
		time          = 0;
	}

	double speed;
	//время утечки , моделируем работу протектированного бака , при больших повреждения время на бесконечность
	double time;
	double desired_speed;
	void   repair()
	{
		speed         = 0;
		desired_speed = 0;
		time          = 0;
	}
	void   setInfinite(double speed_);
	void   setFinite(double speed_,double time_);

	void   simulate(FuelStorage & target,double dt);
};

class FMBASE_API FloatFuelSensor
{
public:
	FloatFuelSensor(FuelStorage& tank, const Vec3& center_shift, float tank_height, 
		double Freq0, double K, double dt); // center_shift - center of the sensor in tank coordinate system
								 // Freq0 - cutoff frequency
								 // K - filter gain

	~FloatFuelSensor();

	float update(Vec3 local_acceleration
#ifdef DEBUG
		, double dt
#endif
		);

private:
	FuelStorage&	tank_;

	Vec3			center_shift_;
	float			rel_tank_height_x_;
	float			rel_tank_height_z_;

	Math::IIR_Filter filter_X_;
	Math::IIR_Filter filter_Z_;

#ifdef DEBUG
	static int count;
	int my_count;
	double time_;
#endif

};
}

#endif

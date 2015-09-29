#ifndef __FuelPipeline_h__
#define __FuelPipeline_h__

#include "Base.h"
#include "FMMathematics/math.h"
#include <algorithm>

namespace EagleFM
{

//магистраль топливной системы
//служит для соединения элементов топливной системы,
//хранения информации и общения через нее элементов топливной системы
//Допущение: в жизни из магистрали можно взять только если она полностью заполнена,
//здесь можно брать пока есть.

class FMBASE_API FuelPipeline
{
public:
	FuelPipeline(double VolMax = 1.0) : Volume(0.0), VolumeMax(VolMax), P_in(false), P_out(false), Open(true) {}

	void	setVolume(double V) { Volume = __max(0.0, __min(V, VolumeMax));	if(Volume == 0.0)P_in = P_out = false; }
	void	setVolumeMax(double VolMax) { VolumeMax = __max(0.0, VolMax); }
	void	setPin(bool P) { if(Volume == 0.0 || !Open)P = false; P_in = P; }
	void	setPout(bool P) { if(Volume == 0.0 || !Open)P = false; P_out = P; }
	bool	getPin() { if(Volume == 0.0 || !Open)P_in = false; return P_in; }
	bool	getPout() { if(Volume == 0.0 || !Open)P_out = false; return P_out; }
	double	getVolume() const { return Volume; }
	double	getVolumeMax() const { return VolumeMax; }
	double	getVolumeReq() const { return Open ? VolumeMax - Volume : 0.0; }
	
	double	addVolume(double dV) //добавить топливо, возвращает излишек
	{
		if(!Open)
			return dV;
		double NewV = Volume + dV;
		setVolume(NewV);
		return NewV > VolumeMax ? NewV - VolumeMax : 0.0;
	}

	double	takeVolume(double dV) //взять топливо
	{
		if(!Open || dV <= 0.0)
			return 0.0;
		if(Volume >= dV)
		{
			setVolume(Volume - dV);
			return dV;
		}
		dV = Volume;
		setVolume(0.0);
		return dV;
	}

	bool	Open;		//можно закрыть магистраль (напр.перекрывным краном), при этом условимся что работает правило ни дать, ни взять

private:
	double	Volume;		//текущий объем топлива в магистрали (принято в [кг])
	double	VolumeMax;	//максимальный объем
	bool	P_in;		//давление на входе (true - нагнетание топлива)
	bool	P_out;		//давление на входе (true - забор топлива)
};

}

#endif
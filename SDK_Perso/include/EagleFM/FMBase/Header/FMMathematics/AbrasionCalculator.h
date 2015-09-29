//Вычислитель износа
//Заложена возможность задания до двух параметров, превышение которых определяет время износа
//по заданному полному времени до наступления окончательного отказа.

#ifndef __AbrasionCalculator_h__
#define __AbrasionCalculator_h__

#include "Base.h"

namespace EagleFM
{

class FMBASE_API AbrasionCalculator
{
public:
	AbrasionCalculator();
	virtual ~AbrasionCalculator() {}

	void	setTimeFull(double Time);
	void	initParameter1(double Limit, double K, double Pwr, int FuncType = 1);
	void	initParameter2(double Limit, double K, double Pwr, int FuncType = 1);
	
	double	calc(double Param1_Value); //для системы с одним параметром
	double	calc(double Param1_Value, double Param2_Value);

	double	getTimeFull() const { return TimeFull; }

private:
	double	TimeFull;

	double	Param1_Limit;
	double	Param1_K;
	double	Param1_Pwr;
	double	Param1_FuncType;

	double	Param2_Limit;
	double	Param2_K;
	double	Param2_Pwr;
	double	Param2_FuncType;
};

}

#endif
#ifndef __Spring_h__
#define __Spring_h__

#include "Base.h"

namespace EagleFM
{

//Модель пружины, сутью которой является расчёт силы реакции пружины 
//по величине её обжатия. Применяется при «сборке» амортизатора.
class FMBASE_API Spring
{

public:
	Spring();

	// Расчет силы и длины пружины
	inline double calcForceByLength();
	inline double calcLengthByForce(); //Under contruction
	inline double calcForceByLength(double Length);
	inline double calcLengthByForce(double Force); //Under contruction

	double MinLength;		// Минимальная длина пружины
	double MaxLength;		// Максимальная длина пружины
	double BasicLength;		// Длина пружины в свободном состоянии
	double ForceFactor;		// Коэффициент упругости пружины
	double ForceFactorRate;	// Степень

	double Length;			// Длина пружины
	double Force;			// Сила пружины
};

}

#endif
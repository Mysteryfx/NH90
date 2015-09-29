#ifndef __ControlSurface_h__
#define __ControlSurface_h__


#include "Base.h"

namespace EagleFM
{

//Класс, описывающий характеристики управляющей поверхности: углы отклонения и диапазоны

class FMBASE_API ControlSurface
{
public:
			// МЕТОДЫ

	ControlSurface();
	ControlSurface(double MinAngle, double MaxAngle);
	virtual ~ControlSurface() {}

	void	init(double MinAngle, double MaxAngle);

	// Методы доступа к углу отклонения управляющей поверхности
	void	setAngle(double Ang);
	void	setRelativeAngle(double Ang);		//отн.несимметричный угол (-1 ... 0 ... 1) => (MinAngle ... 0 ... MaxAngle)
	void	setRelLineAngle(double Ang);		//отн.симметричный угол (-1 ... 0 ... 1) => (MinAngle ... {Bias} ... MaxAngle)
	void	setIF(double);
	void	setADFactor(int Num, double Value) { if(Num > 0 && Num < 10) ADFactors[Num] = Value; }	// (0 - setAngle)
	
	double	getAngle()			const { return ADFactors[0]; }
	double	getRelativeAngle()	const;
	double	getRelLineAngle()	const;
	double	getBias()			const { return Bias; }
	double	getRelativeBias()	const { return K == 0 ? 0.0 : Bias/K; }
	double	getMinAngle()		const { return MinAngle; }
	double	getMaxAngle()		const { return MaxAngle; }
	double	getIF()				const { return IntegrityFactor; }

	
			// ПОЛЯ
			
protected:
	//параметры
	double	ADFactors[10];		// Коэффициенты влияния на аэродинамику управляющей поверхности. 0-й - угол отклонения!!!
	double	IntegrityFactor;	// Коэффициент повреждения управляющей поверхности

	//характеристики
	double	MinAngle;			// Минимальный угол отклонения управляющей поверхности
	double	MaxAngle;			// Максимальный угол отклонения управляющей поверхности

	//кэш
	double	Bias;
	double	K;


	friend class ADElement;
};

}

#endif
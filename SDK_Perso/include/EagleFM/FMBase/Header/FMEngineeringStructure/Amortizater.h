#ifndef __Amortizater_h__
#define __Amortizater_h__

#include "Base.h"
#include "FMEngineeringStructure/Spring.h"
#include "FMEngineeringStructure/Damper.h"


namespace EagleFM
{

struct FMBASE_API AmortizaterInit
{
	AmortizaterInit()
	{
		BasicLength = 0.0;
		MinLength = 0.0;
		MaxLength = 0.0;
		SpringForceFactor = 0.0;
		SpringForceFactorRate = 1.0;
		StaticForce = 0.0;
		ReduceLength = 0.0;
		DirectDamperForceFactor = 0.0;
		BackDamperForceFactor = 0.0;
		CalcStockSpeedFlag = true;
	}

	double	BasicLength;
	double	MinLength;
	double	MaxLength;
	double	SpringForceFactor;
	double	SpringForceFactorRate;
	double	StaticForce;
	double	ReduceLength;
	double	DirectDamperForceFactor;
	double	BackDamperForceFactor;
	bool	CalcStockSpeedFlag;

};


//Модель амортизатора, который включает в себя пружину (Spring) 
//и демпферы (Damper) прямого хода и обратного. 
//Применяется при «сборке» шасси и моделировании точек контакта с поверхностью
class FMBASE_API Amortizater
{
public:
	Amortizater();
	Amortizater(const AmortizaterInit&);

	void			init(const AmortizaterInit&);

	void			simulate(double dt);

	//		ДОСТУП

	double			setLength(double Length, bool Relative = false); //Relative = true - Length отн. BasicLength, возвращает излишек
	double			getLength()	const { return Spr.Length; } 

	double			getForce()	const	{ return Force; }

	//		---настройка (доступ к)---	
	void			setMinLength(double MinLength);// минимальной длине амортизатора	
	void			setMaxLength(double MaxLength);// максимальной длине амортизатора
	void			setBasicLength(double BasicLength);//к длине амортизатора в свободном состоянии
	
	void			setStockSpeed(double StockSpeed) { DirectDamper.Speed = BackDamper.Speed = StockSpeed; }


	double			getMaxLength() const { return MaxLength; } 
	double			getMinLength() const { return MinLength; } 


	AmortizaterInit	getAmortizaterProp() const; //для перенастройки конкретной характеристики

	//		ПОЛЯ
private:
	Spring	Spr;				// Пружина
	Damper	DirectDamper;		// Дэмпфер прямого хода
	Damper	BackDamper;			// Дэмпфер обратного хода

	double	StaticForce;		// Статическая сила реакции амортизатора
	double	ReduceLength;		// Cвободный (ненагруженный) ход штока амортизатора

	double	MinLength;			// Минимальная длина амортизатора
	double	MaxLength;			// Максимальная длина амортизатора

	double	LastLength;			// Длина амортизатора на предыдущем шаге моделирования

	double	Force;				// Сила реакции амортизатора

	bool	CalcStockSpeedFlag;	// Флаг управления способом вычисления скорости амортизатора
};

}

#endif
//проверка события по заданной вероятности на заданном интервале времени с заданной частотой проверки

#ifndef __ProbabilityChecker_h_
#define __ProbabilityChecker_h_

#include "Base.h"

#include "Optimizable.h"
#include "FMMathematics/Randomizer.h"

namespace EagleFM
{

//Аппарат для проверки события.
//getChecker() вернет true, если событие произошло.
//Внешняя (setEvent) установка служит для явного задания вероятности события на интервале времени;
//пример - установка из редактора. По вероятности однократно определяется произойдет ли событие, в случае успеха
//кубиком определяется момент времени на заданном интервале. При достижении этого момента результат выставляется в true.
//Внутренняя накопительная вероятность (addInternalProbabilityAndTime) задается в предусматриваемом клиентом случае;
//пример - повреждение системы. Эта вероятность будет действовать постоянно во время моделирования, не смотря на
//заданный интервал времени - он служит только для качественной оценки вероятности, например, если задать вероятность
//0.3 на интервале 1 мин, то не произошедшее в ближайшую минуту событие вероятнее всего произойдет при дальнейшем моделировании.
//Один из способов использования этого аппарата: на каждом шаге клиента аппарата сбрасывать вероятность (resetProbability),
//и выставлять посчитанные вероятности на посчитанных интервалах (addProbabilityAndTime) от разных факторов.

class FMBASE_API ProbabilityChecker : public Optimizable
{
public:
	ProbabilityChecker(double dt = 1.0); //dt не должно быть меньше, чем Т0/360, чтобы ГСЧ коррекно работал с порогом
	virtual ~ProbabilityChecker() {} 

	void			initRandomizer(uint32_t x) { Rnd.init(x); }
	
	void			setEvent(double Probability, double TimeStart, double TimeInterval); //внешняя установка события на заданном интервале времени с заданной вероятностью (определяет факт отказа и его время на заданном интервале)
	void			addProbabilityAndTime(double Probability, double TimeInterval);	//добавление вероятности события  на заданном интервале времени (проверяется постоянно за время моделирования)

	void			resetEvent()		{ Time = 0; TimeEvent = -1.0; CheckResult = false; Alpha = Limit = 0.0; }
	void			resetTime()			{ Time = 0; }
	void			resetCheckResult()	{ CheckResult = false; }
	void			resetProbability()	{ Alpha = Limit = 0.0; }

	bool			getCheckResult() const { return CheckResult; }

	double			getTime()			{return  Time;}

protected:
	virtual void	doSimulate();

	bool			CheckResult;	//результат проверки 

	double			Time;			//текущее время

	double			Limit;			//вычисленное значение порога для кубика по внутренней накопленной вероятности
	double			Alpha;			//альфа - независимое от частоты дискретизации полученное для заданной вероятности и интеравала времени значение 

	double			TimeEvent;		//Момент времени события по заданному внешнему условию

	Randomizer		Rnd;			//кубик
};

}



#endif

#ifndef __ModelTimeQuantizer_h__
#define __ModelTimeQuantizer_h__

#include "World.h"
// Квантователь модельного времени предназначен для нарезания интервалов
// модельного времени имитации в мире, в зависимости от хода реального
// времени, коэффициента ускорения модельного времени, принятой дисциплины
// управления частотой кадров и т.д. Объектам мира квантователь сам по себе
// не нужен, они должны использовать внутренний модельный таймер wTime (см. wTime.h).

struct lua_State;

typedef double	RealTime;
typedef double	ModelTime;

class WORLD_API ModelTimeQuantizer
{
	int			halted;				// Счётчик заморозки автоматического квантования модельного времени, если > 0 - время остановлено 
	ModelTime	zero;				// Модельное время инициализации
	ModelTime	start;				// Модельное время начала очередного шага имитации
	ModelTime	finish;				// Модельное время окончания очередного шага имитации
	RealTime	realZero;			// Реальное время инициализации
	RealTime	end;				// Накопленное непрерывными шагами реальное время (при заморозке не изменяется)
	RealTime	phase;				// Реальное время от end до заморозки
	RealTime	freezeTime;			// Реальное время заморозки
	RealTime	unfreezeTime;		// Реальное время разморозки
	double		acceleration;		// Коэффициент ускорения (acceleration >= 0.01)
	double		savedAcceleration;	// Сохраненное ускорение для отката
	bool		bFreezeNextTime;	// Признак заморозки после текущего шага
	RealTime	constStep;			// Постоянный шаг моделирования
    RealTime    antifreeze;         // Шаг считающийся лагом и пропускаемый.
public:
	ModelTimeQuantizer();
	~ModelTimeQuantizer();

	// Инициализация производится начальным модельным временем и признаком заморозки
	void		initialize(ModelTime startTime = 0.0f, bool status = true);
	void		initialize(RealTime realStart, ModelTime startTime, bool status);
	void		freeze();	// Заморозить автоматический расчет шага имитации
	void		unfreeze();	// Разморозить автоматический расчет шага имитации
	inline bool	freezed() {return halted > 0;}; 	// Квантователь заморожен?
	// Принудительное выполнение шага (работает и при замороженном квантователе)
	void		nextStep(ModelTime);
	// Автоматический расчет шага при размороженном квантователе
	void		nextStep();	
	ModelTime   getDeltaTime();
	ModelTime	getFinishTime();
	void		setFinishTime(ModelTime finishTime);
	ModelTime	getStartTime();
	RealTime	getRealStartTime();
	RealTime	getRealFinishTime();
	RealTime	getLateness();	// Запаздывание реального времени от предыдущей засечки
	RealTime	getRealFreezeTime();
	RealTime	getRealUnfreezeTime();
	double		getAcceleration();
	void		setAcceleration(const double newAcceleration);
	void		accelerate();
	void		decelerate();
    void        setAntifreeze(RealTime newAntifreeze);
};

extern WORLD_API ModelTimeQuantizer *globalModelTimeQuantizer ;

#endif //__ModelTimeQuantizer_h__
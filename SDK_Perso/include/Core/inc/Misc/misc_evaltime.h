#ifndef MISK_EVALTIME_H__
#define MISK_EVALTIME_H__

#include <stdint.h>
#include "_config.h"

namespace misc
{
	// Счетчик времени выполнения функции
	struct FunEvalTimeCounter
	{
		double sumtime;		// Сумарное время выполнения
		int evalcnt;		// Число вызовов 
		double averagetime;	// Среднее время 
		double peaktime;	// Пиковое время 
		double mintime;
		FunEvalTimeCounter(){reset();}
		void reset(){sumtime=0; evalcnt=0; peaktime=0; mintime=1.0e100; }
		void calc()
		{
			if( evalcnt)
				averagetime = sumtime/evalcnt;
			else
				averagetime = 0;
		}
	};
	// Время выполнения функции
	class FunEvalTime
	{
		FunEvalTimeCounter& counter;
		int64_t starttime;
	public:
		ED_CORE_EXTERN FunEvalTime(FunEvalTimeCounter& _counter);
		ED_CORE_EXTERN ~FunEvalTime();
	};
};

#endif
#ifndef __PlaneFM_h__
#define __PlaneFM_h__

#include "AerodyneFM.h"

namespace EagleFM
{

class FMBASE_API PlaneFM : public AerodyneFM
{
public:
	PlaneFM(double dt = 0.006);
	virtual ~PlaneFM();

	//доступ к частностям состояния (-1...1 или 0...1 или 0...Pi2):
	virtual double			getEngineTurbineRelativePower(int engineNum)const; //относительная мощность турбины двигателя (0...1..) (для двухконтурных - турбина КНД)
	virtual double			getEngineRelativeThrust(int engineNum)		const; //относительная тяга двигателя
	virtual double			getEngineParam(int engineNum, int param)	const; // 0 - АПУ 1 - Левый 2 - Правый двигатель. Список параметров.
	virtual double			getEngineNozzleState(int engineNum)			const; //состояние сопла (0 - закрыто, 1 - открыто)
	virtual double			getAfterburnerState(int engineNum)			const; //форсаж (0 - нет, 1 - полный)
	virtual double			getPropPhase(int num) const;
	virtual double			getPropPitch(int num) const;
};

}

#endif
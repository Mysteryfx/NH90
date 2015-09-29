#pragma once

#include "ItemBase.h"
#include "Consumer.h"

namespace EagleFM { namespace Elec {

class FMBASE_API ParallelNode : public ItemBase
{
public:
	ParallelNode();
	virtual ~ParallelNode();

	virtual bool connectBeforeMe(ItemBase* itemIn);
	virtual void savePreviousVoltage();
	virtual double getVoltageFromInputs(double threshold);

private:
	ed::vector<double> prevInputVoltages;
};

} }

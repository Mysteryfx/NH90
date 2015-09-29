#pragma once

#include "Source.h"

namespace EagleFM { namespace Elec {

typedef ed::vector<Source*> SourcePVector;

class FMBASE_API CalcNode : public ItemBase
{
public:
	CalcNode(int num);
	virtual ~CalcNode();

	virtual bool UpdateVoltage(double valIn = -1.0);
	virtual void updateOwnVoltage(double valIn = -1.0);

	void updateGlobalVoltage(double valIn);

	void initSources();
	
private:
	void checkSources();
	
	int num;
	SourcePVector voltageSources;
};

} }

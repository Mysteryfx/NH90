#pragma once

#include "FMElectricSystems/Items/Source.h"

namespace EagleFM { namespace Elec {

typedef ed::vector< std::shared_ptr<CalcNodePtr> > calcNodesVec;

class FMBASE_API Converter : public Source
{
public:
	Converter(double nominalInput, double nominalOutput);
	virtual ~Converter() {}

	virtual bool connectBeforeMe(ItemBase* itemIn);
	virtual void setNominalVoltage(double nominal);
	virtual void setOnOff(bool flag_in);
	virtual void setCalcNodePtr(std::shared_ptr<CalcNodePtr> valIn, ItemBase* inputElement = nullptr);
	
	virtual bool isPowered() const;

	double getEfficiencyCoefficient() const {return efficiencyCoefficient;}
	void setEfficiencyCoefficient(double valIn) {efficiencyCoefficient = valIn;}

	void setIsInputDependent(bool valIn) {isInputDependent = valIn;}

protected:
	virtual void updateOwnVoltage(double valIn = -1.0);
	virtual void savePreviousVoltage();

	// convertionK = output U / input U
	double convertionK;
	// Previous input U
	double prevInputVoltage;
	// ÊÏÄ
	double efficiencyCoefficient;
	// If true, output U will be calculated as input U * efficiencyCoefficient
	// If false, output always equals to nominal voltage
	bool isInputDependent;

private:
	CalcNode* getCalcNodeN(int num);

	calcNodesVec calcNodes;
	CalcNode* prevCalcNode;
};

} }

#ifndef __Electric_System__ 
#define __Electric_System__ 

#include "Base.h"
#include "Optimizable.h"

namespace EagleFM
{

class DynamicBody;

namespace Elec {
	class CalculateManager;
	class CalcNode;
}

class FMBASE_API ElectricSystem : public Optimizable
{
	
public:
	ElectricSystem(DynamicBody* dynbd);
	virtual ~ElectricSystem();
	virtual void reset() {}

protected:
	double getAmbientTemperatureC();
	
	DynamicBody* DBody;
};

}

#endif // __Electric_System__ 

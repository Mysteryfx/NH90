#ifndef __AutoTransferSwitch_h__
#define __AutoTransferSwitch_h__

#include "FMElectricSystems/Items/MultiThrowInput.h"

namespace EagleFM { namespace Elec {

class FMBASE_API AutoTransferSwitch : public MultiThrowInput
{
public:
	AutoTransferSwitch();
	virtual ~AutoTransferSwitch();

protected:
	void checkPos(const lwl::CSignalInfo* pInfo);
	void SetControlWire(ItemBase* pItem, bool negative = true);
	virtual bool connectBeforeMe(ItemBase* itemIn);
};

} }

#endif // __AutoTransferSwitch_h__

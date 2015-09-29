#pragma once

#include "FMElectricSystems/Items/Wire.h"

namespace EagleFM { namespace Elec {
// Многопозиционный переключатель.
// Один вход - несколько выходов.
// При переключении запитывается один из выходов.
class FMBASE_API MultiThrowSwitch : public Wire
{
public:
	MultiThrowSwitch();
	~MultiThrowSwitch();

	// methods
	void setPos(int pos);
	int  getPos() const {return m_position;}

	virtual bool canTransmitTo(const ItemBase* device_in) const;

protected:
	
	int m_position;
};

}}

#pragma once

#include "Wire.h"


namespace EagleFM { namespace Elec {

// выключатель
// один вход много выходов 
class FMBASE_API Switch : public Wire
{
public:
	Switch();
	virtual ~Switch();

	virtual bool getIsOn() const;
	virtual void setOnOff(bool flag_in);
	void switchOnOff();

	virtual void Reset();

	virtual bool canTransmitTo(const ItemBase* pItem) const;
	virtual bool canReceiveFrom(const ItemBase* pItem) const;

protected:
	void setOnOff_(bool flag_in);

	bool m_isOn;
};

} }

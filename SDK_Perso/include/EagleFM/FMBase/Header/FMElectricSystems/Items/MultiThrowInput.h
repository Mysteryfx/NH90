// ******************************************************** 
//               COPYRIGHT  (C) 2010
//                
//     This program is property of Eagle Dynamics 
//            All rights reserved.
// ******************************************************** 
//                
//  SubSystem   : Base Electric System  
//  Author		: Mikhail Makaruk 
//  Created		: 22.03.2010
//                
#pragma once

#include "ItemBase.h"

namespace EagleFM { namespace Elec {

// Многопозиционный переключатель.
// Несколько входов - несколько выходов
// При переключении запитываются все выходы от одного из входов.
class FMBASE_API MultiThrowInput : public ItemBase
{
public:
	MultiThrowInput();
	~MultiThrowInput();

	void setPos(unsigned int pos);
	unsigned int getPos() const;
	virtual bool canReceiveFrom(const ItemBase* device_in) const;

protected:
	unsigned int m_position;
};

} }

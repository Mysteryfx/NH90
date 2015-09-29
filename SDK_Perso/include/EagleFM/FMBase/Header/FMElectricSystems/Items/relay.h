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

#include "Switch.h"

namespace EagleFM { namespace Elec {

class FMBASE_API Relay : public Switch
{
public:
	Relay(bool DefaultState = true);
	virtual ~Relay();

	void checkOnOff(const lwl::CSignalInfo* pInfo);
	void SetControlWire(ItemBase* pItem, bool NormallyOpen = true);

private:
	bool m_NormallyOpen;
};

} }

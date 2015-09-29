#pragma once

#include "ItemBase.h"

namespace EagleFM { namespace Elec {

class FMBASE_API Wire : public ItemBase
{
public:
	Wire()
	{
		Type = e_item_Wire;
		m_inputs.SetLimit(1);
	}

	virtual ~Wire() {};	
};

} }

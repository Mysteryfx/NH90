//                    file wBurningTail.h
#pragma once

#include "viObjectNode.h"

#define IID_IBurningTail Common::identify<wBurningTail>() 

class wBurningTail : public viObjectNode
{
public:

		wBurningTail();
		virtual ~wBurningTail();

	static wBurningTail*	create(viObjectNode* _parent, const cVector& point, cVector& scale);

	virtual void			setParent(viObjectNode* _parent, const cVector& point, cVector& scale);
	virtual cPosition&		Position(wModelTime t);
	void					serialize(Serializer &serializer);

protected:

	cPosition				pos;
};
#ifndef __VI_OBJECTSHAPE_H
#define __VI_OBJECTSHAPE_H

#include "private/Base.h"

namespace vi
{

class ObjectShape : protected vi::Base<viObjectShape>
{
public:
	inline ObjectShape(const vi::Object& obj);

	inline Graphics::Model* GetModel();
};

}

#include "inl/ObjectShape.inl"

#endif

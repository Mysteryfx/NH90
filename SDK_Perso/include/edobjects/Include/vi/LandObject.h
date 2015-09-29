#ifndef __VI_LANDOBJECT_H
#define __VI_LANDOBJECT_H

#include "private/Base.h"

namespace vi
{

class LandObject : protected vi::Base<lLandObject>
{
public:
	inline LandObject(const vi::Object& obj);
	inline bool isValid();
	inline wShape *Type() const;
};

}

#include "inl/LandObject.inl"

#endif

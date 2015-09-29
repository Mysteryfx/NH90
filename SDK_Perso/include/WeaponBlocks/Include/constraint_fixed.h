#ifndef __CONSTRAINT_FIXED_H__
#define __CONSTRAINT_FIXED_H__


#include "constraint_base.h"


class wConstraintFixed : public wConstraintBase
{
public:
	wConstraintFixed();
	wConstraintFixed(wIConstraintHost* host, const Math::Vec3d& position, const Math::Matrix3d& orientation);
	virtual ~wConstraintFixed() {};

	virtual eConstraintType getType() const { return CONSTRAINT_FIXED; }
protected:

};

#endif 

#ifndef __UnitProxy__
#define __UnitProxy__

//Classes used to refer units and group by mission id.
//The reason of having them is to create reference to the unit or the group when it is not created yet.

#include "WorldGeneral.h"
#include <ed/string.h>
#include "wMovingObject.h"
#include "wControl.h"

namespace AI
{

class WORLDGENERAL_API UnitProxy
{
public:
	UnitProxy(const ed::string & unitId);	
	void set(const ed::string & unitId);
	const woPointer & get() const;
private:
	mutable woPointer	pUnit_;
	mutable bool		firstCall_;
	ed::string 			unitId_;
};

class WORLDGENERAL_API GroupProxy
{
public:
	GroupProxy(unsigned int groupId);
	void set(unsigned int groupId);
	const wcPointer & get() const;
private:
	mutable wcPointer	pGroup_;
	mutable bool		firstCall_;
	unsigned int		groupId_;
};

}

#endif __UnitProxy__
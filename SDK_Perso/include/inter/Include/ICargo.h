#ifndef __ICargo_h__
#define __ICargo_h__


#include "Inter.h"


struct StaticObjectData;

class ICargo
{
public:
    virtual const StaticObjectData &    getCargoData() = 0;
    virtual void                        setCargoState(int eState, ObjectID extId, bool bSend) = 0;
};



#endif
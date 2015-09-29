#ifndef I_INTERNAL_CARGO_H
#define I_INTERNAL_CARGO_H

#include "../../Core/inc/ed/set.h"
#include "../../edObjects/Include/Registry/Registered.h"

class iInternalCargo
{
public:
    virtual ~iInternalCargo(){}

    virtual bool                addUnitId(ObjectID id) = 0;
    virtual ed::set<ObjectID>       getUnitsId() const = 0;
    virtual bool                checkUnitId(ObjectID id) const = 0;
    virtual bool		        removeUnitId(ObjectID id) = 0;

    virtual bool                setCapacity(int nominalCapacity, int maximalCapacity) = 0;
    virtual int                 getNominalCapacity() const = 0;
    virtual int                 getMaximalCapacity() const = 0;

    virtual float               getUnitsMass() const = 0;
    virtual int                 getUnitsSize() const = 0;
    virtual bool                canPlaceUnits(ed::set<ObjectID> unitsId) const = 0;

};

class iTransportable
{
public:
    virtual ~iTransportable(){}

    virtual bool setSize(int size) = 0;
	virtual void setRandomTransportable(bool randomTransportable) = 0;
    virtual int getSize() const = 0;
    virtual bool isTransportable() const = 0;
	virtual bool isRandomTransportable() const =0;
};

#endif
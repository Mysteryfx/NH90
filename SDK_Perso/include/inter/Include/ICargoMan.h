#ifndef __ICargoMan_h__
#define __ICargoMan_h__


#include "Inter.h"

enum wcCoalitionName;
class  cPosition;


typedef ed::vector<ObjectID> CargoList_t;

class ICargoMan
{
public:
    virtual const CargoList_t&  getCargosList() = 0;
    virtual void                clear() = 0;
    virtual bool                checkCargoChoosen(ObjectID hellID) = 0;
    virtual bool                choosingCargo(ObjectID hellID, ObjectID cargoId) = 0;
    virtual void                cancelChoosingCargo(ObjectID objId) = 0;
    virtual void                chooseCargo(ObjectID objId) = 0;
    virtual void                unhookCargo(ObjectID hellId, const cPosition& pos) = 0;
};


#endif
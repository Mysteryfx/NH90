#pragma once
#include "Terrain.h"
#include "lStandartStructure.h"

class IwWarehouse;

class TERRAIN_API lWarehouseOwner : public lStandartStructure
{
    IwWarehouse * warehouse;
public:
    lWarehouseOwner();
    ~lWarehouseOwner();
    // если объект получил достаточно повреждений, чтобы деградировать
    virtual void Degrade(bool silent);
    virtual void ReinitState();
    
	IwWarehouse       * getWarehouse()			{ return warehouse; };
    const IwWarehouse * getWarehouse() const	{ return warehouse; };

	virtual unsigned	getStoreType() const {return 0;}

    void               take_missionID(const ed::string & group_id,const ed::string & unit_id);
};
#pragma once

#include "Terrain.h"
#include "LandObjects\lWarehouseOwner.h"

#define IID_lOilTank Common::identify<lOilTank>()

// Класс для статических цистерн, они повторно детанируют при любом попадании
class TERRAIN_API lOilTank : public lWarehouseOwner
{
	bool blownUp;
public:
	static lLandObject* CreateObject();
	lOilTank();
	~lOilTank();
	
	// если объект получил достаточно повреждений, чтобы деградировать
	virtual void Degrade(bool silent);
	virtual void ReinitState();
	// зажечь пожары после разрушения строения
	void		 CreateFires();
	virtual unsigned	getStoreType() const {return 0;/*AS_FUEL_STORE*/}
};

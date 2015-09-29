#pragma once

#include "Terrain.h"
#include "LandObjects/lLandObject.h"


#define IID_lShelter Common::identify<lShelter>()

// Аэродромные укрытия
class TERRAIN_API lShelter : public lLandObject
{
private:
	Graphics::DParamList doorstate; // 0-закрыто, 1-открыто

public:
	static lLandObject* CreateObject();
	lShelter();

	//virtual viDrawArguments DrawArguments(wModelTime t, viArgumentPurpose purpose);
	virtual const Graphics::DParamList& GetDrawArguments() {return doorstate;}
	// если объект получил достаточно повреждений, чтобы деградировать
	virtual void Degrade(bool silent);
	// восстановить исходное состояние (если создавалась dyndata)
	virtual void ReinitState();

	// функции открытия/закрытия укрытия
	// возвращают true если операция окончена
	bool Open(float dtime);
	bool Close(float dtime);
	bool IsOpen();
	
	// блоки отрисовки
//	bool AllowDrawBlocks();
};

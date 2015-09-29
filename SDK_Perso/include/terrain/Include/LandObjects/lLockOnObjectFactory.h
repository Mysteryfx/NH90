#pragma once

#include "Terrain.h"
#include "viObjectFactory.h"

// создание объектов
class TERRAIN_API lLockOnObjectFactory : public viObjectFactory
{
public:
	static lLockOnObjectFactory* instance;
	// Создать объект заданного типа
	virtual viObject* CreateObject(
		wShape* type, 
		cPosition& pos, 
		viFlag status,
		void* context);
};


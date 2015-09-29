#pragma once

#include "cPosition.h"
#include "Registry/Registered.h"

class viObject;
struct wShape;
// Интерфейс создания объекта
class viObjectFactory
{
public:
	// Создать объект заданного типа
	virtual viObject* CreateObject(
		wShape* type, 
		cPosition& pos, 
		viFlag status,
		void* context=0)=0;
};

#ifndef TypedResourceManager_H
#define TypedResourceManager_H

#include "Resourcer/ResourceType.h"
#include "graphicsxp.h"

#include <ed/string.h>

class Resource;

// Интерфейс менеджера ресурсов определенного типа
// Необходима регистрация в ресурсере
// Основная задача - создавать ресурс по имени
class GRAPHICSXP_API TypedResourceManager
{
public:
	virtual ~TypedResourceManager();

	// вызываются вместе с инициализацией/деинициализацией ресурсера
	virtual bool initialize() = 0;
	virtual void uninitialize() = 0;

	// Создать ресурс по имени (скорее всего подтребуется обратный вызов к resource->CreateByClass
	// Необходимо создать ресурс, передать ему инициализационные параметры и зарегистрировать
	virtual Resource *CreateByName(ResourceType type, const ed::string& name) = 0;
};

#endif // TypedResourceManager_H

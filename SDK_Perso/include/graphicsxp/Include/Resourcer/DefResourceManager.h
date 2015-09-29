#ifndef DefResourceManager_H
#define DefResourceManager_H

#include "graphicsxp.h"
#include "TypedResourceManager.h"

// простейший менеджер ресурса, создающий ресурсы всегда одного и того же класса
class GRAPHICSXP_API DefResourceManager : public TypedResourceManager
{
protected:
	ed::string className;

public:
	// передается имя класса, из которого будут создаваться ресурсы
	DefResourceManager(const ed::string& className);

	// вызываются вместе с инициализацией/деинициализацией ресурсера
	virtual bool initialize() {return true;}
	virtual void uninitialize() {};

	// Создать ресурс по имени (скорее всего подтребуется обратный вызов к resource->CreateByClass
	// Необходимо создать ресурс, передать ему инициализационные параметры и зарегистрировать
	virtual Resource *CreateByName(ResourceType type, const ed::string& name);
};


#endif // DefResourceManager_H
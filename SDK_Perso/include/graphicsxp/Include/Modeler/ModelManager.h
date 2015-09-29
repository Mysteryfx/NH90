#ifndef ModelManager_H
#define ModelManager_H

#include "graphicsxp.h"
#include "Resourcer\DefResourceManager.h"
#include "Renderer\RendererCallback.h"

namespace Graphics
{

// Стандартный менеджер создания моделей
class GRAPHICSXP_API ModelManager : public DefResourceManager, public RendererCallback
{
protected:
	typedef ed::vector< ed::string > stringList;

	// пути до моделей
	stringList paths;

	// варианты расширений моделей
	stringList extensions;

	// попробовать найти по путям данный файл, расширение задано
	ed::string FindInPaths(const ed::string& name_ext);

	// попробовать загрузить как edm модель
	Resource *LoadAsNGModel(const ed::string& name);
	
	// попробовать загрузить как edm модель
	Resource *LoadAsSetOfLods(const ed::string& name);

public:
	ModelManager(const ed::string& className = "Standart");

	// вызываются вместе с инициализацией/деинициализацией ресурсера
	virtual bool initialize();
	virtual void uninitialize();

	// синхронизация
	virtual void OnInitialize();
	
	// попробовать найти данный файл
	ed::string FindFile(const ed::string& name);

	// Создать ресурс по имени (скорее всего подтребуется обратный вызов к resource->CreateByClass
	// Необходимо создать ресурс, передать ему инициализационные параметры и зарегистрировать
	virtual Resource *CreateByName(ResourceType type, const ed::string& name);
};

} // namespace Graphics

#endif // ModelManager_H

#ifndef ResourceManager_H
#define ResourceManager_H

#include "graphicsxp.h"
#include "ResourceType.h"

#include "ed/vector.h"

#include <ed/string.h>

class Resource;
class TypedResourceManager;
class Plugin;

// функция, создающая ресурс
typedef Resource* (ResourceCreator)(ResourceType type, const ed::string &className);

typedef void (*ResourceEvent_t)(int operation, Resource*, void* data);

// Интерфейс менеджера графических ресурсов
class ResourceManager
{
public:
	virtual ~ResourceManager() {};

	// инициализация/деинициализация
	// необходимо вызвать после инициализации и до деинициализации renderer
	virtual bool initialize() = 0;
	virtual bool isinitialized() = 0;
	virtual void uninitialize() = 0;

	// Обращение к ресурсам (при необходимости ресурс создастся)
	// При создании из класса ресурс не регистрируется
	virtual bool IsResourceExists(ResourceType type, const char *name) = 0;
	virtual Resource *GetResourceByClass(ResourceType type, const char *className) = 0;
	virtual Resource *GetResource(ResourceType type, const char *name) = 0;
	
	inline bool IsResourceExists(ResourceType type, const ed::string &name){return IsResourceExists(type, name.c_str());}
	inline Resource *GetResourceByClass(ResourceType type, const ed::string &className){return GetResourceByClass(type, className.c_str());}
	inline Resource *GetResource(ResourceType type, const ed::string &name){return GetResource(type, name.c_str());}

	// Регистрация
	virtual void RegisterResourceClass(ResourceType type, const char *className, ResourceCreator *creator) = 0;
	virtual void RegisterTypedManager(ResourceType type, TypedResourceManager *manager) = 0;

	// получение зарегистрированных классов и менеджеров
	virtual int GetClassCount(ResourceType type) = 0;
	virtual const char* GetResourceClass(ResourceType type, int index) = 0;
	virtual TypedResourceManager *GetTypedManager(ResourceType type) = 0;

	// получение зарегистрированных плагинов
	virtual Plugin *GetPlugin(const char *name) = 0;
	inline Plugin *GetPlugin(const ed::string &name){return GetPlugin(name.c_str());}
	virtual int GetPluginCount() = 0;
	virtual Plugin *GetPlugin(int index) = 0;

    // уровень загрузки ресурса синхронно, при вызове resource->UsedThisFrame()
    // 0 - для более плавного полета, но при первом взгляде объекты могут быть не видны
    // 100 - все ресурсы будут синхронно прогружаться
    virtual void SetOnDemandLoadStep(int level) = 0;

	// Выделение времени менеджеру для загрузки ресурсов и других действий в однопоточном режиме
	// Должен обязательно вызываться каждый кадр
	// maxTime - время, отводимое на загрузку ресурсов
    // возвращает true если хоть один ресурс был дозагружен
	virtual bool Update(float maxTime) = 0;

	//увеличить счетчик кадров. Нужен для парса лампочек, чтобы не вызывать Update(..) 
	//и не обнулять таким образом фрейм хип (симс ту би костыль, мопед не мой, я просто разместил объяву...(с))
	virtual void NextFrame() = 0;
	
	// получить текущий номер кадра (счетчик увеличивается в конце Update)
	virtual unsigned GetCurFrame() = 0;

	// Уничтожение всех аппаратных ресурсов (будет вызвано FreeToStep(0) для каждого ресурса)
	virtual void Purge(ResourceType type) = 0;

	//костыльный метод освобождения ресурса
	virtual void UncheckedFree(ResourceType type, const char *name) = 0;
	inline void UncheckedFree(ResourceType type, const ed::string &name){UncheckedFree(type, name.c_str());}

	// Обновить
	virtual void Refresh(ResourceType type) = 0;

	// Получить конец очереди на отгрузку (ресурс который не был нужен наиболее длительное время)
	virtual Resource *GetNotUsed(ResourceType type) = 0;

	// Dump (Mitka 29.01.2008)
	virtual void Dump(FILE* outfile, const char* options=NULL){};

	// Callback (Mitka 20.01.2010)
	virtual void setCallBack(ResourceEvent_t callback, void* data)=0;

	/// Returns list of loaded resources of given type.
	virtual void getResourcesNames(ResourceType type, ed::vector<ed::string> &res)const = 0;
};

extern GRAPHICSXP_API ResourceManager *resourcer;

#endif // ResourceManager_H

#ifndef ResourceManagerImpl_H
#define ResourceManagerImpl_H

#include "Resourcer\ResourceManager.h"

class Plugin;

#include <ed/set.h>
#include <ed/map.h>
#include <ed/unordered_map.h>
#include <ed/vector.h>

// Аппаратно-независимая реализация менеджера ресурсов
class ResourceManagerImpl : public ResourceManager
{
protected:
	typedef ed::unordered_map<ed::string, Resource *> resourceList;
	typedef ed::set<Resource *> unnamedList;

	struct typed_store
	{
		resourceList named;
		unnamedList unnamed;

		// двунаправленный список, при UsedThisFrame ресурс ставится в начало
		Resource *last_used, *not_used;

		typed_store() {last_used = not_used = 0;}
	};
	typedef ed::vector<typed_store> resourceStore;

	typedef ed::map<ed::string, ResourceCreator *> classList;
	typedef ed::vector<classList> classStore;

	typedef ed::vector<TypedResourceManager *> managerList;

	typedef std::multimap<ed::string, Plugin *> pluginList;

	struct priorityResource
	{
		float priority;
		Resource *resource;

		priorityResource(float _priority, Resource *_resource)
		{
			priority = _priority;
			resource = _resource;
		}

		inline bool operator <(const priorityResource& op) const
		{
			return priority < op.priority;
		}
	};
	typedef std::priority_queue<priorityResource> resourceQueue;

	// инициализирован
	bool initialized;

    // уровень загрузки при вызове resource->UsedThisFrame()
    int onDemandStep;

	// основное хранилище ресурсов
	resourceStore store;

	// хранилище фабрик ресурсов
	classStore factories;

	// хранилище специализированных менеджеров ресурсов
	managerList managers;

	// список плагинов
	pluginList plugins;

	// очередь ресурсов, зарегистрированных на загрузку
	resourceQueue forUpload;

	// номер кадра
	unsigned cur_frame;

	// resource callback
	ResourceEvent_t callback;
	void* callbackdata;
public:
	ResourceManagerImpl();
	~ResourceManagerImpl();

	// инициализация/деинициализация
	virtual bool initialize();
	virtual bool isinitialized();
	virtual void uninitialize();

	// Обращение к ресурсам (при необходимости ресурс создастся)
	virtual bool IsResourceExists(ResourceType type, const char *name);
	virtual Resource *GetResourceByClass(ResourceType type, const char *className);
	virtual Resource *GetResource(ResourceType type, const char *name);

	// Регистрация
	virtual void RegisterResourceClass(ResourceType type, const char *className, ResourceCreator *creator);
	virtual void RegisterTypedManager(ResourceType type, TypedResourceManager *manager);
	virtual bool RegisterResource(Resource *resource);

	// регистрация плагинов (автоматическая)
	virtual void RegisterPlugin(Plugin *plugin);
	virtual void UnregisterPlugin(Plugin *plugin);
	
	// получение зарегистрированных классов и менеджеров
	virtual int GetClassCount(ResourceType type);
	virtual const char * GetResourceClass(ResourceType type, int index);
	virtual TypedResourceManager *GetTypedManager(ResourceType type);

	// получение зарегистрированных плагинов
	virtual Plugin *GetPlugin(const char *name);
	virtual int GetPluginCount();
	virtual Plugin *GetPlugin(int index);

    // уровень загрузки ресурса синхронно, при вызове resource->UsedThisFrame()
    // 0 - для более плавного полета, но при первом взгляде объекты могут быть не видны
    // 100 - все ресурсы будут синхронно прогружаться
    virtual void SetOnDemandLoadStep(int level);

	// получить строку - имя типа ресурса
	virtual const char *GetResourceTypeName(ResourceType type);
	
	// загрузить ресурс до следующего шага
	virtual bool LoadToNextStep(Resource *resource);

	// Выделение времени менеджеру для загрузки ресурсов и других в однопоточном режиме
	// Должен обязательно вызываться каждый кадр
	// maxTime - время, отводимое на загрузку ресурсов
    // возвращает true если хоть один ресурс был дозагружен
    virtual bool Update(float maxTime);

	//увеличить счетчик кадров. Нужен для парса лампочек, чтобы не вызывать Update(..) 
	//и не обнулять таким образом фрейм хип (симс ту би костыль, мопед не мой, я просто разместил объяву...(с))
	virtual void NextFrame();

	// Уничтожение всех аппаратных ресурсов (будет вызвано FreeToStep(0) для каждого ресурса)
	virtual void Purge(ResourceType type);

	// Обновить
	virtual void Refresh(ResourceType type);

	// уведомление об уничтожении ресурса
	virtual void OnResourceDestroy(Resource *resource);

	//костыльный метод освобождения ресурса
	void UncheckedFree(ResourceType type, const char *name);

	// получить текущий номер кадра
	virtual unsigned GetCurFrame();

	// ресурс нужен - зарегистрировать на загрузку
	void RegisterForUpload(Resource *resource);

	// операции над очередью ресурсов
	Resource *GetLastUsed(ResourceType type);
	void SetLastUsed(ResourceType type, Resource *resource);
	virtual Resource *GetNotUsed(ResourceType type);
	void SetNotUsed(ResourceType type, Resource *resource);

	// Dump (Mitka 29.01.2008)
	virtual void Dump(FILE* outfile, const char* options=NULL);

	// Callback (Mitka 20.01.2010)
	virtual void setCallBack(ResourceEvent_t callback, void* data);

	virtual void getResourcesNames(ResourceType type, ed::vector<ed::string> &res)const;

#ifdef ED_ALLOCATOR_DEBUG
	unsigned int getNumTextures()const;
	unsigned int getNumModels()const;
#endif
};

#endif // ResourceManagerImpl_H

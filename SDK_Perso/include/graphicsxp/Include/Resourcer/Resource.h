#ifndef Resource_H
#define Resource_H

#include "graphicsxp.h"
#include "Common.h"
#include "ResourceType.h"

#include <ed/string.h>

static const int FORCE_LOAD_RESOURCE = 0x7fffffff;

#pragma pack (8)

// Интерфейс ресурса
class Resource{
private:
    bool registered;
	ed::string name;
	unsigned last_frame;

	// двунаправленный список в порядке использования
	Resource *prev, *next;
	void MakeFirstInList();
protected:
	ResourceType rtype;
public:
	GRAPHICSXP_API Resource();
	GRAPHICSXP_API virtual ~Resource();

	// Создать ресурс по имени
	// не все ресурсы обязаны поддерживать эту возможность
	GRAPHICSXP_API virtual bool CreateByName(const ed::string& resname);

	// Общие характеристики ресурса

	// тип ресурса
	GRAPHICSXP_API virtual ResourceType Type();

    // зарегистрирован ли он в ResourceManager
    GRAPHICSXP_API virtual bool IsRegistered();
	
	// имя ресурса (уникально среди ресурсов данного типа)
	GRAPHICSXP_API const ed::string& Name()const;
	GRAPHICSXP_API void SetName(const ed::string& resname);

	// имя класса ресурса
	virtual const ed::string& ClassName() = 0;

	// вернуть строку с характеристиками ресурса (для отладки)
	GRAPHICSXP_API virtual ed::string GetDebugString();

    // зарегистрировать в менеджере ресурсов (имя должно быть заполнено)
    GRAPHICSXP_API virtual void Register();
	GRAPHICSXP_API virtual void Unregister();

	// управление загрузкой ресурса (может быть реализовано с поддержкой многопоточности)
	// загрузка производится по шагам, семантика шагов зависит от конкретного типа ресурса
	// на шаге 0 не должно быть захвачено никаких аппаратных ресурсов
	
	// получить приоритет ресурса
	virtual float GetLoadPriority() = 0;

	// получить текущий шаг загрузки
	virtual int GetLoadStep() = 0;
	// вернет минимальный LoadStep для подчиненных ресурсов
	GRAPHICSXP_API virtual int GetDependentsLoadStep();

	// получить количество шагов для полной загрузки ресурса
	virtual int GetLoadStepCount() = 0;

	// загрузить ресурс до шага step
	virtual void LoadToStep(int step) = 0;

	// освободить ресурс до шага step
	virtual void FreeToStep(int step) = 0;
	
	// загрузить ресурс вместе с необходимыми для него другими ресурсами
	GRAPHICSXP_API virtual void LoadWithDependents(int step);

	// индекс кадра последнего обращения, менеджер ресурсов будет стараться 
	// освобить не используемые ресурсы

	// если кто-то использует данный ресурс - необходимо вызвать эту функцию
	GRAPHICSXP_API virtual void UsedThisFrame();

	// получить индекс кадра последнего обращения
	GRAPHICSXP_API virtual unsigned GetLastFrame();

	// сериализация
	GRAPHICSXP_API virtual void serialize(Serializer& serializer);

	// Обновить
	GRAPHICSXP_API virtual void Refresh();

	// Убрать из очереди на отгрузку (должно вызываться из FreeToStep(0) )
	GRAPHICSXP_API void RemoveFromList();
	GRAPHICSXP_API Resource *GetPrevForUnload();
};

#pragma pack ()

#endif // Resource_H

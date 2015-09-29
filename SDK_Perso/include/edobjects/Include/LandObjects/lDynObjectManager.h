#ifndef lDynObjectManager_H
#define lDynObjectManager_H
#include "edObjects.h"

#include "Common.h"
#include "Serializer\Serializable.h"
#include <ed/string.h>
#include "cPosition.h"
#include "LandObjects/lLandObject.h"



// измененные объекты регистрируются здесь
class EDOBJECTS_API lDynObjectManager : public Common::Unknown<Serializable>
{
public:
	// глобальный менеджер
	static lDynObjectManager *instance;

	// конструктор/деструктор
	static lDynObjectManager *create();
	virtual ~lDynObjectManager() {};

	// Начать работу
	virtual void start() = 0;
    // Закончить работу
    virtual void stop() = 0;


    // добавление/удаление объектов из списка
    virtual void addObject(lLandObject *obj) = 0;
    virtual void removeObject(lLandObject *obj) = 0;

	// добавление
	virtual void addStaticObject(lLandObject* obj) = 0;

    // Сообщения о повреждениях объектов
    // Должна вызываться только для клиентских объектов
    virtual void registerObjectDamage(lLandObject* obj, double damage, ObjectID hitman) = 0;
    // Должна вызываться только для серверных объектов
    virtual void registerObjectDegrade(lLandObject* obj, ObjectID hitman) = 0;
	
	// Сообщение об удалении объектов
	// Должна вызываться только для серверных объектов
	virtual void registerObjectDestroy(lLandObject* obj) = 0;

	// создать обломки
	virtual void createDebris(const ed::string &shape, const cPosition &pos) = 0;
};

#endif // lDynObjectManager_H
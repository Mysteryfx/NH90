#ifndef __LTYPEOBJECTMANAGER_H__
#define __LTYPEOBJECTMANAGER_H__

#include "edObjects.h"

#include "Registry\Registered.h"

#ifdef GetUserName
//#error FUCK WinAPI!
#undef GetUserName
#endif

struct wShape;
class lLandObject;
class lObjectManager;
class cPosition;
class gShapeObject;

namespace Lua
{
    class Config;
}

class wShapeFunctor
{
public:
	virtual ~wShapeFunctor(){}
    virtual void operator()(wShape * shape)const = 0;
};
// Для доступа к типам объектов
class EDOBJECTS_API lTypeObjectManager
{
public:
	static lTypeObjectManager* instance;

	static lTypeObjectManager* Create();

	// Инициализация
	virtual bool Init(
		const char* foldername		// Имя текст. таблицы объектов
		)=0;
	virtual bool AddShapeTable(
		const char* filename
		)=0;
    virtual bool AddShapeTable(
        Lua::Config & config
        )=0;

	////////////////////////////////////
	// Данные объекта
	virtual const char* GetName(
		int itbl, int ind)=0;
	virtual gShapeObject* GetGObject(
		int itbl, int ind)=0;
	virtual float GetLife(
		int itbl, int ind)=0;
	virtual const char* GetUserName(
		int itbl, int ind)=0;
	virtual int GetDestr(
		int itbl, int ind)=0;

	// Тип объекта по имени
	virtual wShape* GetType(
		const char* type)=0;

	// Тип объекта по индексу
	virtual wShape* GetType(
		int itbl, int ind)=0;


	//  пакетная работв с объектами
    virtual void forEachShape(const wShapeFunctor & func) = 0;

	// Тип объекта по имени USERNAME
	virtual wShape* GetTypeByUserName(
		const char* username)=0;

	// Создать объект
	virtual lLandObject* CreateLandObject(
		wShape* objecttype, 
		const cPosition& pos,
		viFlag status = viUnknown,
		double scale = 1.0
		)=0;

	// Загрузить тип объекта
	virtual void LoadType(
		wShape* type, 
		float priority=0.1)=0;
};
// Реализация
#define pTypeObjectManager (lTypeObjectManager::instance)

#endif

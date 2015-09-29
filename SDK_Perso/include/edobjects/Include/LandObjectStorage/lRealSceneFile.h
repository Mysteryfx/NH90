#ifndef __L_REAL_SCENE_FILE_FORMAT_H__
#define __L_REAL_SCENE_FILE_FORMAT_H__

#include "lTerraBasic.h"
//#include "zDatatypes.h"
//#include "gGeometry.h"
#include "Registry/Registered.h"
#include "Registry/RegisterManager.h"
#include "misc/misc_mmf.h"

// Текущая версия
const unsigned int realscene_currentversion = 1;

////////////////////////////////////////////////
// Файл описания объектов сцены
struct EDOBJECTS_API RealSceneFileHeader
{
	unsigned int version;				// Версия 
	// Версия исходных данных
	ObjectCollection_Version srcdata_version;

	unsigned int typescount;			// Число типов
	unsigned int objectscount;			// Число объектов
	ObjectID startId;					// Границы выгр. ид.

	unsigned int objectstypes_offset;	// Ссылка на массив id объектов
	unsigned int objects_offset;		// Ссылка на массив объектов


	// ObjectType[typescount]
	// ObjectID [objectscount];
	// Object [objectscount]

	struct ObjectType
	{
		char type[32];
	};

	struct Object
	{
		cPosition pos;
		unsigned int type;
	};

	ObjectType* getType(int index)
	{
		if(index<0) return NULL;
		if(index>=(int)typescount) return NULL;

		ObjectType* pObjectType = (ObjectType*)(((char*)this) + objectstypes_offset);
		return pObjectType + index;
	}
	Object* getObject(ObjectID id)
	{
		if( id<startId) return NULL;
		id -= startId;
		if( id>=objectscount) return NULL;
		register Object* pObject = (Object*)(((char*)this) + objects_offset);
		return pObject + id;
	}
};

class RealSceneFile
{
protected:
	misc::mmf theMMF;
	RealSceneFileHeader* pRealSceneFileHeader;
public:
	RealSceneFile();
	~RealSceneFile();

	// Открыть файл
	// Поиск файла с нужной версией
	bool Open(
		const char* filename, 
		ObjectCollection_Version srcdata_version);
	void Close();

	// Интервал ид-ров
	void GetIdInterval(
		ObjectID& startId,			// Границы выгр. ид.
		ObjectID& endId);
	// Позиция и тип объекта
	cPosition* getObjectById(
		ObjectID id, 
		char*& type);

public:
	// Записать объекты в файл
	static bool WriteToFile( 
		RegisterManager* registry,	// Реестр
		ObjectID startId,			// Границы выгр. ид.
		ObjectID endId,				
		ObjectCollection_Version& collection_ver, // Версия
		const char* filename);		// Имя файла

};

#endif
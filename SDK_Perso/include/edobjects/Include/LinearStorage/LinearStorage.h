#ifndef LinearStorage_H
#define LinearStorage_H

#include "misc/misc_evaltime.h"
#include "viObjectStorage.h"

// линейное хранилище: при любом поиске проверяются все объекты
class EDOBJECTS_API LinearStorage : public viObjectStorage
{
private:
	typedef ed::map<ObjectID, viObject *> StoreType;
	StoreType store;

	// Статистика
	misc::FunEvalTimeCounter tGetObjects;

public:
	LinearStorage(viFlag _types);
	virtual ~LinearStorage();

	void getAllObjects(ed::vector<viObject*>& objects);
public:
	// имя типа хранилища
	virtual const char *Name(void);

	// операции с объектами
	virtual bool AddObject(viObject *object);
	virtual bool RemoveObject(viObject *object);

	// выбросить объекты из хранилищ по маске
	// хранилища имеют право не выбрасывать объекты - это только для оптимизации
	virtual void Purge(viFlag mask);
	
	// поиск по структуре поиска
	virtual void GetObjects(viSearch *search);

	// выдать количество объектов в хранилище
	virtual unsigned GetCount(void);

	// выдать состояние (статистику)
	virtual void GetStatistics(char buffer[]);
};

#endif // LinearStorage_H

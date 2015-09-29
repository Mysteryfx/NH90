#ifndef HierarchyStorage_H
#define HierarchyStorage_H

#include "HierarchyNode.h"
#include "viObjectStorage.h"

class EDOBJECTS_API HierarchyStorage : public viObjectStorage
{
public:
	Link<HierarchyNode> root;

	HierarchyStorage(viFlag mask, HierarchyNode *_root);
	~HierarchyStorage();
	
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

#endif // HierarchyStorage_H
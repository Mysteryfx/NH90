#ifndef ShapeQuadStorage_h
#define ShapeQuadStorage_h

#include "cLinear.h"
#include "viObjectStorage.h"
#include "GraphMath/box.h"

class ShapeQuadTreeNode;
class ShapeQuadFileStorage;
namespace BinarySceneAndTree{ class File;}
namespace scene3{ class lDistrictsSceneFile;}

// coordinates assume x=north, z=east, y=up
class EDOBJECTS_API ShapeQuadStorage : public viObjectStorage
{
protected:
	friend class ShapeQuadFileStorage;
	friend class BinarySceneAndTree::File;
	friend class scene3::lDistrictsSceneFile;
    float xMin, xMax;
    float zMin, zMax;

	ShapeQuadTreeNode *root;

public:
    // rectangle for all land
    ShapeQuadStorage(const Box &borders, viFlag _flags);
    virtual ~ShapeQuadStorage();

	ShapeQuadTreeNode* getRoot(){return root;}

	// имя типа хранилища
	virtual const char *Name(void);

	// операции с объектами
	virtual bool AddObject(viObject *object);
	virtual bool RemoveObject(viObject *object);

	// выбросить объекты по маске
	virtual void Purge(viFlag mask);
	
	// поиск по структуре поиска
	virtual void GetObjects(viSearch *search);
	
	// получить максимальную высоту объекта по фигуре
	float QueryMaxHeight(const cPoint& sw, const cPoint& ne, float size=0.f);

	// выдать количество объектов в хранилище
	virtual unsigned GetCount(void);

	// выдать состояние (статистику)
	virtual void GetStatistics(char buffer[]);
	
	// Распечатать содержимое в файл
	virtual void PrintToFile(char *filename);
};

#endif // MapObjectsStorage

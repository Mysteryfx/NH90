#pragma once
#ifdef USE_TERRAIN4

#include "viObjectStorage.h"
#include "viObjectHandle.h"
#include <memory>
#include <osg/Matrixd>

namespace edterrain {
	class ITerrain;
}

namespace model 
{
	class IModel;
}

class ShapeQuadStorage;
class Box;

class EDOBJECTS_API LandObjectStorage4 : public viObjectStorage
{

public:
	LandObjectStorage4(const Box& borders, edterrain::ITerrain& terrain);
	~LandObjectStorage4();

	// имя типа хранилища
	const char *Name(void) override;

	// операции с объектами
	bool AddObject(viObject *object) override;
	bool RemoveObject(viObject *object) override;

	// выбросить объекты из хранилищ по маске
	// хранилища имеют право не выбрасывать объекты - это только для оптимизации
	void Purge(viFlag mask)override;

	// поиск по структуре поиска
	void GetObjects(viSearch *search) override;

	// выдать количество объектов в хранилище
	unsigned GetCount(void) override;

	// выдать состояние (статистику)
	void GetStatistics(char buffer[]) override;

	QueryObjectResult QueryObjectsCB(const osg::Vec3d& pt1, const osg::Vec3d& pt2, OnQuerySceneObjectCallback func);
	QueryObjectResult QueryObjectsCB(const osg::Polytope& polytope, OnQuerySceneObjectCallback func);
	QueryObjectResult QueryObjectsCB(const osg::BoundingBoxf& bbox, OnQuerySceneObjectCallback func);

private:
	std::unique_ptr<ShapeQuadStorage> mMutTree;
	edterrain::ITerrain& mTerrain;
};

#endif 
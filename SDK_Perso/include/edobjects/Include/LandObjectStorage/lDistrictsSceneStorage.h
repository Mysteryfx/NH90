#ifndef _lDistrictFile_H
#define _lDistrictFile_H
#ifndef USE_TERRAIN4

#include <osg/Vec3d>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/BoundingBox>

#include "viObjectStorage.h"
#include "Registry/RegVectorStorage.h"

#include "io/mmfStream.h"
#include "viObjectFlatShape.h"
#include "Modeler/gGeometry.h"
#include "ClipVolumes/IntersectionType.h"
#include "ClipVolumes/ClipVolumeBox.h"
#include "landscape3/lDistrictsSceneFile.h"

class ShapeQuadTreeNode;

// 
// Хранилище кварталов
// 
namespace scene3
{
	#define INVALID_VIOBJECT ((viObject*)~0L)

	// DistrictInstance для запихивания в storage
	struct DistrictInstanceObject : public viObjectFlatShape
	{
		lDistrictsSceneFile* districtsSceneFile;
		DistrictInstanceDynamic* dynamic;

		cPosition pos;
		gBox box;
		ClipVolumeBox flatshape;

		ed::vector<viObject*> subobjects;
	
		EDOBJECTS_API DistrictInstanceObject();
		EDOBJECTS_API void Init(DistrictInstanceDynamic* dynamicDistrict, lDistrictsSceneFile* districtsSceneFile);

		// viObjectFlatShape functions
		virtual ClipVolume* FlatShape();

		// viObject virtual functions
		virtual cPosition &Position(wModelTime t);
		virtual const gBox &LocalBox(wModelTime t);
		
		//Graphics::ModelInstance virtual functions
		virtual const Position3& GetPosition();
	};

	// Файл с кварталами
	class EDOBJECTS_API lDistrictsSceneStorage : public viObjectStorage , public RegStorage
	{
	public:
		lDistrictsSceneStorage();
		~lDistrictsSceneStorage();

		// разрушить домик квартала
		virtual bool destroyBuilding(ObjectID id);
		// Открыть файл
		bool Load(scene3::lDistrictsSceneFile* districtsSceneFile, int district_startId);

	// RegStorage
	public:
		virtual ObjectID Insert(Registered *obj);
		virtual bool Remove(Registered *obj);
		virtual Registered *Next(ObjectID id, viFlag oftype = viUnknown);
		virtual Registered *Prev(ObjectID id, viFlag oftype = viUnknown);
		virtual int DeleteAll(viFlag mask);
		virtual int Count() const;
		virtual void serializeSelf(Serializer &);
		virtual void serializeObjects(Serializer &);
		virtual Registered *Get(ObjectID id);
		virtual ObjectID ForceID(Registered *obj, ObjectID id);

	// viObjectStorage
	public:
		// имя типа хранилища
		virtual const char *Name(void);

		// операции с объектами
		virtual bool AddObject(viObject *object);
		virtual bool RemoveObject(viObject *object);

		// выбросить объекты по маске
		virtual void Purge(viFlag mask);
		
		// поиск по структуре поиска
		virtual void GetObjects(viSearch *search);

		// поиск по объему
		void QueryObjects(ClipVolume* search, IDistrictQuerySink* pQuerySink);

		// выдать количество объектов в хранилище
		virtual unsigned GetCount(void);

		// выдать состояние (статистику)
		virtual void GetStatistics(char buffer[]);

	protected:
		// квартал по ID
		viObject* GetDistrictById(ObjectID id);

		// рекурсия. GetStatistics
		void GetStatistics_rec(scene3::StaticSpatial::TreeNode* treenode, int *objects, int *nodes, int *references, int *maxLevel, int *maxObjectPerNode, float *minNodeSize);

		// рекурсия для GetObjectList
		bool GetObjectList_rec(scene3::StaticSpatial::TreeNode* treenode, viSearch* search, IntersectionType it = itIntersects);
		// рекурсия для GetCount
		int GetCount_rec(scene3::StaticSpatial::TreeNode* treenode);

	protected:
		// source
		lDistrictsSceneFile* districtsSceneFile;
		// начальный id
		int district_startId;	
		// инстансированые кварталы
		ed::vector<DistrictInstanceObject*> districtObjects;	
	};
}

#endif // ifndef USE_TERRAIN4
#endif
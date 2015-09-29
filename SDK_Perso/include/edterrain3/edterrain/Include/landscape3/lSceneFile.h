#ifndef __LANDSCAPE3_LSCENEFILE_H
#define __LANDSCAPE3_LSCENEFILE_H
#include "../landscape3_config.h"

#include <osg/Matrixd>
#include <osg/BoundingBox>
#include <osg/Polytope>

namespace model
{
class IGraphModelManager;
class IGraphModel;
class IModel;
}
namespace Graphics
{
class Model;
}
struct wShape;

namespace scene3
{
// интерфейс запроса объектов. см QueryObjects
struct IQuerySink
{
	virtual void OnObject(
	    int id,
	    const osg::Matrixd& pos,
	    int32_t type
		)=0;
};

// Файл для доступа к объектам сцены
class EDTERRAIN_API lSceneFile
{
public:
	// типы домиков
	ed::vector< ed::string> building_types;
	// боксы для типов
	ed::vector< osg::BoundingBox> building_type_boxes;

	// позиции объектов
	io::mmfvector<osg::Matrixd> positions;
	// типы объектов в списке building_types (-1 - invalid type)
	io::mmfvector<int32_t> types;
	// дистанция видимости объекта (уровень) 256*2^(level): 0=256, 1=512, 2=1024, 3=2048, ....
	// дистанция видимости бокса радиусом 1м принята 256. ONE_METER_BOX_DISTANCE = 256
	static uint32_t ViewDistanceLevel(const osg::BoundingBox& box);
	// индекс в списке связности (-1 - empty)
	io::mmfvector<int32_t> indexInLinkList;

	// список связности
	io::mmfvector<int32_t> linkList;

	//////////////////////////////////////////////////
	// дерево:
	struct TreeNode
	{
		float xMin, xMax;
		float zMin, zMax;
		float yMin, yMax;

		// Полное число объектов
		uint32_t objCount; // including children
		// Число объектов в массиве
		uint32_t local_objectcount;

		// индекс в массисе ObjectID
		uint32_t ObjectID_index;

		// смешение на массив смещений на детей, м.б. -1
		int32_t offset_ChildsArray;
	};
	// nodes
	io::mmfvector<TreeNode> nodes;
	// массив child индексов TreeNode, суда ссылается TreeNode::offset_ChildsArray
	io::mmfvector<int32_t> ChildNodes;
	// массив ид, суда ссылается TreeNode::ObjectID_index
	io::mmfvector<uint32_t> ObjectIDs;

	// полный бокс для файла
	osg::BoundingBox bound;

	// NON SERIALIZABLE
	// Временные данные для рендера
public:
	// загруженые модели строений
#ifdef EDGE
	ed::vector< model::IGraphModel*> loaded_building_types;
#else
	ed::vector< model::IModel*> loaded_models_types;
	ed::vector< wShape*> loaded_building_types;
#endif
	ed::vector< int> loaded_references;

	// кеширование выборки. чтоб не возвращала дубликаты
	int currentquery;
	ed::vector< int> objectqueries;

public:
	lSceneFile();

	void clear();
public:
	// Открыть файл
	bool Load(
	    io::MmfStream& mmfStream
	);
	bool Save(
	    const char* filename
	);
	bool Serialize(io::Stream& file);

public:
	int GetCount()
	{
		return positions.size();
	}
	// выдать состояние (статистику)
	void GetStatistics(char buffer[]);

	// Запрос объектов
	int QueryObject(
	    const osg::Polytope& polytope,
	    IQuerySink* pQuerySink=0
	);

	// Запрос объектов
	int query( const osg::Polytope& polytope, IQuerySink* pQuerySink=0);
	int query(const osg::BoundingBoxf& bbox, IQuerySink* pQuerySink = 0);
	int query( const osg::Vec3d& a, const osg::Vec3d& b, IQuerySink* pQuerySink=0);

	// Вернет количество связаных объектов
	int GetLinkedObjects(int32_t id, int32_t* buffer, int buffersize);

protected:
	// рекурсия. GetStatistics
	void GetStatistics_rec(scene3::lSceneFile::TreeNode* treenode, int* objects, int* nodes, int* references, int* maxLevel, int* maxObjectPerNode, float* minNodeSize);

	// рекурсия. Запрос объектов
	int QueryObject_rec(
	    lSceneFile::TreeNode* treenode,
	    const osg::Polytope& polytope,
	    IQuerySink* pQuerySink=0
		);
	int QueryObject_rec(
		lSceneFile::TreeNode* treenode,
		const osg::BoundingBoxf& bbox,
		IQuerySink* pQuerySink = 0
		);
	int QueryObject_rec(
	    lSceneFile::TreeNode* treenode,
	    const osg::Vec3d& a, const osg::Vec3d& b, 
		IQuerySink* pQuerySink=0
		);
};

}

#endif
#ifndef _LANDSCAPE3_LDISTRICTFILE_H
#define _LANDSCAPE3_LDISTRICTFILE_H
#include "../landscape3_config.h"
#include "lTerraBasic.h"

#include <osg/Vec3d>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/BoundingBox>
#include <osg/Polytope>
#include "StaticSpatial.h"
#include "landscape3/lSceneFile.h"

#ifdef EDGE
namespace model
{
class IGraphModelManager;
class IGraphModel;
class IModel;
}
#else
namespace Graphics
{
class Model;
}
namespace model
{
class IModel;
}
struct wShape;
#endif

//
// Файл сцены состоящей из кварталов
//
namespace scene3
{
class lBlockFile;

// интерфейс запроса объектов. см QueryObjects
struct IDistrictQuerySink
{
	virtual void OnDistrict(uint32_t index) = 0;
};

struct DistrictInstance
{
	enum enFlags
	{
		VALID=1,		// домик есть
		VISIBLE=2,		// домик виден в кварталах
	};

	// тип квартала см. types
	int districttype;

	// бокс в WS
	osg::BoundingBox box;

	// позиции домов квартала (в WS)
	io::mmfvector<osg::Matrixd> object_pos;
	// тип домика
	io::mmfvector<uint32_t> object_type;
	// флаги, см enFlags
	io::mmfvector<uint32_t> object_flag;
	// y компоненты векторов object_pos[].x и object_pos[].z
	// Такие чтоб cross(object_pos[].x, object_pos[].z) давал нормаль поверхности
	// Нужно для локоновских теней
	io::mmfvector<osg::Vec3d> surfaceY_xz;
};

// структура
struct DistrictInstanceDynamic
{
	DistrictInstance* districtInstance;

	// удаленные домики квартала
	ed::vector<uint32_t> deletedBuildings;


	// Это какаято санина отсебятина для отрисовки.
	// Здесь ей не место

	// Чтоб можно было для разных кварталов корректировать дальность появления
	double distanceFactor;
	// текстура с позициями квартала
	void* districtPositionTexture;
	// начальный индекс
	int offsetIndex;
	// указатель на массив смещений текстуры
	ed::vector<int32_t> *districtOffset;

	DistrictInstanceDynamic() : districtInstance(NULL), distanceFactor(1.0), districtPositionTexture(NULL) {}
};

// Файл с кварталами
class EDTERRAIN_API lDistrictsSceneFile
{
public:
	// все кварталы и соответствующие динамические инстансы
	ed::vector<DistrictInstance> districts;
	ed::vector<DistrictInstanceDynamic> dynamicDistricts;

	// В a10 каждый домик имеет свой уникальный id = <номер-квартала-этого-домика> * district_stripeId + <номер-этого-домка-в-квартале>
	// сообственно district_stripeId - максимальное число домиков в квартале
	int district_stripeId;

	// Названия типов кварталов (по именам референсных текстур)
	ed::vector<ed::string> district_types;

	// Названия типов домиков
	ed::vector<ed::string> building_types;

	// Боксы для типов домиков
	ed::vector<osg::BoundingBox> building_type_boxes;

	//////////////////////////////////////////////////
	// Дерево
	StaticSpatial districtSpatial;

	// Дымы. Хранит id отдельных домов
	StaticSpatial smokesSpatial;

	// границы области
	intpoint sqmin, sqmax;
	// Массив квадратов (указателей)
	ed::vector<io::mmfvector<uint32_t> > squarepool;
	// Массив квадратов размерность X*Z (лежит номер в куче squarepool)
	ed::vector<int32_t> pSquares;

	// NON SERIALIZABLE
	// Временные данные для рендера

	// загруженые модели кварталов
	ed::vector<scene3::lBlockFile*> loaded_district_types;

	// загруженые модели строений
#ifdef EDGE
	ed::vector<model::IGraphModel*> loaded_building_types;
#else
	ed::vector<model::IModel*> loaded_models_types;
	ed::vector<wShape*> loaded_building_types;
#endif

	// кеширование выборки. чтоб не возвращала дубликаты
	int currentquery;
	ed::vector< int> objectqueries;

	lDistrictsSceneFile();

	// построить файл
	// на входе:
	//	  список кварталов, начальный id
	void Build(
		int startId,
		ed::list<DistrictInstance>& districts,
		ed::vector< ed::string>& district_types,	// список типов кварталов (по именам референсных текстур)
		ed::vector< ed::string>& building_types	// типы домиков
	);
public:
	// Загрузить сцену с кварталами
	bool Load(io::MmfStream& mmfStream);

	// Загрузить сцену с кварталами из файла
	// Загрузка происходит не в Memory Mapped режиме
	bool Load(const char* filename)
	{
		io::MmfStream mmfStream;
		lDistrictsSceneFile sceneFile;

		if(!mmfStream.Open(filename, io::MmfStream::READ))
			return false;

		if(!sceneFile.Load(mmfStream))
			return false;

		*this = sceneFile;

		return true;
	}

	// Сохранить
	bool Save(const char* filename);
	bool Serialize(io::Stream& file);

	// Доступ
public:
	// максимальный id домика
	uint32_t getMaxObjectId(
		);
	// номер квартала по ид квартала или домика в этом квартале
	uint32_t getDistrictById(
		uint32_t id,
		uint32_t& objectindex		// номер дома в квартале+1, если 0 то это сам квартал
		);
	// id домика по номеру квартала и номеру дома в нем
	uint32_t getObjectId(
		uint32_t districtid,
		uint32_t objectindex		// номер дома в квартале+1, если 0 то это сам квартал
		);

	//==============================================================================
	// Получить индексы кварталов в этом квадрате
	//   squareIndex - индекс квадрата
	//   [out]count  - количество кварталов в этом квадрате
	//   [return]    - поинтер на индексы кварталов в данном квадрате
	//==============================================================================
	/* deprecated */uint32_t* getDistrictsBySquare(intpoint squareIndex, int& count);
	uint32_t* getDistrictsBySquare(const osg::Vec2i& squareIndex, int& count);

	// кварталы по объему
	int QueryDistrict(
	    const osg::Vec3d& p1, const osg::Vec3d& p2, 
		IDistrictQuerySink* pQuerySink=0
	);
	int QueryDistrict(
		const osg::Polytope& polytope,
		IDistrictQuerySink* pQuerySink=0
	);
	int QueryDistrict(
		const osg::BoundingBoxf& boundingbox,
		IDistrictQuerySink* pQuerySink = 0
		);
	int QueryDistrict(
		const osg::Vec3d& center, double radius, 	// Сфера
		IDistrictQuerySink* pQuerySink=0
	);
	// строения по объему
	int QueryObjects(
		const osg::Polytope& polytope,
		ed::vector< std::pair<int, int> >& objects		// pair< district, objectindistrict>
	);
	// Query Smokes
	int QuerySmokes(
		const osg::Polytope& polytope,
		IQuerySink* pQuerySink=0
		);

protected:
	// рекурсия. Запрос кварталов
	int QueryDistrict_rec(
		StaticSpatial::TreeNode* treenode,
	    const osg::Vec3d& p1, const osg::Vec3d& p2, 
		IDistrictQuerySink* pQuerySink=0
		);
	int QueryDistrict_rec(
		StaticSpatial::TreeNode* treenode,
		const osg::Polytope& polytope,
		IDistrictQuerySink* pQuerySink=0
		);
	int QueryDistrict_rec(
		StaticSpatial::TreeNode* treenode,
		const osg::BoundingBoxf& boundingbox,
		IDistrictQuerySink* pQuerySink = 0
		);
	int QueryDistrict_rec(
		StaticSpatial::TreeNode* treenode,
		const osg::Vec3d& center, double radius, 	// Сфера
		IDistrictQuerySink* pQuerySink=0
		);
	int QueryObject_rec(
		StaticSpatial::TreeNode* treenode,
		const osg::Polytope& polytope,
		IQuerySink* pQuerySink, 
		StaticSpatial& spatial
		);
};
}

// максимальный id домика
inline uint32_t scene3::lDistrictsSceneFile::getMaxObjectId(
)
{
	uint32_t count = districts.size()+1;
	return district_stripeId * count;
}

// квартал по id домика или квартала
inline uint32_t scene3::lDistrictsSceneFile::getDistrictById(
	uint32_t id,
	uint32_t& objectindex		// номер дома в квартале+1, если 0 то это сам квартал
)
{
	int districtindex = (id)/this->district_stripeId;
	objectindex = id - (districtindex*this->district_stripeId);

	if(districtindex<0 || districtindex>=(int)this->districts.size())
		return -1;

	return districtindex;
}
// id домика по номеру квартала и номеру дома в нем
inline uint32_t scene3::lDistrictsSceneFile::getObjectId(
	uint32_t districtid,
	uint32_t objectindex
	)
{
	uint32_t id = districtid * this->district_stripeId + objectindex;
	return id;
}

// кврталы по номеру квадрата - ипользовать osg-версию
/* deprecated */inline uint32_t* scene3::lDistrictsSceneFile::getDistrictsBySquare(intpoint squareIndex, int& count)
/* deprecated */
{
	/* deprecated */	count = 0;
	/* deprecated */	if(squareIndex.x < sqmin.x || squareIndex.x >= sqmax.x) return NULL;
	/* deprecated */	if(squareIndex.z < sqmin.z || squareIndex.z >= sqmax.z) return NULL;
	/* deprecated */
	/* deprecated */	int i = (squareIndex.x - sqmin.x) * (sqmax.z - sqmin.z) + (squareIndex.z - sqmin.z);
	/* deprecated */	int32_t index = pSquares[i];
	/* deprecated */	if(index < 0)
		/* deprecated */		return 0;
	/* deprecated */	if(index >= (int)squarepool.size())
		/* deprecated */		return 0;
	/* deprecated */
	/* deprecated */	io::mmfvector<uint32_t>& list = squarepool[index];
	/* deprecated */	if(list.empty())
		/* deprecated */		return 0;
	/* deprecated */	count = list.size();
	/* deprecated */	return list.ptr();
	/* deprecated */
}

inline uint32_t* scene3::lDistrictsSceneFile::getDistrictsBySquare(const osg::Vec2i& squareIndex, int& count)
{
	count = 0;

	// Проверяем попадает ли вообще этот квадрат в нашу область
	if(squareIndex.x() < sqmin.x || squareIndex.x() >= sqmax.x) return NULL;
	if(squareIndex.y() < sqmin.z || squareIndex.y() >= sqmax.z) return NULL;

	// squareIndex1D - одномерный индекс квадрата для обращения к pSquares
	int squareIndex1D = (squareIndex.x() - sqmin.x) * (sqmax.z - sqmin.z) + (squareIndex.y() - sqmin.z);
	int32_t index = pSquares[squareIndex1D];

	// Проверяем есть ли такой квадрат
	if(index < 0 || index >= (int)squarepool.size()) return NULL;

	io::mmfvector<uint32_t>& list = squarepool[index];
	if(list.empty()) return NULL;

	count = list.size();
	return list.ptr();
}

#endif
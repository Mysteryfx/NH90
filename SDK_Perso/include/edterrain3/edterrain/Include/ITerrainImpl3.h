#ifndef ED_ITerrainImpl3_H
#define ED_ITerrainImpl3_H

#include "ed/unordered_map.h"
#include "edTerrain/ITerrain.h"
#include "IRoadNetworkImpl3.h"
#include "lTerraDispatch.h"
#include "landscape3/lSceneFile.h"
#include "landscape3/lDistrictsSceneFile.h"
#include "edModel/IModel.h"
#include "edModel/ModelEntryPoint.h"


 namespace edterrain {
 	class ITerrainContext;
 }
using namespace edterrain;


class EDTERRAIN_API ITerrainImpl3 : public edterrain::ITerrain
{
	lTerraDispatch* terrain;
	ed::unordered_map<std::string, IRoadNetworkImpl3*> roadNetworks;
public:
	ITerrainImpl3(lTerraDispatch* terrain);
	~ITerrainImpl3();

	lTerraDispatch* getDispatcher(){return terrain;}
public:

	// удалить
	virtual void release();

	// openTerrain
	virtual bool openTerrain(const char* filename, lua_State* options);

// TerrainContext
// Все изменения в сценах, дорожных сетях и прочем содержаться в нем
public:
	// createContext
	virtual ITerrainContext* createContext() const;
	// setCurrentContext
	virtual void setCurrentContext(ITerrainContext*);
	// getCurrentContext
	virtual ITerrainContext* getCurrentContext() const;

public:
	virtual void init(const char* category);
	// Получение конфига земли
	virtual lua_State* getTerrainCfg();

	// Уровень моря
	virtual double getSeaLevel() const;

	// Получить приблизительную высоту по координате с учётом макс. высот всех площадок в квадрате со стороной dirtySize и центром (x,z)
	virtual double getDirtyHeight(double x, double z, double dirtySize);

	// Запрос высоты в точке. 
	// Вернет высоту точки на поверхности ниже точки query
	virtual edterrain::PointOnSurface getPointOnSurface(const osg::Vec3d& query)const;

	// Пересечение отрезка с поверхностью
	virtual edterrain::PointOnSurface intersection(const osg::Vec3d& pt1, const osg::Vec3d& pt2)const;

	virtual enQueryObjectsResult querySceneObjectsCB(const osg::Vec3d& pt1, const osg::Vec3d& pt2, uint32_t mask, OnQuerySceneObjectNdata_t callback)const;
	virtual enQueryObjectsResult querySceneObjectsCB(const osg::Polytope& polytope, uint32_t mask, OnQuerySceneObject_t callback) const;
	virtual enQueryObjectsResult querySceneObjectsCB(const osg::BoundingBoxf& box, uint32_t mask, OnQuerySceneObject_t callback) const;

	// матрица
	virtual osg::Matrixd getSceneObjectPosition(SceneObjectId objectId)const;
	// тип
	virtual model::IModel* getSceneObjectType(SceneObjectId objectId)const;

	// convert x,y in UTM coords(x-east, y-north), to lat,lon (in degrees, N+, E+)
	virtual void toLatLon(double x, double y, double& lat, double& lon)const;
	// convert lat,lon (degrees, N+, E+) to UTM coords
	virtual void fromLatLon(double lat, double lon, double& x, double& y)const;
	// convert lockon to MGRS
	virtual ed::string fromLockonToMGRS(double lockon_z, double lockon_x, int prec=5)const;
	// convert from UTM to lockon
	virtual osg::Vec3d fromUTMtoLockon( const osg::Vec2d& utm)const;

	virtual IRoadNetwork* getRoadNetwork(const char* name);
	
// Утилиты
public:
	// Имя семантики
	virtual ed::string getSemanticName(int semantic);

protected:
	mutable model::ModelEntryPoint<model::IModelManager> edmEntryPoint;
	mutable model::ModelEntryPoint<model::IModelManager> edterrainEntryPoint;

	// storeIModel
	model::IModel* ITerrainImpl3::storeIModel(model::IModel* &pModel, const ed::string& tname) const;

// id utils
	mutable ITerrainContext* mCurrentContext;
protected:
	SceneObjectId makeSceneObjectId(uint32_t localid) const;
	SceneObjectId makeDistricsObjectId(uint32_t district, uint32_t localid) const;
	bool parseDistricsObjectId(SceneObjectId id, uint32_t& district, uint32_t& localid) const;
	bool parseSceneObjectId(SceneObjectId id, uint32_t& localid) const;

};

#endif

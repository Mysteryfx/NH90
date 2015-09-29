#ifndef __ITERRAIN_H__
#define __ITERRAIN_H__

#include "../inter.h"
#include "stdint.h"
#include <osg/Vec3d>
#include <osg/Polytope>
#include <osg/Matrixd>
#include <functional>
#include <xrefwrap>
#include "ed/string.h"

struct lua_State;
namespace model
{ 
	class IModel;
}

class ITerrainImpl3;
class ITerrainImpl4;

namespace edterrain
{

class ITerrain;
class ITerrainContext;
class IRoadNetwork;
class IRoute;


/*
 * ������� ������ ��� �����, ��������� ����� release().
 * Usage:
	// with Scoped
  	{
		edterrain::Scoped<edterrain::ICrossroad> cross = rn->getCrossroad(i);
	}
	// without Scoped
	{
		auto cross = rn->getCrossroad(i);
		cross->release();
	}
 */
template <class T>
class Scoped
{
public:
	Scoped(T* t)
		:	t(t)
	{}

	~Scoped()
	{
		if (t)
			t->release();
		t = nullptr;
	}

	operator T*()
	{
		return t;
	}

	T* operator->()
	{
		return t;
	}

private:
	T *t;

	Scoped(const Scoped<T>& t);
	Scoped<T>& operator=(const Scoped<T>& t);
};


enum enSurfaceTypes
{
	ST_LAND   = 0x0001,
	ST_RUNWAY = 0x0004,
	
	ST_SEA    = 0x0010,
	ST_RIVER  = 0x0020,
	ST_LAKE   = 0x0040,
	ST_WATER  = 0x00f0,

	ST_ROAD   = 0x0100,
	ST_RAILWAY= 0x0200,
	ST_TOWN   = 0x0400,
	ST_FIELD  = 0x0800,
	ST_BEACH  = 0x1000,
	ST_MAX,
};

inline const char* getSurfaceTypeName(uint64_t surfaceType);
inline ed::string getSurfaceTypesName(uint64_t surfaceTypes);

// PointOnSurface
struct PointOnSurface
{
	PointOnSurface();
	
	// ��?
	bool isValid() const;

	// ������ ������
	void set(const osg::Vec3d& query); // Y ����������� ��� ��������� ����� ����
	void set(double x, double z);

	// �����
	double height() const;
	// �����
	osg::Vec3d point() const;
	// �������
	osg::Vec3d normal() const;

	// ����� �����, ��. enSurfaceTypes 
	uint64_t types() const;
	// hasType, ��. enSurfaceTypes 
	bool hasType(uint64_t mask) const;

	// helpers
	bool isWater() const{return hasType(ST_WATER);}
	bool isRoad() const{return hasType(ST_ROAD);}
	bool isRunway() const{return hasType(ST_RUNWAY);}

protected:
	const ITerrain* terrain;
	osg::Vec3d p, n;
	uint64_t t;
	friend class ITerrain;
	friend class ITerrainImpl3;
	friend class ITerrainImpl4;
};

// id ������� �����
typedef uint32_t SceneObjectId;

// ��������� SceneObjectId
const SceneObjectId startSceneObjectId = 0x4300000;

// ���������� ���������� ��������
enum class enQueryObjectBehavior
{
	CONTINUE,		// ���������� �������
	FINISH			// �������� �������
};

enum class enQueryObjectsResult
{
	CANCELED,		// ���������� ������ QOB_FINISH
	COMPLETED		// ��� ������ ����������
};

// ����� ������� �������� �����
enum enQueryObjectsMask
{
	QOM_SHELL = 0x80000000,			// �������� collision shell, ��� ���� ������ �����
	QOM_SUBOBJECTS = 0x40000000,	// �������� �����������
	QOM_ALL   = 0xffffffff,
};

// ���������� � ����������� ������� � �������
struct ModelIntersectionData
{
	// ������. ����� ��� �����
	model::IModel* model;
	// ������� ������
	osg::Matrixd* matrix;
	// ����� �����������
	osg::Vec3d point;
	// ������� � ����� ����������� ������ � ��������
	osg::Vec3d normal;
};

// ���� �� ����� ��� ��������� ����������
const double ITerrainVersion = 0.0;

class ITerrain
{
public:
	// RequiredTerrainDll
	static inline ed::string RequiredTerrainDll(const char* filename);
	// SetTerrain
	static INTER_API void SetDefaultTerrain(ITerrain* terrain);
	// GetTerrain
	static INTER_API ITerrain* GetDefaultTerrain();
public:
	// openTerrain
	virtual bool openTerrain(const char* filename, lua_State* options) = 0;

// TerrainContext
// ��� ��������� � ������, �������� ����� � ������ ����������� � ���
public:
	// createContext
	virtual ITerrainContext* createContext() const = 0;
	// setCurrentContext
	virtual void setCurrentContext(ITerrainContext*) = 0;
	// getCurrentContext
	virtual ITerrainContext* getCurrentContext() const = 0;

public:
	// ~
	virtual ~ITerrain(){};
	// �������
	virtual void release(){};

public:
	// ��������� ������� �����
	virtual lua_State* getTerrainCfg() = 0;

	// ������� ����
	virtual double getSeaLevel() const = 0;

	// �������������, �������
	// �������� "*" ���� ����� ���
	virtual void init(const char* category) = 0;

	// �������� ��������������� ������ �� ���������� � ������ ����. ����� ���� �������� � �������� �� �������� dirtySize � ������� (x,z)
	virtual double getDirtyHeight(double x, double z, double dirtySize)=0;

	// ������ ������ � �����. 
	// ������ ������ ����� �� ����������� ���� ����� query
	virtual PointOnSurface getPointOnSurface(const osg::Vec3d& query)const = 0;

	// ����������� ������� � ������������
	virtual PointOnSurface intersection(const osg::Vec3d& pt1, const osg::Vec3d& pt2)const = 0;

	// ���� ����������� ����� �������
	virtual int envelope(const osg::Vec3d& pt1, const osg::Vec3d& pt2, osg::Vec3f* dest, int destMaxCount) const{return 0;}

	// ������ �������� �����
	// mask - ����� ����� enQueryObjectsMask
	// data - �� null ���� ������� ���� QOM_SHELL
	// usage:
	// pTerrain->querySceneObjects(p1, p2, QOM_ALL, [&](SceneObjectId id, ModelIntersectionData* data)
	// {
	//     pTerrain->getSceneObjectPosition();
	//	   return enQueryObjectBehavior::CONTINUE;
	// });
	template <class FUNC>
	enQueryObjectsResult querySceneObjects(const osg::Vec3d& pt1, const osg::Vec3d& pt2, uint32_t mask, FUNC&& func)const
	{
		return this->querySceneObjectsCB(pt1, pt2, mask, std::ref(func));
	}
	template <class FUNC>
	enQueryObjectsResult querySceneObjects(const osg::Polytope& polytope, uint32_t mask, FUNC&& func)const
	{
		return this->querySceneObjectsCB(polytope, mask, std::ref(func));
	}
	template <class FUNC>
	enQueryObjectsResult querySceneObjects(const osg::BoundingBoxf& box, uint32_t mask, FUNC&& func)const
	{
		return this->querySceneObjectsCB(box, mask, std::ref(func));
	}
	
protected:
	typedef std::function<enQueryObjectBehavior(SceneObjectId objectId)> OnQuerySceneObject_t;
	typedef std::function<enQueryObjectBehavior(SceneObjectId objectId, ModelIntersectionData* data)> OnQuerySceneObjectNdata_t;

	// querySceneObjectsCB
	virtual enQueryObjectsResult querySceneObjectsCB(const osg::Vec3d& pt1, const osg::Vec3d& pt2, uint32_t mask, OnQuerySceneObjectNdata_t callback)const=0;
	virtual enQueryObjectsResult querySceneObjectsCB(const osg::Polytope& polytope, uint32_t mask, OnQuerySceneObject_t callback)const =  0;
	virtual enQueryObjectsResult querySceneObjectsCB(const osg::BoundingBoxf& box, uint32_t mask, OnQuerySceneObject_t callback)const =  0;


// ���������� �� ������� �����
public:
	// �������
	virtual osg::Matrixd getSceneObjectPosition(SceneObjectId objectId)const=0;
	// ���
	virtual model::IModel* getSceneObjectType(SceneObjectId objectId)const=0;

// �������������� ���������
public:

	// convert x,y in UTM coords(x-east, y-north), to lat,lon (in degrees, N+, E+)
	virtual void toLatLon(double x, double y, double& lat, double& lon)const{}
	// convert lat,lon (degrees, N+, E+) to UTM coords
	virtual void fromLatLon(double lat, double lon, double& x, double& y)const{}
	// convert lockon to MGRS
	virtual ed::string fromLockonToMGRS(double lockon_z, double lockon_x, int prec=5)const{return "";}
	// convert from UTM to lockon
	virtual osg::Vec3d fromUTMtoLockon( const osg::Vec2d& utm)const{return osg::Vec3d();}

// �������� ����, �������, ����� �����
public:
	// "roads", "railroads", "airfield1", ...
	// ������ �������� ���� ��� ������ �����
	virtual IRoadNetwork* getRoadNetwork(const char* name){return nullptr;};

	// ������ ���� �� ����������� � �������������
	virtual bool findPath(const osg::Vec3d& pt1, const osg::Vec3d& pt2, IRoute& resultRoute, double maxSlope, double maxShallowDepth){return false;};

// �������
public:
	// ��� ���������
	virtual ed::string getSemanticName(int semantic) = 0;
};

// ������� ��� �������� terrainX.dll � ������� � ITerrain
class ITerrainEntryPoint
{
public:
	ITerrainEntryPoint();
	~ITerrainEntryPoint();

public:
	ITerrain* openTerrain(const char* filename, lua_State* options=nullptr);
	edterrain::ITerrain* instance();
	void close();

private:
	bool providerLoaded;
	void* provider_dlhandle;
	edterrain::ITerrain* terrain;
};


}

#include "inl/ITerrain.inl"
#include "inl/ITerrainEntryPoint.inl"

#endif //__ITERRAIN_H__

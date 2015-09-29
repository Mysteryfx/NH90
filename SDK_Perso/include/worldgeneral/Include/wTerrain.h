#pragma once

#include "cLinear.h"
#include "cPosition.h"
#include "wMovingObject.h"

#define NEW_ROADS

class lLandObject;
class gShapeObject;
class MapObjectsStorage;

// Типы позиций размещения объектов на местности
const int lLandscapePosTypeVertical		= 1;
const int lLandscapePosTypeNormal		= 2;

// Значения семантик земных поверхностей
const int lLandLand			= 0;
const int lLandSea			= 1;
const int lLandRoad			= 2;
const int lLandBeach		= 3;
const int lLandRiver		= 4;
const int lLandTown			= 5;
const int lLandAirfield		= 6;
const int lLandLake			= 7;
const int lLandIsland		= 8;
const int lLandRailway		= 9;
const int lLandReservoir	= 10;
const int lLandSettlement	= 11;
const int lLandLocality		= 12;
const int lLandShosse		= 13;
const int lLandAuto			= 14;
const int lLandChannel		= 15;
const int lLandBluff		= 16;

const int LAND_SEMANTICS_MAX	= 16;

const unsigned int flagMeshLand			= 0x00001;
const unsigned int flagMeshSea			= 0x00002;
const unsigned int flagMeshRoad			= 0x00004;
const unsigned int flagMeshBeach		= 0x00008;
const unsigned int flagMeshRiver		= 0x00010;
const unsigned int flagMeshTown			= 0x00020;
const unsigned int flagMeshAirfield		= 0x00040;
const unsigned int flagMeshLake			= 0x00080;
const unsigned int flagMeshIsland		= 0x00100;
const unsigned int flagMeshRailway		= 0x00200;
const unsigned int flagMeshReservoir	= 0x00400;
const unsigned int flagMeshSettlement	= 0x00800;
const unsigned int flagMeshLocality		= 0x01000;
const unsigned int flagMeshShosse		= 0x02000;
const unsigned int flagMeshAuto			= 0x04000;
const unsigned int flagMeshChannel		= 0x08000;
const unsigned int flagMeshBluff		= 0x10000;

class wTerrain 
{
public:
	virtual	~wTerrain() = 0 {}
	virtual	int			Init(const char* terraluacfg) = 0;
	virtual int			Exit() = 0;
	virtual int			Load(const ed::string& confing) = 0;
//	virtual	void		InitScenes() = 0;
//	virtual	void		CleanScenes() = 0;

//    virtual	float		heightBox(float x, float z) = 0;
	virtual	float		heightSurface(float x, float z) = 0;
	virtual float		getDirtyHeight(float x, float z, float dirtySize) = 0;

    // программа определяет, на аэродроме ли находится точка с координатами (xW, zW) ?
	virtual	bool		IsOnAirfield(float xW, float zW) = 0;

    // программа определяет, на дороге ли находится точка с координатами (xW, zW) ?
    virtual	bool        IsOnRoad(float xW, float zW) = 0;

	//
	virtual bool		IsInCity(float x, float z) = 0;

    // функция определяет тип треугольника под точкой
	virtual	unsigned char   surfType(float xW, float zW) = 0;
    // нормаль под точкой
	virtual cVector   surfNormal(float xW, float zW) = 0;

    // функция определяет тип треугольника под точкой c учетом накладных озер
	virtual	unsigned char surfTypePlus(float xW, float zW) = 0;
	virtual	int surfaceTrianglePlus(float x, float z, cPoint &p, cVector &n) = 0;
    
    // программа определяет, на накладном ли озере находится точка с координатами (xW, zW) ?
	virtual	bool IsOnLake(float xW, float zW) = 0;

	// 
	// !!! WARNING: OSG version of this procedures is in class lPointOnSurface
	// 
	// see lPointOnSurface::VectorGroundIntersection
	//     lPointOnSurface::groundIntersection
	//     lPointOnSurface::isVisible
	// 
	virtual bool	VectorGroundIntersection(const cPoint &Pnt, const cVector &Vec, float MaxDist, cPoint & point) = 0;
	virtual bool	groundIntersection(const cPoint &from, const cVector &to, cPoint & point) = 0;

    // flags - это затычка, позволяющая функции heightSurface(), игнорируя
	// подсунутое под землю море, найти высоту по первому попавшемуся треугольнику.
	// Пока никому больше не рекомендуется использовать этот опасный параметр.
	virtual	int surfaceTriangle(float x, float z, cPoint &p, cVector &n, unsigned int flags = 0) = 0;
	virtual	bool		isVisible(const cVector & from, const cVector & to) = 0;
	virtual	int			Envelope(const cPoint & from, const cPoint &to, cPoint *lEnv, int lMax) = 0;

    virtual	int GetWObjects(cPoint &p, float r, woPointer *lw, int lMax, unsigned int classes = 0xFFFFFFFF) = 0;

    // вызывается в NextTime мира пока только для изменения стационарных объектов
	virtual	void		NextTime(float dtime) = 0;

	// Размер квадрата по уровню детализации
	virtual int GetLevelSize(int level) = 0;

	// convert x,y in UTM coords(x-east, y-north), to lat,lon (in degrees, N+, E+)
	virtual void toLatLon(double x, double y, double& lat, double& lon) const = 0;
	// convert lat,lon (degrees, N+, E+) to UTM coords
	virtual void fromLatLon(double lat, double lon, double& x, double& y) const = 0;

	virtual lua_State*	getTerrainCfg() = 0;
};

extern WORLDGENERAL_API wTerrain *globalLand;


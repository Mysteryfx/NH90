#pragma once

#include "Terrain.h"
#include "wTerrain.h"
#include "lSurface.h"
#include "cLinear.h"
#include "cPosition.h"
//#include "wMovingObject.h"
#include "edTerrain/ITerrain.h"

#ifndef USE_TERRAIN4
#include "lTerraDispatch.h"
#include "Resourcer\ResourceManager.h"
#include "smSceneManager.h"
#include "viObjectManager.h"
#endif

#define NEW_ROADS

class lLandObject;

class TERRAIN_API lSystem : public wTerrain
{
public:
	lSystem();
	virtual ~lSystem();

	virtual int			Init(const char* terraluacfg);
	virtual int			Exit();

	virtual int			SwitchSeason(const char* season);

	virtual int			Load(const ed::string& terrainConfig);

//    virtual float		heightBox(float x, float z);
	virtual float		heightSurface(float x, float z);

	virtual float		getDirtyHeight(float x, float z, float dirtySize = 1000);
	//virtual float		getDirtyHeight2(float x, float z, float dirtySize = 1000) = 0;

    // программа определяет, на аэродроме ли находится точка с координатами (xW, zW) ?
	virtual bool		IsOnAirfield(float xW, float zW);
    
    // программа определяет, на дороге ли находится точка с координатами (xW, zW) ?
    virtual bool        IsOnRoad(float xW, float zW);
        
    // программа определяет, на накладном ли озере находится точка с координатами (xW, zW) ?
    virtual bool        IsOnLake(float xW, float zW);

	virtual bool		IsInCity(float x, float z);

    // функция определяет тип треугольника под точкой
	virtual unsigned char   surfType(float xW, float zW);
    
    // нормаль под точкой
	virtual cVector   surfNormal(float xW, float zW);

    // функция определяет тип треугольника под точкой c учетом накладных озер
	virtual unsigned char surfTypePlus(float xW, float zW);
	virtual int surfaceTrianglePlus(float x, float z, cPoint &p, cVector &n);

    virtual bool	VectorGroundIntersection(const cPoint &Pnt, const cVector &Vec, float MaxDist, cPoint & point);
	virtual bool	groundIntersection(const cVector &from, const cVector &to, cPoint & point);

    // flags - это затычка, позволяющая функции heightSurface(), игнорируя
	// подсунутое под землю море, найти высоту по первому попавшемуся треугольнику.
	// Пока никому больше не рекомендуется использовать этот опасный параметр.
	virtual int surfaceTriangle(float x, float z, cPoint &p, cVector &n, unsigned int flags = 0);
	virtual bool		isVisible(const cPoint	&from, const cPoint &to);
	virtual int			Envelope(const cPoint &from, const cPoint &to, cPoint *lEnv, int lMax);

    virtual int GetWObjects(cPoint &p, float r, woPointer *lw, int lMax, unsigned int classes = 0xFFFFFFFF);

    // вызывается в NextTime мира пока только для изменения стационарных объектов
	virtual void		NextTime(float dtime);

	// Размер квадрата по уровню детализации
	virtual int GetLevelSize(int level);

	// convert x,y in UTM coords(x-east, y-north), to lat,lon (in degrees, N+, E+)
	virtual void toLatLon(double x, double y, double& lat, double& lon) const;
	// convert lat,lon (degrees, N+, E+) to UTM coords
	virtual void fromLatLon(double lat, double lon, double& x, double& y) const;

	virtual lua_State*	getTerrainCfg();
protected:
	/*/
   	// Квадрат
	nl::lSquare* getSquare(intpoint squareindex)
	{
		return (nl::lSquare*)&pSurface->GetSquare(squareindex);
	}

    // Регион
	nl::lRegion* getRegion(intpoint squareindex)
	{
		return (nl::lRegion*)&pSurface->GetRegion(squareindex);
	}
	/*/

private:
	edterrain::ITerrainEntryPoint terrainEntryPoint;
	

#ifndef USE_TERRAIN4
	landscape::lSurface* pSurface;
	landscape::lSurfaceDataManager* pSurfaceDataManager;
#endif

	ed::string terraluacfg;
	bool isInited;

	// смонтированые каталоги
	ed::list< ed::string> mounteddirs;

    bool InitSurface();

	virtual void		InitScenes();
	virtual void		CleanScenes();
public:
	// Чтение файла terrain.cfg.lua
	// И загрузка земли, накладных, дорог, сцены
	bool LoadTerrainCfg(
		const char* directoryOrFile, 
		const char* season, 
		const char* quality, 
		const char* textureQuality,
		const char* lang
#ifndef USE_TERRAIN4
		,lTerraDispatch* pTerraDispatch, 
		landscape::lSurfaceDataManager* pSurfaceDataManager,
		ResourceManager* resourcer, 
		viObjectManager* pObjectManager, 
		smSceneManager* visualizer
#endif
		);

private:
#ifdef USE_TERRAIN4
	void addShapetableToObjectManager();
#endif

protected:
	void read_airfield_scenery_data(
#ifndef USE_TERRAIN4
		lTerraDispatch    * pTerraDispatch,
#endif
		Lua::Config       & setup);

};


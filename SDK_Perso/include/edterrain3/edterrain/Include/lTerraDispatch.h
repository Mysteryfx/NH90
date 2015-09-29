#pragma once
#include <osg/Vec2i>
#include <osg/Vec3d>
#include <osg/Vec2d>
#include "edTerrain.h"
#include "lTerraBasic.h"
#include "lSurface.h"
#include <ed/list.h>
#include <ed/vector.h>
#include <ed/map.h>
#include <ed/string.h>

struct CMGRS
{
	unsigned char zone;
	char zoneLetter,MGRSLetter1,MGRSLetter2;
	int easting,northing,precision;

	CMGRS(const unsigned char zone, const char zoneLetter, const char MGRSLetter1, const char MGRSLetter2, const int northing, const int easting, const int precision=5)
	{
		this->zone=zone;
		this->zoneLetter=zoneLetter;
		this->MGRSLetter1=MGRSLetter1;
		this->MGRSLetter2=MGRSLetter2;
		this->northing=northing;
		this->easting=easting;
		this->precision=precision;
	}

	/*inline char* toString()
	{
		char res[16];
		sprintf_s(res,"%d%c%c%c%0*d%0*d",zone,zoneLetter,MGRSLetter1,MGRSLetter2,precision,easting,precision,northing);
		return res;
	}*/
};


namespace landscape
{
	class lSurfaceDataManager;
	class lSurface;
}
namespace scene3
{
	class lDistrictsSceneFile;
	struct DistrictInstance;
	struct DistrictInstanceDynamic;
	class lBlockFile;
	class lSceneFile;
}
namespace landscape4
{
	class lReferenceFile;
	class lSurfaceFile;
}
namespace utm_private
{
	struct projection_t;
}
namespace ed
{
	class ITerrainPlugin;
}

#include "ed_lua.h"

namespace offshore
{
	struct Mesh;
}
namespace path
{
	class PathNetwork;
}	
namespace edterrain
{
	class ITerrain;
}

// Управляющий объект для земли
class lTerraDispatch
{
// Загрузка
public:
	virtual bool Init(
		const char* terraincfg,
		const char* season, 
		const char* quality, 
		const char* lang,
		landscape::lSurfaceDataManager* pSurfaceDataManager		// атавизьм!!!
		)=0;
	virtual bool Init(
		const char* category
		)=0;
	virtual bool Exit(
		const char* category
		)=0;

// Функции доступа
public:
	// конфиг
	virtual lua_State* getTerrainCfg() = 0;

	// ITerrainPlugin
	virtual ed::ITerrainPlugin* GetTerrainPlugin() = 0;

	// описание географической проекции
	virtual const utm_private::projection_t& getProjection() const = 0;

	// convert x,y in UTM coords, to lat,lon (in degrees, N+, E+)
	virtual void toLatLon(double lockon_z, double lockon_x, double &lat, double &lon)=0;
	// convert lat,lon (degrees, N+, E+) to UTM coords
	virtual void fromLatLon(double lat, double lon, double &lockon_z, double &lockon_x)=0;
	// convert lockon to MGRS. prec - число знаков для числовой части координат
	virtual ed::string fromLockonToMGRS(double lockon_z, double lockon_x, int prec = 5)=0;

	virtual void MGRStoLL(const CMGRS* MGRS, double& latitude, double & longitude)=0;
	// convert MGRS -> LL

	// convert from UTM to lockon
	virtual osg::Vec3d fromUTMtoLockon( const osg::Vec2d& utm)=0;


	// Поверхность
	virtual landscape::lSurface* GetSurface()=0;
	virtual NM_SURFACE::lSurfaceFile* GetSurfaceFile()=0;
	virtual landscape4::lSurfaceFile* GetSurfaceFile4()=0;
	
	// Накладные объекты 2
	virtual ed::list<NM_SUPERFICIAL::lSuperficialFile*>& getSuperficialFiles2()=0;
	// Автомобильные дороги
	virtual ed::list<NM_ROAD::lRoadFile*>& GetAutoRoads()=0;
	// Аэродромы
	virtual ed::list<NM_ROAD::lRoadFile*>& GetAirRoads()=0;

	// Дорожные сети
	virtual path::PathNetwork* getAutoRoadNetwork()=0;
	virtual path::PathNetwork* getRailRoadNetwork()=0;
	virtual path::PathNetwork* getAirRoadNetwork(const char* taxiwayfilename)=0;		// не используется

	// Сцены
	virtual ed::list<scene3::lSceneFile>& getScenes() = 0;

	virtual scene3::lDistrictsSceneFile* getDistricts() = 0;

	// Block files
	virtual void getBlockFiles( ed::list<scene3::lBlockFile*>& blocks) = 0;
	// smallshit
	virtual scene3::lBlockFile* getSmallShitFile(const ed::string &name) = 0;

	virtual ed::list<landscape4::lReferenceFile>& getReferenceFile() = 0;

// Функции инициализации
public:
	// Создать поверхность
	virtual landscape::lSurface* CreateSurface()=0;

	// Добавить файл с накладными
	virtual void AddSuperficialFile(NM_SUPERFICIAL::lSuperficialFile* file)=0;
	// Добавить файл с дорожной сетью
	virtual void AddRoadNetworkFile(NM_ROAD::lRoadFile* file)=0;

	// Открыть файл с накладными
	virtual NM_SUPERFICIAL::lSuperficialFile* AddSuperficialFile(const char* filename, bool bMapFile=true)=0;
	// Открыть файл дорожной сети
	virtual bool AddRoadNetwork(const char* filename)=0;

	// Добавить файл сцены
	virtual scene3::lSceneFile* AddSceneFile(const char* filename)=0;
public:
	virtual void Preload(intpoint sqindex, int level)=0;

	// удалить все
	virtual void Release()=0;

// Более детальные ф-ции
public:
	// Квадрат
	virtual surface3::Square* getSquare(const osg::Vec2i& squareIndex, int level) = 0;
	/* deprecated */virtual surface3::Square* getSquare(intpoint squareindex, int level) = 0;

	// Высота квадрата
	virtual float getDirtyHeight(intpoint squareindex, int level) = 0;
	virtual float getDirtyHeight(const osg::Vec2i& squareIndex, int level) = 0;

	// кварталы в квадрате
	virtual void getSquareDistricts(intpoint squareindex, 
		ed::vector<scene3::DistrictInstance*>& districts, 
		ed::vector<scene3::lBlockFile*>& blocks) = 0;

	virtual void getSquareDistricts(const osg::Vec2i& squareIndex,
		ed::vector<scene3::DistrictInstance*>& districts,
		ed::vector<scene3::lBlockFile*>& blocks) = 0;

	virtual void getSquareDynamicDistricts(const osg::Vec2i& squareIndex,
		ed::vector<scene3::DistrictInstanceDynamic*>& districts,
		ed::vector<scene3::lBlockFile*>& blocks) = 0;

	virtual ed::string getShapeFilename(const ed::string& shapename) = 0;
// MITKA 16.09.2009
// Сервисные функции для получения геометрии земли и накладных 
public:
	// Область
	virtual bool getSurfaceBound(
		intpoint& min, 
		intpoint& max 
		)=0;
	// Поверхность (вертексы в WS)
	virtual bool getSurfaceOffshore( 
		intpoint squareindex, 
		ed::map<ed::string, offshore::Mesh>& meshbysemantic
		)=0;
	// Накладные по семантике (вертексы в WS)
	virtual bool getSuperficialOffshore( 
		intpoint squareindex, 
		const char* semantic, 
		offshore::Mesh& mesh
		)=0;
};

// Запрос диспетчера
EDTERRAIN_API lTerraDispatch* GetTerraDispatch();
// Создать диспетчер
EDTERRAIN_API lTerraDispatch* CreateTerraDispatch();
// Указать активный диспетчер
EDTERRAIN_API void SetTerraDispatch(lTerraDispatch*);
// Удалить диспетчер
EDTERRAIN_API void DeleteTerraDispatch(lTerraDispatch*);

// Временно:
EDTERRAIN_API edterrain::ITerrain* CreateITerrainStub(lTerraDispatch*);

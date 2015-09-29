#ifndef __ED_ITERRAINPLUGIN_H__
#define __ED_ITERRAINPLUGIN_H__

namespace ed
{

enum TERRAINTYPE
{
	TT_Land       = 0,  // Земля
	TT_Sea        = 1,  // Вода
	TT_Road       = 2,  // Дороги
	TT_River      = 4,  // Реки
	TT_Town       = 5,  // Города
	TT_Lake       = 7,
	TT_Railway    = 9,  // Ж.дороги
	TT_Field      = 22, // поля
	TT_Runway     = 23, // аэродром (впп)
};


// текущая версия интерфейса. Передается в getITerrainPlugin(). При несоответствии версии getITerrainPlugin() обязан вернуть 0.
const int ITerrainPlugin_version = 0x0000100;

class ITerrainPlugin
{
public:
	// Инициализация
	virtual bool init(const char* initstring)=0;
	// Удалить себя
	virtual void release()=0;

	// Получить высоту земли
	virtual double getHeight(const double* xyz) = 0;
	// Получить тип поверхности (см. TERRAINTYPE)
	virtual int getType(const double* xyz) = 0;
	// Получить нормаль поверхности
	virtual void getNormal(const double* xyz, double* normal) = 0;

	// Пересечение с отрезком
	virtual bool segIntersection(const double* P0, const double* P1, double* xyz) = 0;

	// Преобразование координат в географические
	virtual void toLatLon(const double* xyz, double& lat, double& lon)=0;
	// Преобразование географических координат в плоские метрические
	virtual void fromLatLon(double lat, double lon, double* xyz)=0;
};

// точка входа dll
typedef ITerrainPlugin* (*getITerrainPlugin_t)(int version);

};
#endif

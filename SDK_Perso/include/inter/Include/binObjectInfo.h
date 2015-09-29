#ifndef _BIN_OBJECT_INFO_H_
#define _BIN_OBJECT_INFO_H_

#include "binEffects.h"
#include "binWeather.h"
#include "binAtmosphere.h"

#include <osg/Matrixd>
#include <osg/Vec3d>

#include <ed/map.h>
#include <ed/vector.h>

//признак обновления данных. ключ уникален для каждого потока связи с IG - значение выставляется в случае обработки потоком
typedef ed::map<int, bool> tMapUpdateBool;

struct binParam
{
	binParam() : fVal( 0.0f )
	{};
	
	//признак обновления параметра
	tMapUpdateBool mUpdate;

	//значение параметра
	float fVal;
};

typedef ed::vector<binParam> tVecParam;
typedef ed::vector<float> tVecFloat;

struct binObjectInfo
{
	binObjectInfo() : uiID(0)
		, iHuman(0)
		, bExplode(false)
		, fFlowVelocity(0.0f)
		, bIsHuman(false)
		, iCount(0)
        , bDelete(false)
	{};

	unsigned int uiID;

	ed::string sName;
	osg::Matrixd Matr;
	int iHuman;
	bool bExplode;
    bool bDelete;
	bool bIsHuman;
	float fFlowVelocity;
	int iCount;
};

typedef ed::map<unsigned int, binObjectInfo> tMapObjectInfo;

//параметры анимации модели
typedef ed::map<unsigned int, tVecFloat> tMapFloat;

struct binOcean
{
	binOcean():fLambda(0.0f),
		fSquare(0.0f),
		fAlpha(0.0f),
		fWindBearing(0.0f),
		fWindVelocity(0.0f)
	{};

	float fLambda;			//отвечает за остроту волны(lambda)
	float fSquare;		//это сторона квадрата, в котором моделируется волнение (метры)(square_x,square_y)
	float fAlpha;			//отвечает за масштабирование волн, при одинаковом распределении(alpha)
	float fWindBearing;	//направление ветра (wind_x,wind_y)
	float fWindVelocity;	//скорость ветра(wind_x,wind_y)
};

struct binPlatform
{
	binPlatform()
	{};

	osg::Vec3f f3Pos;	//положение 0 платформы
	osg::Vec3f f3Rot;	//углы ориентации

	osg::Vec3f f3View;	//положения наблюдателя
};

struct binWorldInfo
{
	binWorldInfo() : dStartTime(-1.0)
		, dModelTime(0.0)
		, iSeason(1)
		, iIdModel(-1)
        , dAngle(0.0)
        , iMode(0)
		, bF11(false)
		, bBackspace(false)
		, iServer(0)
	{};

	//инфа о объектах сцены
	tMapObjectInfo mObjects;

	//инфа о эффектах для передачи
	tListEffects lEffects;

	//параметры анимации модели
	tMapFloat mParam;

	//данные о погоде
	binWeather Weather;

	//данне о состоянии атмосферы
	binAtmosphere Atmosphere;

	//работа с платформой
	binPlatform Platform;

	//данные для океана
	binOcean Ocean;

	//время старта миссии в секундах
	double dStartTime;

	//текущее время модели
	double dModelTime;

	//id сезона: 1-Лето, 2-Зима, 3-Весна, 4-Осень
	int iSeason;

	//смещение наблюдателя
	osg::Vec3d v3Offset;

	//направление ветра
	osg::Vec3d v3Wind;

	bool bF11;
	bool bBackspace;

	//id модели
	int iIdModel;

    //доворот сетки прицела
    double dAngle;

    //режим увеличения
    int iMode;

	// режимы работы сервера
	// 0-все кроме платформы
	// 1-только инфа для платформы
	// 2-инфа для платформы+все остальное
	// 3-настройки корректора
	int iServer;
};

#endif	//_BIN_OBJECT_INFO_H_

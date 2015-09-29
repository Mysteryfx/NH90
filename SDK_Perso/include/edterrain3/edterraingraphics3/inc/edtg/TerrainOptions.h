#ifndef EDTG_TERRAIN_OPTIONS_H
#define EDTG_TERRAIN_OPTIONS_H

#include "edtg.h"
#include "edtg/eDrawRoutines.h"
#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/Vec3d>
#include <osg/Vec4f>
#include <stdint.h>
#include "offshore/scene.h"
#include "landscape3/lSemantics.h"

namespace edtg
{
struct TerrainOptions;

// для указания какие дистанции пересчитывать
enum edRecalcDistancesFilter
{
	RDF_TERRAIN=1,
	RDF_ONLAY=2,
	RDF_DISTRICTS=4,
	RDF_UNIQUESCENE=8,
	RDF_DETAILS=16,
	RDF_ALL=0xFFFFFFFF
};


// пересчитать дистанции по множителю
EDTERRAINGRAPHICS_API void RecalcDistances(
	edtg::TerrainOptions& terrainOptions,
	float factor,   // 1 - текущие настройки
	edRecalcDistancesFilter filter = RDF_ALL
);

// пересчитать дистанции для текущего раствора камеры
EDTERRAINGRAPHICS_API void RecalcDistances(
	edtg::TerrainOptions& terrainOptions,
	float origin,   // эталонный раствор камеры (рад)
	float current,	// текущий раствор камеры (рад)
	edRecalcDistancesFilter filter = RDF_ALL
);

// прочитать TerrainOptions из луа файла
EDTERRAINGRAPHICS_API bool ReadTerrainOptions(
	Lua::Config& config,
	edtg::TerrainOptions& terrainOptions
);

// прочитать TerrainOptions.hiddensemantics из луа файла
EDTERRAINGRAPHICS_API void ReadHiddenSemantics(
	Lua::Config& config,
	edtg::TerrainOptions& terrainOptions
);

// пересчитать дистанции при недостаточном освещении
EDTERRAINGRAPHICS_API void RecalcDistancesAtNight(
	edtg::TerrainOptions& terrainOptions,
	float sunIntensity
);
}

namespace edtg
{
//#define _COPYCONSTRUCTORTEST
#ifdef _COPYCONSTRUCTORTEST
struct CopyBreakpoint
{
	int dummy;
	CopyBreakpoint()
	{
		dummy = 000;
	}
	CopyBreakpoint(const CopyBreakpoint& arg)
	{
		dummy = arg.dummy;
	}
	CopyBreakpoint& operator =(const CopyBreakpoint& arg)
	{
		dummy = arg.dummy;
		return *this;
	}
};
#else
	struct CopyBreakpoint
	{
	};
#endif

// настройки земли
//
// Все настройки земли лежат в этой структуре
// Материалы обращаются только к ней
struct TerrainOptions
{
	static const uint32_t packetType = 100;		// fmb::RT_TERRAIN
	static const uint32_t packetVersion = 1;
public:
	CopyBreakpoint copybreakpoint;
		
	edtg::eDrawRoutines drawRoutine;

	// Возможна отложеная загрузка
	bool bDelayLoadingAvailable;

	// пропустить парсинг сцены (рендерить старый кадр)
	bool bSkipParse;

	// отладочный дамп
	mutable uint32_t dumpFrame;
	mutable FILE* dumpOutput;
	mutable offshore::Scene* dumpScene;

	// модельное время
	float time;

	// wireframe
	bool wireframe;

	// направление и цвет солнца
//	osg::Vec3d sunDirectionWS;
	// Вращение вектора солнца вокруг оси OZ, чтоб normalmap не был серым, когда солце в зените
	// Мы как бы специально наклоняем солнце, чтоб оно не было строго вертикальным
//	osg::Vec3d sunBiasDirectionWS;
//	osg::Vec4f sunColor;
//	osg::Vec4f ambientColor;
	// ttf матрица наложения освещенки (сам считает)
	osg::Matrixf landlightttf;
	// аналогично луна
//	osg::Vec3d moonDirectionWS;
//	osg::Vec4f moonColor;

	//////////////////////////////////////////
	// Настройки рендеринга
	//
public:
	struct Instancer
	{
		int bufferSize;
		bool checkBufferSize;
		// debug
		int debugMode;
		// имя референся для тестов
		ed::string checkreference;
		Instancer():bufferSize(30000),checkBufferSize(false),debugMode(0){};
	};
	Instancer instancer;
	// Дистании
	struct Distance
	{
		// дистанция с которой видимы квадраты 10x10
		float distance10x10;
		float distanceLevel0;	// дистанция с которой видны квадраты уровня 0
		// Для карты (если -1 игнорируется)
		int levelformap;
		// EDGE: дистанция рендера уникальной застройки
		float uniqueSceneFarDistance;
		// EDGE: адаптивная дистанция до объектов уникальных сцен
		bool uniqueSceneAdaptiveDistance;
		// Минимальная площадь объекта на экране. Если меньше - не рендерим
		// Если площадь больше minimalSquareOnScreenFull рендерим целиком. Между - бленд
		float minimalSquareOnScreen, minimalSquareOnScreenFull;
		// текущая дальность smallShit, метров (от настроек и раствора камеры)
		float smallShitDimention;
		// дальность отрисовки дымов
		float smokesSceneFarDistance;

		// дальность отрисовки лодов для деталей
		float detailsDistances[16];

		// дальность рендера автомобилей
		float vehicles;
	};
	Distance distance;

	// mapLodDistance[0] - расстояние ближе которого в режиме карты будут рендериться квадраты нулевого лода
	// mapLodDistance[1] - расстояние ближе которого в режиме карты будут рендериться квадраты первого лода
	// и т.д. Максимальный лод - четвёртый
	float mapLodDistance[4];

	// Настройки шума
	float land_noisemax, land_noisemin;

	// проявление шума 1
	float land_noise1front, land_noise1back, land_noise1top, land_noise1bottom;

	// проявление шума 2
	float land_detailNoiseHeight, land_detailNoiseSlope;
	// Посторяемость шумов на 10км
	float land_noise1PerSquare, land_noise2PerSquare;

//	edcg::Fog fog;

	// фильтр по семантике
	bool hiddensemantics[typeMeshMAX];
	// фильтр по слоям
	bool hiddenlayer[50];
	// по уровням
	bool hiddenlevels[10];
	// по камере
	bool hiddencameras[2];
	// фильтр по очередям
	bool hiddenqueues[maxQueueCount];

	//Infrared options
	float infrared_landDetail;
	float infrared_landDarkness;
	float infrared_riverDarkness;
	float infrared_roadDarkness;
	float infrared_runwayDarkness;

	//Новый шум
	float noise_startDistance;			//Расстояние с которого начинает появляться шум
	float noise_endDistance;			//Расстояние на котором шум имеет максимальное значение
	float noise_maxBlend;				//Максимальный бленд шума
	float noise_scale;					//Тайлинг шума с расчёта на квадрат 10 км

	// Шум для рампы
	float rampNoise_power;
	float rampNoise_scale;

	//Детальный шум
	float smallNoise_startDistance;		//Расстояние с которого начинает появляться шум
	float smallNoise_endDistance;		//Расстояние на котором шум имеет максимальное значение
	float smallNoise_maxBlend;			//Максимальный бленд шума
	float smallNoise_scale;				//Тайлинг шума с расчёта на квадрат 10 км


	//Аэродромные фонари
	struct LampOptions
	{
		int lampOn;
		float maxSize;
		float staticSize;
		float spriteScale;
		float minDistance;
		float maxDistance;
		float maxAlphaDistance;
		float minAlphaDistance;
		float minAlpha;
		float maxAlpha;
		float minBrightnessDistance;
		float maxBrightnessDistance;
		float dsLightRadius;
		float dsLightBrightness;
	};

	LampOptions airfieldlamp;
	LampOptions lamp31;

	// тип рендера кварталов
	enum DistrictRenderType
	{
		DISTRICT_RENDER_SIMPLE,			// Обычный
		DISTRICT_RENDER_TEXTURE,		// Позиции передаются через текстуру
		DISTRICT_RENDER_INSTANCE		// через инстансы
	};

	// district ranges

	struct DistrictOptions
	{
		// максимальное число кварталов вокруг камеры
		int maxDistrictsAround;
		// Включить автоматический расчет дистанции через maxDistrictsAround
		int arrangeDistrictDistances;
		// Расстояние дальше которого кварталы уже не рендертся
		float farDistance;
		// Максимально дальное расстояние на котором кварталы ещё рендерятся целиком
		float farFullRenderDistance;
		// Максимально близкое расстояние на котором кварталы ещё рендерятся целиком
//		float nearFullRenderDistance;
		// Расстояние ближе которого кварталы уже не рендерятся
		float nearDistance;
		// Расстояние на котором переключаются лоды
		float treeslodDistance;
		// Расстояние дальше которого кварталы (деревья) уже не рендертся
		float treesFarDistance;
		// Максимально дальное расстояние на котором кварталы (деревья) ещё рендерятся целиком
		float treesFarFullRenderDistance;
		// Максимально дальное расстояние на котором кварталы (деревья) ещё не блендятся с текстурой подложки
		float treesFarFullBlendRenderDistance;
		float heightFactor;
		float heightRandomFactor;
		float ajastfactor;
		float lampFarDistance;
		float splineBlockFarDistance;
		float maxFarDistance()
		{
			float mfd = std::max(farDistance, treesFarDistance);
			mfd = std::max(mfd, lampFarDistance);
			mfd = std::max(mfd, splineBlockFarDistance);
			return mfd;
		};
		// Настройки ламп
		LampOptions lamp;
		DistrictRenderType renderType;
	};

	DistrictOptions district;

	// Настройки новых кварталов
	struct Block4Options
	{
		bool enabled;

		// Использовать настройки дальностей от старых кварталов см. DistrictOptions
		bool legacyMode;

		// Дальность в метрах с которой вместо edm начинает рендерится нулевой лод домиков
		float lod0Distance;

		// Дальность в метрах с которой вместо нулевого лода начинает рендерится первый лод
		float lod1Distance;

		// Дальность в метрах с которой домики начинают блендится с землей и сплющиваться
		float blendDistance;

		// Дальность в метрах дальше которой кварталы не рендерятся
		float farDistance;
	};

	Block4Options block4;

	// Бленд объектов по дальности
	struct DistanceBlend
	{
		// Ближние и дальние расстояния для бленда по-дальности. Отдельные для каждого типа накладных.
		float townNearDistance,      townFarDistance;         // Подложки городов
		float townLightNearDistance, townLightFarDistance;    // Освещеннка на подложках городов
		float fieldNearDistance,     fieldFarDistance;        // Поля
		float waterNearDistance,     waterFarDistance;        // Реки\озера
		float shelfNearDistance,     shelfFarDistance;        // Берег
		osg::Vec2f subforest;								  // накладные леса
		osg::Vec2f townClipmap;								  // города при включеном клипмапе
		osg::Vec2f beach;
		osg::Vec2f road;
	};

	DistanceBlend distanceBlend;

	// Вентилятор
	struct Fan
	{
		osg::Vec3d pos;		// от куда дует
		osg::Vec3d dir;		// направление
		float power;		// мощность (метров)
		float radius;		// радиус циллиндра

		float oscillator;
		float frequency;
	};

	Fan fan;

	// Погода
	struct Weather
	{
		Weather();
		osg::Vec2f windDirection;
		float windSpeed;
		float snowDensity;
	};

	Weather weather;

	// Тупые плоские тени
	struct FlatShadow
	{
		// Флажок позволяющий вырубить рендер теней вообще
		bool enabled;
		// Расстояние дальше которого тени уже не рендерятся
		float farDistance;
		// Расстояние до которого тени рендерятся ещё в полную силу
		// fullFarDistance должен быть меньше farDistance. Между ними сила тени блендится
		float fullFarDistance;
	};

	FlatShadow flatShadow;

	// Настройки клипмапа
	struct Clipmap
	{
		bool loggingEnabled;
		int maxUpdatePerFrame;
		int updateToGPU;
		int interlaced;
		bool updateFlagsToGPU;
		osg::Vec3d pointOfView;
		// Сюда надо втянуть настройки разрешения клипмапов, и что там еще не должно лежать в земле
		// ...

		Clipmap()
		{
			loggingEnabled = false;
			maxUpdatePerFrame = 10000;
			interlaced = 1;
			updateToGPU = 7;
			updateFlagsToGPU = true;
		}

		struct ClipmapSettings
		{
			int textureSize;
			int updateStep;
			int updatesPerFrame;
			bool forcedRGBA;

			ClipmapSettings() : textureSize(-1), updateStep(-1), updatesPerFrame(-1),forcedRGBA(false) {}
		};

		ClipmapSettings settings[10];
	};

	Clipmap clipmap;

	// Настройки сплошной облачности
	struct Overcast
	{
		bool visible;
		float bottom;
		float top;

		Overcast()
		{
			visible = false;
			bottom = 0.0f;
			top = 0.0f;
		}
	};

	Overcast overcast;

	// Настройки рендера карты высот
	struct Heightmap
	{
		bool enabled;

		// Размер стороны текстуры карты высот в пикселях
		int resolution;

		// Размер стороны текстуры карты высот в метрах
		float size;
	};

	Heightmap heightmap;

	// Настройки рендера деталей земли
	struct SurfaceDetails
	{
		bool enabled;

		// Сила бленда земли прямо перед камерой
		float surfaceBlendStrength;
	};

	SurfaceDetails surfaceDetails;
	
	// Настройки для дебага
	struct Debug
	{
		int switchoffDrawRoutine;
		int switchoffEdm;
		int switchoffFetchSurface;
		int switchoffSurfaceDetails;
		int switchoffFetchUniqueScenes;
		int switchoffFetchDistricts;
		int switchoffFetchSmokes;
		int switchoffFetchLights;
		int switchoffRenderLights;
		int switchoffRenderLockonTrees;
		int switchoffSomething;
		int switchoffClipmapUpdates;
		int switchoffVehicle;
		int clipmapDebugTextures;
		int viewBoxesForSemantic;
		int switchoffDataFiles;
		int switchoffHeighMapRender;
		char switchoffReference[32];
		
		// test flags for testing some features while developing
		int test1;
		int test2;
		int test3;
		int test4;
		int test5;

		float blockTreesScale;
		float splineBlockTreesScale;

		int renderMode;	// не читается. Устанавливается в TerrainObject
		Debug();
	};

	Debug debug;

public:
	// Сбор статистики
	int batch_count;
	int prim_count;
	int surface_batch_count;
	int block_batch_count;
	int blocktree_batch_count;

	EDTERRAINGRAPHICS_API void clearStatictic();
public:
	TerrainOptions();
//	EDTERRAINGRAPHICS_API void setSunDirection(const osg::Vec3d& sunDir);
};
}

#include "TerrainOptions.inl"

#endif

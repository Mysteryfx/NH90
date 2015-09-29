#ifndef _METEO_DISPATCHER_IMPL_H
#define _METEO_DISPATCHER_IMPL_H
#include "Resourcer/Plugin.h"

#include "weather.h"
#include "Interpolator.h"
#include "math/IIR_Filter.h"
#include "cLinear.h"
#include "wTime.h"
#include "Renderer/RendererCallback.h"
#include "IMeteoDispatcher.h"

#include <ed/SounderAPI.h>
#include "lTerraBasic.h"

class gCloudsZW;
class viCloudObject;

class WEATHER_API moMeteoDispatcherImpl : 
	public IMeteoDispatcher,
	public Plugin, 
	public wTime,
	public Graphics::RendererCallback
{
public:
	static moMeteoDispatcherImpl* instance;
	static moMeteoDispatcherImpl* Create();
	static void Destroy();
protected:
    int precipitationID;

	// Карта облачности
	bool bGenerateComplete;
	Interpolator<2> interp;
	double cloud_alpha;
	double cloud_power;
	// Добавок к вероятности
	float deltaPosibility;

	CloudsType clouds_type;

	// Число типов облаков
//	int typecount;		
	// Плотность облаков (число облаков на кв.километр при вероятности 1) 
	float cloudsPerKM2;			
	// Размер генерируемой области (за границами повторение)
	float wrapsize;

	// Температура для включения осадков - интерполяция линейная
	float tOnSeaLevel;			// Температура у земли
	float tOn10kmLevel;			// Температура на 10 км

	int pressure0;				// Давление на уровне моря в мм.рт.ст.

	// Вероятность что облако c осадками (0-1)
	float precipitationPosibility;
	// Мощность осадков под облаком
	float precipitation_power_min;
	float precipitation_power_max;
	// Мощность осадков фоновая
	float precipitation_default;

	// Период появления молнии
	float thunderbolt_period;

	// Ниж.граница и Верх.граница области
	float height_min, height_max;
	// Радиус облака
	float radius_min, radius_max;
	// Миним. расстояние между облаками
	float min_distance;
	// Параметры наземного тумана
	float fog_maxheigth, fog_minheigth, fog_density;
	// Видимость на высоте > fog_maxheigth
	float vis_near_maxheigth;
	float vis_far_maxheigth;
	float vis_mf_maxheigth;			// мин.плотность тумана
	// Видимость на высоте < fog_minheigth
	float vis_near_minheigth;
	float vis_far_minheigth;
	float vis_mf_minheigth;			// мин.плотность тумана
	// Видимость при плотности осадков = 1
	float vis_near_maxpower;
	float vis_far_maxpower;


	// Текущие осадки
	float precipitation_last;
	float precipitation_current;//текущая сила осадков
	float precipitation_ratio;//отношение снега/дождя [0;1]
	float rain_current;

	// последнее положение камеры
	Position3 last_cam_pos;

	// Тимуровы облака
	gCloudsZW* pCloudsZW;

// Описание облачности обитаемой области
protected:
	// Уровень квадрата 0-10 1-20 2-40
	int level;
	// Описание квадрата
	struct square
	{
		int startRandomSequence;	// Стартовое значение для посл. случ. чисел
		bool bComplete;
		square();
	};
	ed::map<intpoint, square> squares;

    // Список квадратов, облака из которых на отрисовке для случая с динамической атмосферой
    ed::map<osg::Vec2i, bool>  visualizedSquares;

	// Список обрабатываемых облаков
	ed::map<DWORD, viCloudObject*> processedclouds;

	// Максимальная дистанция для облаков
	float fardistance;

    int seasonID;

	// Ветер
	Vector3 wind0, wind2000, wind8000;
	float turbulence0, turbulence2000, turbulence8000;

	// Номер тек. кадра
	int frame_number;

	void UpdateFog(const Position3& pos,	edcg::Fog* pFog);
	void UpdateRainSnow(const Position3& pos);
	void UpdateClouds(const Position3& pos, float distance);
    void UpdateCloudsUsingDynamicAtmosphere(const Position3& pos, float distance);

public:

public:
	moMeteoDispatcherImpl();
	~moMeteoDispatcherImpl();

	// Инициализация
	void Init();
	void Exit();

	// Вызывается при старте миссии
	void StartMission();
	// Вызывается при завершении миссии
	void StopMission();

    virtual void SetSeasonID(int seasonID_) {seasonID = seasonID_;};
    virtual int GetSeasonID()const{return seasonID;};

    virtual void SetPrecipitationID(int precipitationID_) {precipitationID = precipitationID_;};
    virtual int GetPrecipitationID()const{return precipitationID;};

    // Настройки
	virtual void SetCloudsType(CloudsType type);

	// Радиус облака (1000-3000)
	virtual void SetRadius(float min, float max, float min_distance);
    virtual void GetRadius(float& min_, float& max_, float& min_distance_)const;

	// Высота облачности
	virtual void SetHeight(float min, float max);
	virtual void GetHeight( float *fMin , float *fMax );
	
    // Размер карты облаков (за границами повторение) 100-300 км
	virtual void SetWrapSize(float size);
    virtual float GetWrapSize()const;

	// Вероятность что облако c осадками (0-1)
	virtual void SetPrecipitationPosibility(float value);
    virtual float GetPrecipitationPosibility()const;
	
    // Плотность облаков (число облаков на кв.километр при вероятности 1) 
	virtual void SetCloudsDensity(float cloudsPerKM2);
    virtual float GetCloudsDensity()const;
	
    // Температура для включения осадков - интерполяция линейная
	virtual void SetTemperature(
		float tOnSeaLevel,			// Температура у земли
		float tOn10kmLevel);		// Температура на 10 км
	
	//Давление
	virtual void SetPressure(int pressure0_mmHg); //задается в мм.рт.ст
	virtual int GetPressure()const;	//выдается в мм.рт.ст

	// Мощность осадков
	virtual void SetPrecipitation(
		float precipitation_power_min,		// Под облаком
		float precipitation_power_max,		// Под облаком
		float precipitation_default);		// Мощность осадков фоновая

	virtual void GetPrecipitation(
		float & precipitation_power_min,		// Под облаком
		float & precipitation_power_max,		// Под облаком
		float & precipitation_default)const;		// Мощность осадков фоновая
	
    // Период появления молнии в сек. (0 - без молнии)
	virtual void SetThunderbolt(float thunderbolt_period);
    virtual float GetThunderbolt()const;

	// Параметры наземного тумана
	virtual void SetGroundFog(
		float maxheigth,			// Верхняя граница
		float minheigth,			// Нижняя граница
		float density);				// Плотность
    virtual void GetGroundFog(float& maxheigth_, float& minheigth_, float& density_)const;

	// Максимальная видимость (на высоте > fog_maxheigth)
	virtual void SetMaxVisibility(float ffar, float fnear, float fminfog);
    virtual void GetMaxVisibility(float& ffar_, float& fnear_, float& fminfog_);

	// Минимальная видимость (на высоте < fog_minheigth)
	virtual void SetMinVisibility(float ffar, float fnear, float fminfog);
    virtual void GetMinVisibility(float& ffar_, float& fnear_, float& fminfog_);

    // Видимость при плотности осадков = 1
	virtual void SetPrecipitationVisibility(float ffar, float fnear);
    virtual void GetPrecipitationVisibility(float& ffar_, float& fnear_);

	// Ветер
	virtual void SetWind(
		const Vector3& wind0,
		const Vector3& wind2000,
		const Vector3& wind8000);
    virtual void GetWind( Vector3& wind0_, Vector3& wind2000_, Vector3& wind8000_)const;
	
    // Турбулентность
	virtual void SetTurbulence(
		float turbulence0, 
		float turbulence2000, 
		float turbulence8000);
    virtual void getTurbulence(float& turbulence0_, float& turbulence2000_, float& turbulence8000_);

	// Генерация карты облаков
	bool Randomize(
		int startRandomSequence,	// Стартовое значение для посл. случ. чисел
		int numZones,				// Число зон (10-30)
		double cloudDensity,		// Плотность облаков (0-1)
		double cloudDispersion		// Резкость фронтов облаков (0-1)
		);

	// уничтожить облака
	virtual void OnFreeResources();

	// На каждом кадре
	void OnNextFrame(
		const Position3& pos,		// Позиция камеры
		edcg::Fog* pFog=NULL);

	// Высота облачности в точке
	virtual float GetCloudHeight(const Vector3& pos);
	
	// есть ли сплошная облачность, и если есть - ее границы
	virtual bool IsOvercastClouds(float &low, float &high);

	// power - сила осадков в текущем положении камеры [0,1]; ratio - отношение снега/дождя [0,1], где 1 - дождь
	virtual void GetCurrentPrecipitationPower(float &ratio, float &power);

	// получить тип облаков
	virtual CloudsType GetCloudsType();
	// Высота облачности
	virtual void GetHeight(float& min, float& max){min=height_min; max=height_max;};

	// Температура воздуха
	virtual float GetTemperature(float hgt_from_sea_level);

	// Ветер
    virtual void getWind(const Vector3& pos, Vector3& wind);
	virtual void getWind(const Vector3& pos, float& velocity, float& direction);

	virtual void getWindExtended(const Vector3& pos, Vector3& wind, float AltitudeAboveGround);//from Weather::sAtmopshere
	virtual void getWindBase	(const Vector3& pos, Vector3& wind, float AltitudeAboveGround);//from Weather::sAtmopshere


	// Ветер с учетом поверхности
	virtual void getWind(const landscape::lPointOnSurface& pos, float y, Vector3& wind);
	// Турбулентность
	virtual float getTurbulence(const Vector3& pos);
	// Плотность тумана в точке (0-нет, 1 - много тумана)
	virtual float getFogDensity(const Vector3& pos);
	// Получить степень перекрытия отрезка облаками [0,1]
	virtual float getCloudOcclusion(const Vector3& A, const Vector3& B);

	// Преобразование в bmp
	void BuildBmp(int size, const char* filename);

public:
	// Граф. интерфейс
	gCloudsZW* GetCloudsZW(){ return pCloudsZW;}	
	// Добавить в список
	void AddCloudToList(DWORD, viCloudObject*);
	void RemoveCloudFromList(DWORD);
	// Максимальная дистанция для облаков
	float GetFarDistance();

protected:
	// Генерация облака в точке
	bool GenerateCloud(cVector& pos);

	// Плотность осадков в точке (0-1)
	float GetPrecipitationPower(cPoint& pos);

	// Генерация облаков в заданном объеме
	void GenerateCloudsInVolume(int startRandomSequence, Box& box);

	// найти точку из которой ударит молния... и собственно куда й*бнет
	bool GetLightningPos(Vector3 &p, Vector3 &target, float &surfaceHeight);

public:
	// Вероятность появления облаков в точке
	// Координаты точки нормированы от 0 до 1
	double GetCloudPosibility(double x, double y);

// Ф-ции wTime
protected:
	virtual void NextEvent();

// SOUND {
protected:
    void sound_init();
    void sound_update(const Position3& pos);
    void sound_thunder(const Vector3& pos);
    void sound_exit();

	// эмбиентный звук -----------
	int					surfSamples;//количество семплов семантик земели
	float				fadeOutHeight;	//высота, на которой уже не слышно воды, кузнечиков, и т.п.
	float				resampleDistance;
	double				resampleTime;
	float				sampleRadius;//радиус семплирования семантик
	Math::IIR_Filter	filterLPF;
	bool				bWinter;
	wModelTime			updateTime, curTime;
	Vector3				oldCamPos;//старое положение камеры
	osg::Vec2d			*samplePos;//массив оффсетов позиций для семплирования
	float				seaWeight, fieldWeight, waterWeight;

	void ambientSoundInit();
	void ambientSoundExit();
	void ambientSoundUpdate(const Position3 &pos);
	void updateAmbientSoundWeights(const Vector3 &cameraPos, float height);

	ed::Sounder sounder;
// } SOUND

};


#endif
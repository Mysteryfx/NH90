#pragma once
#include "Inter.h"

#include "Vector.h"
#include "Position.h"
#include "Renderer\VolumeEffect.h"
#include "clinear.h"

namespace edcg
{
	class Fog;
};
namespace landscape
{
	class lPointOnSurface;
}

namespace landscape4
{
	class lPointOnSurface;
}

// тип облачности
enum CloudsType
{
	ctNone,		// отсутствие облачности
	ctCumulus,	// кучевые облака
	ctOvercast,	// сплошная облачность
};

enum SEASON_PRECIPITAIONS
{
	// ID_WAROBJECT = 0,
	PRECIPITAION_RAIN = 1,
	PRECIPITAION_THUNDERSTORM,
	PRECIPITAION_SNOW,
	PRECIPITAION_SNOWSTORM
};

// Абстрактный класс управления погодными явлениями
class IMeteoDispatcher
{
public:
	// Инициализация
	virtual void Init()=0;
	virtual void Exit()=0;

	// Вызывается при старте миссии
	virtual void StartMission()=0;
	// Вызывается при завершении миссии
	virtual void StopMission()=0;

    virtual void SetSeasonID(int seasonID_) = 0;
    virtual int GetSeasonID()const = 0;

    virtual void SetPrecipitationID(int precipitationID_)=0;
    virtual int GetPrecipitationID()const =0;

    // Настройки
	virtual void SetCloudsType(CloudsType type) = 0;
	
    // Радиус облака (1000-3000)
	virtual void SetRadius(float min, float max, float min_distance)=0;
    virtual void GetRadius(float& min_, float& max_, float& min_distance_)const=0;
	
    // Высота облачности
	virtual void SetHeight(float min, float max)=0;
	virtual void GetHeight( float *fMin , float *fMax ) = 0;
	
    // Размер карты облаков (за границами повторение) 100-300 км
	virtual void SetWrapSize(float size)=0;
	
    // Вероятность что облако c осадками (0-1)
	virtual void SetPrecipitationPosibility(float value)=0;
    virtual float GetPrecipitationPosibility()const=0;

	// Плотность облаков (число облаков на кв.километр при вероятности 1)  (не больше 0.01)
	virtual void SetCloudsDensity(float cloudsPerKM2)=0;
    virtual float GetCloudsDensity()const=0;

	// Температура для включения осадков - интерполяция линейная
	virtual void SetTemperature(
		float tOnSeaLevel,			// Температура у земли
		float tOn10kmLevel)=0;		// Температура на 10 км
	
	//Давление
	virtual void SetPressure(int pressure0_mmHg)=0; //задается в мм.рт.ст
	virtual int GetPressure()const=0;	//выдается в мм.рт.ст

	// Мощность осадков
	virtual void SetPrecipitation(
		float precipitation_power_min,		// Под облаком
		float precipitation_power_max,		// Под облаком
		float precipitation_default)=0;		// Мощность осадков фоновая

	virtual void GetPrecipitation(
		float & precipitation_power_min,		// Под облаком
		float & precipitation_power_max,		// Под облаком
		float & precipitation_default)const=0;		// Мощность осадков фоновая
	
    // Период появления молнии в сек. (0 - без молнии)
	virtual void SetThunderbolt(float thunderbolt_period)=0;
    virtual float GetThunderbolt()const=0;

	// Туман
	virtual void SetGroundFog(
		float maxheigth,			// Верхняя граница
		float minheigth,			// Нижняя граница
		float density)=0;				// Плотность
    virtual void GetGroundFog(float& maxheigth_, float& minheigth_, float& density_)const=0;

	// Максимальная видимость (на высоте > fog_maxheigth)
	virtual void SetMaxVisibility(float far, float near, float minfog)=0;
    virtual void GetMaxVisibility(float& ffar_, float& fnear_, float& fminfog_)=0;

	// Минимальная видимость (на высоте < fog_minheigth)
	virtual void SetMinVisibility(float far, float near, float minfog)=0;
    virtual void GetMinVisibility(float& ffar_, float& fnear_, float& fminfog_)=0;

    // Видимость при плотности осадков = 1
	virtual void SetPrecipitationVisibility(float ffar, float fnear)=0;
    virtual void GetPrecipitationVisibility(float& ffar_, float& fnear_)=0;

	// Ветер
	virtual void SetWind(
		const Vector3& wind0,
		const Vector3& wind2000,
		const Vector3& wind8000)=0;
    virtual void GetWind( Vector3& wind0_, Vector3& wind2000_, Vector3& wind8000_)const=0;

	// Турбулентность
	virtual void SetTurbulence(
		float turbulence0, 
		float turbulence2000, 
		float turbulence8000)=0;
    virtual void getTurbulence(float& turbulence0_, float& turbulence2000_, float& turbulence8000_)=0;


	// Генерация карты облаков
	virtual bool Randomize(
		int startRandomSequence,	// Стартовое значение для посл. случ. чисел
		int numZones,				// Число зон (10-30)
		double cloudDensity,		// Плотность облаков (0-1)
		double cloudDispersion		// Резкость фронтов облаков (0-1)
		)=0;

// Функции
public:
	// На каждом кадре
	virtual void OnNextFrame(
		const Position3& pos,		// Позиция камеры
		edcg::Fog* pFog=NULL)=0;

	// Высота облачности в точке
	virtual float GetCloudHeight(const Vector3& pos) = 0;
	
	// есть ли сплошная облачность, и если есть - ее границы
	virtual bool IsOvercastClouds(float &low, float &high) = 0;

	// power - сила осадков в текущем положении камеры [0,1]; ratio - отношение снега/дождя [0,1], где 1 - дождь
	virtual void GetCurrentPrecipitationPower(float &ratio, float &power) = 0;
	
	// получить тип облаков
	virtual CloudsType GetCloudsType() = 0;
	// Высота облачности
	virtual void GetHeight(float& min, float& max)=0;

	// Температура воздуха
	virtual float GetTemperature(float hgt_from_sea_level) = 0;

	// Ветер
    virtual void getWind(const Vector3& pos, Vector3& wind)=0;
	virtual void getWind(const Vector3& pos, float& velocity, float& direction)=0;


	virtual void getWindExtended(const Vector3& pos, Vector3& wind, float AltitudeAboveGround) =0;//from Weather::sAtmopshere
	virtual void getWindBase	(const Vector3& pos, Vector3& wind, float AltitudeAboveGround) =0;//from Weather::sAtmopshere

	// Ветер с учетом поверхности
	virtual void getWind(const landscape::lPointOnSurface& pos, float y, Vector3& wind)=0;
	// Турбулентность
	virtual float getTurbulence(const Vector3& pos)=0;
	// Плотность тумана в точке (0-нет, 1 - много тумана)
	virtual float getFogDensity(const Vector3& pos)=0;
	// Получить степень перекрытия отрезка облаками [0,1]
	virtual float getCloudOcclusion(const Vector3& A, const Vector3& B)=0;

    virtual float GetPrecipitationPower(cPoint& pos)=0;

public:
	//interface for new clouds
	struct CloudsParams {
		int		density;
		float	low, high;
		CloudsParams(): density(0), low(0), high(0)	{}
		CloudsParams(int d, float l, float h): density(d), low(l), high(h)	{}
	};

	virtual	void				setCloudsParams(int d, float low, float high)	{	cloudsParams = CloudsParams(d, low, high);	}
	virtual	const CloudsParams&	getCloudsParams()								{	return cloudsParams;						}

protected:
	CloudsParams	cloudsParams;

};

extern INTER_API IMeteoDispatcher* globalMeteoDispatcher;

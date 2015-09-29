#ifndef _BIN_WEATHER_H_
#define _BIN_WEATHER_H_

//описание состояния облачности
struct binWeather
{
	binWeather() : fCloudsBase(0.0f)
		, fCloudsThickness(0.0f)
		, fCloudsDensity(0.0f)
		, iPrecipitaionID(0)
		, fWindDir(0.0f)
		, fHorizWindSp(0.0f)
		, fVertWindSp(0.0f)
	{};
	float fCloudsBase;
	float fCloudsThickness;
	float fCloudsDensity;
	int iPrecipitaionID;
	float fWindDir;
	float fHorizWindSp;
	float fVertWindSp;
};

#endif	//_BIN_WEATHER_H_
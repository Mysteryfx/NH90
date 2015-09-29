#ifndef _BIN_ATMOSPHERE_H_
#define _BIN_ATMOSPHERE_H_

//описание состояния атмосферы
struct binAtmosphere
{
	binAtmosphere():fVisibility(0.0f)
		, iHumidity(0)
		, fAirTemp(0.0f)
	{};

	float fVisibility;	//дальность видимости
	int iHumidity;		//влажность (%)
	float fAirTemp;		//температура
};

#endif	//_BIN_ATMOSPHERE_H_
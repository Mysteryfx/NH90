
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WEATHER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WEATHER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef WEATHER_EXPORTS
#define WEATHER_API __declspec(dllexport)
#else
#define WEATHER_API __declspec(dllimport)
#endif

// This class is exported from the Weather.dll
/*
class WEATHER_API CWeather {
public:
	CWeather(void);
	// TODO: add your methods here.
};

extern WEATHER_API int nWeather;

WEATHER_API int fnWeather(void);
*/

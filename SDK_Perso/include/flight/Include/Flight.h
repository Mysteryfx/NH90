#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FLIGHT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FLIGHT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef FLIGHT_EXPORTS
#define FLIGHT_API __declspec(dllexport)
#else
#define FLIGHT_API __declspec(dllimport)
#endif

/*
// This class is exported from the Flight.dll
class FLIGHT_API CFlight {
public:
	CFlight(void);
	// TODO: add your methods here.
};

extern FLIGHT_API int nFlight;

FLIGHT_API int fnFlight(void);
*/

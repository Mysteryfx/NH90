#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WRADIO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WRADIO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WRADIO_EXPORTS
#define WRADIO_API __declspec(dllexport)
#else
#define WRADIO_API __declspec(dllimport)
#endif
/*
// This class is exported from the WRadio.dll
class WRADIO_API CWRadio {
public:
	CWRadio(void);
	// TODO: add your methods here.
};

extern WRADIO_API int nWRadio;

WRADIO_API int fnWRadio(void);
*/

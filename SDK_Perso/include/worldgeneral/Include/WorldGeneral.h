#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WORLDGENERAL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WORLDGENERAL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WORLDGENERAL_EXPORTS
#define WORLDGENERAL_API __declspec(dllexport)
#else
#define WORLDGENERAL_API __declspec(dllimport)
#endif
/*
// This class is exported from the WorldGeneral.dll
class WORLDGENERAL_API CWorldGeneral {
public:
	CWorldGeneral(void);
	// TODO: add your methods here.
};

extern WORLDGENERAL_API int nWorldGeneral;

WORLDGENERAL_API int fnWorldGeneral(void);
*/

#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INTER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INTER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef INTER_EXPORTS
#define INTER_API __declspec(dllexport)
#else
#define INTER_API __declspec(dllimport)
#endif



/*/
// This class is exported from the Inter.dll
class INTER_API CInter {
public:
	CInter(void);
	// TODO: add your methods here.
};

extern INTER_API int nInter;

INTER_API int fnInter(void);

/*/

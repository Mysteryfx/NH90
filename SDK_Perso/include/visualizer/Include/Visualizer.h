#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VISUALIZER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VISUALIZER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef VISUALIZER_EXPORTS
#define VISUALIZER_API __declspec(dllexport)
#else
#define VISUALIZER_API __declspec(dllimport)
#endif
/*
// This class is exported from the Visualizer.dll
class VISUALIZER_API CVisualizer {
public:
	CVisualizer(void);
	// TODO: add your methods here.
};

extern VISUALIZER_API int nVisualizer;

VISUALIZER_API int fnVisualizer(void);
*/

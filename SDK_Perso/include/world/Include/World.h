
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WORLD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WORLD_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WORLD_EXPORTS
#define WORLD_API __declspec(dllexport)
#else
#define WORLD_API __declspec(dllimport)
#endif
/*
// This class is exported from the World.dll
class WORLD_API CWorld {
public:
	CWorld(void);
	// TODO: add your methods here.
};

extern WORLD_API int nWorld;

WORLD_API int fnWorld(void);
*/

#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WEAPONS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WEAPONSBASE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WEAPONSBASE_EXPORTS
#define WEAPONSBASE_API __declspec(dllexport)
#else
#define WEAPONSBASE_API __declspec(dllimport)
#endif

#include "ed/vector.h"

class wAircraftGunMount;
typedef ed::vector<wAircraftGunMount*> wAircraftGunMounts;

//////////////////////////////////////////////////////////////////////
//
// MapCoords.h : interface and realization for the MapCoords class
// This file is a truncated copy of file mMapDesc.h,
// that have been extended subsequently.
//
// Author:  Igor Loginov (IVL)
// Created: 28-May-2002 (IVL)
// Changed: 28-Feb-2003 (IVL)
//
// Copyright (C) 2002-2003 Eagle Dynamics
//////////////////////////////////////////////////////////////////////

#pragma once

#include "WorldGeneral.h" // MISSIONEDITOR_IMPORT_EXPORT_SPECIFICATOR
#include "landscape3/UtmCoords.h"
#include <ed/string.h>

namespace MapCoordsSpace
{
const double PI = 3.141592653589793; // no more digits for double format

const double ToDegree	= 180. / PI;
class MapCoords // Общие параметры просмотра карты
{
//    static utm::projection_t projection;
public:
//	WORLDGENERAL_API static void ParseConfig(const char* cfgfilename=NULL);

	WORLDGENERAL_API static double Longitude(double x, double z); // долгота в градусах, (x (meters) - to North, z (meters) - to East)
	WORLDGENERAL_API static double Latitude(double x, double z); // широта в градусах, (x (meters) - to North, z (meters) - to East)
	WORLDGENERAL_API static void LatLon(double x, double z, double& lat, double& lon);
	WORLDGENERAL_API static bool ConvertCoordToGradMinSec(double inCoordGrads, int *outCoordGrad, int *outCoordMin, int *outCoordSec);
    WORLDGENERAL_API static bool ConvertCoordToGradMinSec(double inCoordGrads, int *outCoordGrad, int *outCoordMin, double *outCoordSec);

	// inX (meters) - to North (latitude), inZ (meters) - to East (longitude)
	WORLDGENERAL_API static ed::string ConvertLongitudeToString(float inX,float inZ,bool without_secs_min_as_decimal = false);
	WORLDGENERAL_API static ed::string ConvertLatitudeToString(float inX, float inZ,bool without_secs_min_as_decimal = false);

	WORLDGENERAL_API static void GetCoords(double inLatitudeGrad, double inLongitudeGrad, float &outX, float &outZ); // принимает градусы
    WORLDGENERAL_API static void GetCoords(double inLatitudeGrad, double inLongitudeGrad, double &outX, double &outZ); // принимает градусы
};

};

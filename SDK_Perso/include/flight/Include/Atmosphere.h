#pragma once

#include "Flight.h"
#include "cLinear.h"

class viObject;
class viSearch;

class FLIGHT_API AtmoshereInit
{
public:
    static void clearAtmosphere();
    static void initAtmosphere(float Temperature, int Pressure_mmHg, cVector *Wind, float *Turbulence, int Precipitation);
    static void initRelativeHumidity();
    static void startAtmosphere();
    static void stopAtmosphere();

    // обходит атмосферные эффекты и вычисляет для всех точек points суммарную скорость
    static void getDynamicAirState(int n, /*in*/ dVector *points, /*out*/ cVector *velocities);

private:
    static void initStaticAtmosphere(float Temperature, int Pressure_mmHg, cVector *Wind, float *Turbulence, int Precipitation);
    static void initDynamicAtmosphere(float Temperature, float *Turbulence);
    static bool dynamicAirStateHandler(viObject *object, viSearch *search, void *data);


};
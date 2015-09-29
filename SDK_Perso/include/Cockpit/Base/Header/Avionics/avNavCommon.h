#pragma once

#include "Avionics\avDevice.h"
#include "dPosition.h"

namespace cockpit
{

class COCKPITBASE_API avNavCommon
{
public:
	static dVector get_pt_lcoords(dPosition, double, double, double);
    static double calcTrackDeviation (double x1, double y1, // начальная точка
                                      double x2, double y2, // конечная точка
                                      double x3, double y3); // текущее местоположение

    static double calcPassedRange  (double x1, double y1, // начальная точка
                                    double x2, double y2, // конечная точка
                                    double x3, double y3) // текущее местоположение
    {
        return avNavCommon::calcTrackDeviation (x1,
                                                y1,
                                              -(y2 - y1) + x1,
                                               (x2 - x1) + y1,
                                                x3,
                                                y3);
    };

	// Вычисление новых линейных координат по азимуту/дальности от точки и курсу
	static dVector calcLinearPtFrom_BrngTo_Range(const dVector& PtIn,
		double BrngIn, double RngIn);
};

}

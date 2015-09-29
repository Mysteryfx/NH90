#ifndef UNIT_CONVERTER_H
#define UNIT_CONVERTER_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <assert.h>

namespace unitConv
{
	enum LengthUnits
	{
		METER = 0,
		FOOT,
		YARD,
		NAUTICAL_MILE,
		INCH,
		LEN_UNIT_MAX
	};

	static const double lenght_k[LEN_UNIT_MAX][LEN_UNIT_MAX] =
	{
		//meter to other
		{1.0,		1.0/0.3048,		1.0/0.9144,	1.0/1852.0,		1.0/0.0254},
		//foot to other
		{0.3048,	1.0,			1.0/3.0,	0.3048/1852.0,	0.3048/0.0254},
		//yards to other
		{0.9144,	3.0,			1.0,		0.9144/1852.0,	0.9144/0.0254},
		//nautical mile to other
		{1852.0,	1852.0/0.3048,	1852.0/0.9144, 1.0,			1852.0/0.0254},
		//inch to other
		{0.0254,	0.0254/0.3048,	0.0254/0.9144, 0.0254/1852.0,	1.0}
	};

	template<typename T>
	T convert(const T value_in, const LengthUnits from, const LengthUnits to)
	{
		assert((from >= METER && from < LEN_UNIT_MAX) 
			&& (to >= METER && to < LEN_UNIT_MAX)
			&&"convert_length: Arguments are out of range");

		return static_cast<T>(value_in * lenght_k[from][to]);
	}

	enum SpeedUnits
	{
		M_S = 0, // meter per second
		FT_S,	 // foot per second
		FT_MIN,	 // foot per minute
		KNOT /* knot */ ,		
		MPH,     // mile per hour
		KPH,     // kilometres per hour
		SPEED_UNIT_MAX
	};

	static const double speed_k[SPEED_UNIT_MAX][SPEED_UNIT_MAX] =
	{
		//mps to other
		{1.0,			1.0/0.3048,		60.0/0.3048,	3600.0/1852.0,		1.0/0.44704,		3.6},
		//fps to other
		{0.3048,		1.0,			60.0,			1097.28/1852.0,		0.3048/0.44704,		0.3048*3.6},
		//fpm to other
		{0.3048/60.0,	60.0,			1.0,			65836.8/1852.0,		18.288/0.44704,		0.3048*3.6/60.},
		//knot to other
		{1852.0/3600.0,	1852.0/1097.28,	1852.0/65836.8,	1.0,				1852.0/1609.344,	1.852},
		//mph  to other
		{0.44704,		0.44704/0.3048,	0.44704/18.288,	1609.344/1852.0,	1.0,				1.609344},
		//kph to other
		{1.0/3.6,		1.0/1.09728,	60.0/1.09728,	1.0/1.852,			1.0/1.609344,		1.0}
	};

	template<typename T>
	T convert(const T value_in, const SpeedUnits from, const SpeedUnits to)
	{
		assert((from >= M_S && from < SPEED_UNIT_MAX) 
			&& (to >= M_S && to < SPEED_UNIT_MAX)
			&&"convert_speed: Arguments are out of range");

		return static_cast<T>(value_in * speed_k[from][to]);
	}

	enum PressureUnits
	{
		PA = 0,	// pascal
		PSI,		// pounds per square inch
		MERCURY_MM,	// mm of mercury
		MERCURY_INCH,//inch of mercury
		PSF,		// pound-force per square foot
		KGSCM,		// kilogram-force/square centimeter 
		ATM,		// Standard atmosphere
		ATA,		// Technical atmosphere
		PRESSURE_UNIT_MAX
	};

	static const double pressure_k[PRESSURE_UNIT_MAX][PRESSURE_UNIT_MAX] =
	{
		//pascal to other
		{1.0,		1.0/6894.757,		1.0/133.322,		1.0/3386.3788,		144.0/6894.757, 0.000010197, 0.000009869,	0.000010197},
		//psi to other
		{6894.757,	1.0,				6894.757/133.322,	6894.757/3386.3788, 144.0,			0.070306958, 0.068045964,	0.070306958},
		//mm mercury  to other
		{133.322,		133.322/6894.757,	1.0,			1.0/25.4,			144.0 * 133.322/6894.757, 0.001359506, 0.001315786, 0.001359506},
		//inch mercury  to other
		{3386.3788,		3386.3788/6894.757,	25.4,			1.0,				144.0 * 3386.3788/6894.757, 0.034531466, 0.033420972, 0.034531466},
		//psf  to other
		{6894.757/144.0,	1.0/144.0,	6894.757/(144.0 * 133.322),	6894.757/(144.0 * 3386.3788),	1.0, 0.000488243,0.000472541, 0.000488243},
		//kilogram-force/square centimeter  to other
		{98066.5,	14.223343307,	735.561272708,	28.959094963,	2048.161436222, 1.0,	0.967841105, 1.0},
		//Standard atmosphere to other
		{101325.0,	14.695948775,	760.002100179,	29.921331924,	68087.256406011, 1.033227453,	1.0, 1.033227453},
		//Technical atmosphere to other
		{98066.5,	14.223343307,	735.561272708,	28.959094963,	2048.161436222, 1.0,	0.967841105, 1.0},
	};

	template<typename T>
	T convert(const T value_in, const PressureUnits from, const PressureUnits to)
	{
		assert((from >= PA && from < PRESSURE_UNIT_MAX) 
			&& (to >= PA && to < PRESSURE_UNIT_MAX)
			&&"convert_pressure: Arguments are out of range");

		return static_cast<T>(value_in * pressure_k[from][to]);
	}

	enum MassUnits
	{
		KG = 0,	  // kilo
		LB,		  // pound
		MASS_UNIT_MAX
	};

	static const double mass_k[MASS_UNIT_MAX][MASS_UNIT_MAX] =
	{
		//kilo to other
		{1.0,			1.0/0.45359237},
		//pound  to other
		{0.45359237,	1.0}
	};

	template<typename T>
	T convert(const T value_in, const MassUnits from, const MassUnits to)
	{
		assert((from >= KG && from < MASS_UNIT_MAX) 
			&& (to >= KG && to < MASS_UNIT_MAX)
			&&"convert_mass: Arguments are out of range");

		return static_cast<T>(value_in * mass_k[from][to]);
	}

	enum VolumeUnits
	{
		METER_3 = 0,	// m^3
		LITRE,			// litre
		US_GALLON,		// US gallon
		INCH_3,			// inch^3
		VOLUME_UNIT_MAX
	};

	static const double volume_k[VOLUME_UNIT_MAX][VOLUME_UNIT_MAX] =
	{
		//m^3 to other
		{1.0,				1000.0,			264.172,	61023.76},
		//litre to other
		{0.001,				1.0,			0.264172,	61.02376},
		//US gallons to other
		{0.003785411784,	3.785411784,	1.0,		231.0},
		//inch^3 to other
		{0.00001687,		0.016387,		0.004329,	1.0}
	};

	template<typename T>
	T convert(const T value_in, const VolumeUnits from, const VolumeUnits to)
	{
		assert((from >= METER_3 && from < VOLUME_UNIT_MAX) 
			&& (to >= METER_3 && to < VOLUME_UNIT_MAX)
			&&"convert_volume: Arguments are out of range");

		return static_cast<T>(value_in * volume_k[from][to]);
	}

	//lenght
	template<typename T>
	T FeetToMeters(const T value_in)
	{
		return convert(value_in,FOOT,METER);
	}

	template<typename T>
	T MetersToFeet(const T value_in)
	{
		return convert(value_in,METER,FOOT);
	}

	template<typename T>
	T YardsToMeters(const T value_in)
	{
		return convert(value_in,YARD,METER);
	}

	template<typename T>
	T MetersToYards(const T value_in)
	{
		return convert(value_in,METER,YARD);
	}

	template<typename T>
	T MetersToNM(const T value_in)
	{
		return convert(value_in,METER,NAUTICAL_MILE);
	}

	template<typename T>
	T NMToMeters(const T value_in)
	{
		return convert(value_in,NAUTICAL_MILE,METER);
	}

	//speed
	template<typename T>
	T MpsToFpm(const T value_in)
	{
		return convert(value_in,M_S,FT_MIN);
	}

	template<typename T>
	T FpmToMps(const T value_in)
	{
		return convert(value_in,FT_MIN,M_S);
	}

	template<typename T>
	T MpsToFps(const T value_in)
	{
		return convert(value_in,M_S,FT_S);
	}

	template<typename T>
	T FpsToMps(const T value_in)
	{
		return convert(value_in,FT_S,M_S);
	}

	template<typename T>
	T MpsToKnots(const T value_in)
	{
		return convert(value_in,M_S,KNOT);
	}

	template<typename T>
	T KnotsToMps(const T value_in)
	{
		return convert(value_in,KNOT,M_S);
	}

	template<typename T>
	T MpsToMilesph(const T value_in)
	{
		return convert(value_in,M_S,MPH);
	}

	template<typename T>
	T MilesphToMps(const T value_in)
	{
		return convert(value_in,MPH,M_S);
	}
 
	//pressure
	template<typename T>
	T PSItoPascals(const T value_in)
	{
		return convert(value_in,PSI,PA);
	}

	template<typename T>
	T PascalsToPSI(const T value_in)
	{
		return convert(value_in,PA,PSI);
	}

	template<typename T>
	T MercuryMmToPascals(const T value_in)
	{
		return convert(value_in,MERCURY_MM,PA);
	}

	template<typename T>
	T PascalsToMercuryMm(const T value_in)
	{
		return convert(value_in,PA,MERCURY_MM);
	}

	//temperature
	template<typename T>
	T KelvinToCelcius(const T value_in)
	{
		return value_in - static_cast<T>(273.15);
	}

	template<typename T>
	T CelciusToKelvin(const T value_in)
	{
		return value_in + static_cast<T>(273.15);
	}

	//mass
	template<typename T>
	T KilogramToPound(const T value_in)
	{
		return convert(value_in,KG,LB);
	}

	template<typename T>
	T PoundToKilogram(const T value_in)
	{
		return convert(value_in,LB,KG);
	}

	//////////////////////////////////////////////////////////////////////////
	// power

	enum PowerUnits
	{
		WATT = 0,	// 
		PS,			// horsepower
		POWER_UNIT_MAX
	};

	static const double power_k[POWER_UNIT_MAX][POWER_UNIT_MAX] =
	{
		//watt to other
		{1.0,				0.001359622},
		//hp metric to other
		{735.49875,		1.0},
	};

	template<typename T>
	T convert(const T value_in, const PowerUnits from, const PowerUnits to)
	{
		assert((from >= WATT && from < POWER_UNIT_MAX) 
			&& (to >= WATT && to < POWER_UNIT_MAX)
			&&"convert_power: Arguments are out of range");

		return static_cast<T>(value_in * power_k[from][to]);
	}

}
#endif // UNIT_CONVERTER_H
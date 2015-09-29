#ifndef HEAT_EXCH_COMMON_H
#define HEAT_EXCH_COMMON_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <ed/string.h>

#include "Mem/Shared.h"

namespace EagleFM { 
namespace Heat {

	typedef double float_t;
	typedef ed::vector<float_t> HeatVector;

	class IHeatBody : public Mem::SharedLite
	{
	public: 
		virtual const float_t&	temp() const = 0;
		virtual float_t&		temp() = 0;

		virtual const float_t&	work_temp() const = 0;
		virtual float_t&		work_temp() = 0;

		virtual const float_t&	heat_cap() const = 0;

		virtual const ed::string& name() const = 0;
	};
}}


#endif
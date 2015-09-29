#ifndef HEAT_BODY_IMPL_H
#define HEAT_BODY_IMPL_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "FMHeatExchange/heat_exch_common.h"

namespace  EagleFM {
namespace  Heat {

class BodyImpl : public IHeatBody
{
	friend class HeatBody;

	BodyImpl(const char* name, float_t heat_cap, float_t init_temp)
		: name_(name), heat_cap_(heat_cap), temp_(init_temp), work_temp_(0.0)
	{
		assert(heat_cap && "Heat Capacity couldn't be NULL!");
	}

	const ed::string name_;
	float_t heat_cap_;
	float_t temp_;
	float_t work_temp_;

public:

	virtual const float_t&	temp() const {return temp_;}
	virtual float_t&		temp() {return temp_;}

	virtual const float_t&	work_temp() const {return work_temp_;}
	virtual float_t&		work_temp() {return work_temp_;}

	virtual const float_t& heat_cap() const {return heat_cap_;}

	virtual const ed::string& name() const {return name_;}

	~BodyImpl() {}
};

}}

#endif
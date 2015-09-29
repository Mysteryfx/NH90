#ifndef HEAT_BODY_H
#define HEAT_BODY_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Base.h"
#include "FMHeatExchange/heat_exch_common.h"

namespace  EagleFM {
namespace  Heat {

//////////////////////////////////////////////////////////////////////////
// heat body class

class FMBASE_API HeatBody
{
public:

	HeatBody();
	HeatBody(const char* name, float_t heat_cap, float_t init_temp);

	const float_t&	temp() const{ return impl_->temp(); }

	float_t&		temp()		{ return impl_->temp(); }


	const float_t&	work_temp() const{ return impl_->work_temp(); }
	float_t&		work_temp()		 { return impl_->work_temp(); }

	const float_t& heat_cap() const { return impl_->heat_cap(); }

	const ed::string& name() const { return impl_->name(); }

	bool valid() const { return valid_; }

private:
	
	bool	valid_;
	Mem::Ptr<IHeatBody> impl_;
};

}}

#endif
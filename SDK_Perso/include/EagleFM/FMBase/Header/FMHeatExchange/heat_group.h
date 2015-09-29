#ifndef HEAT_BODY_GROUP_H
#define HEAT_BODY_GROUP_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <ed/vector.h>
//#include <ed/set.h>
#include <array>

#include "FMHeatExchange/heat_body.h"

namespace  EagleFM {
namespace  Heat {

typedef ed::vector<ed::vector<float_t>> CoefsMatrix;

// struct CompareBody {
// 	bool operator()( const HeatBody &lhs, const HeatBody &rhs) const{
// 		return lhs.name() < rhs.name();
// 	}};
// 
// typedef ed::set<HeatBody,CompareBody> HeatBodySet;
typedef ed::vector<HeatBody> HeatBodies;

//////////////////////////////////////////////////////////////////////////
//init struct
template <size_t N>
struct BodyGroupInit
{
	std::array<HeatBody, N>		bodies_;
	std::array<float_t, N * N>	coefs_;
};

class FMBASE_API BodyGroupImpl : public Mem::SharedLite
{	
	friend class HeatExchangeManager;
public:
	template <typename T>
	BodyGroupImpl(const T& init_group) 
	{
		size_t N = init_group.bodies_.size();

		std::copy(init_group.bodies_.begin(),init_group.bodies_.end(),back_inserter(bodies_));

		coefs_.resize(N);
		ext_flow_.resize(N);

		auto it = init_group.coefs_.begin();

		size_t count = 0;
		for(size_t i = 0; i < N; ++i)
		{
			size_t n = ((count + N) > init_group.coefs_.size()) ? (init_group.coefs_.size() - count) : N;
			coefs_.at(i).assign(it, it + n);

			it += n;
			count += n;
		}
	}

	void change_conductivity(const char* b_name_1, const char* b_name_2, float_t new_c);
	void set_external_flow(HeatVector& ext_flow);
	HeatBody get_body(const ed::string b_name) const;

private:
	HeatBodies	bodies_;
	CoefsMatrix coefs_;
	HeatVector	ext_flow_;
};

class FMBASE_API HeatGroup 
{
public:

	template <typename T>
	HeatGroup(const T& init_group)
		: impl_( new BodyGroupImpl(init_group)),is_valid_(true) {}

	HeatGroup() : is_valid_(false) {}

	BodyGroupImpl* operator->() { return impl_.get(); }
	const BodyGroupImpl* operator->() const{ return impl_.get(); }

	bool valid() const { return is_valid_; }

private:
	Mem::Ptr<BodyGroupImpl> impl_;
	bool is_valid_;
};

}}

#endif
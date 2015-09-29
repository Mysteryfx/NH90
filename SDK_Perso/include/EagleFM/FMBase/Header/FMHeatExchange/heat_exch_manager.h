#ifndef HEAT_EXCH_MANAGER_H
#define HEAT_EXCH_MANAGER_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

/************************************************************************/
/* HEAT EXCH MANAGER DESCRIPTION:	
	
	Initializing:

	1. Create HeatBodyes:
		
		HeatBody([name],[heat_cap_coef],[init_temp]));

	2. Fill BodyGroupInit struct:

		BodyGroupInit<[bodies_num]> init_group = {body_1,
		body_2, ..., body_n,
		1,		k12, ...,	k1n,
		k21,	1,	...,	k2n,
		...
		kn1,	..,			1 };

	3. Register the group in the HeatExchangeManager:

		HeatExchangeManager::register_group(init_group);
		
*/
/************************************************************************/

#include "Optimizable.h"

#include <ed/map.h>

#include "FMHeatExchange/heat_body.h"
#include "FMHeatExchange/heat_group.h"

namespace  EagleFM {
namespace  Heat {

class FMBASE_API HeatExchangeManager : public Optimizable
{

protected:
	HeatExchangeManager();
public:

	static HeatExchangeManager& instance();

	virtual ~HeatExchangeManager();

	void register_group(const char* grop_name, HeatGroup group_ptr);
	void remove_group(const char* grop_name);

	HeatGroup get_group(const char* name) const;

	/*change conductivity [new_c] in group [grop_name] between body [b_name_1] and body [b_name_2] */
	void change_conductivity(
		const char* grop_name, 
		const char* b_name_1,
		const char* b_name_2,
		float_t new_c);

	/*set external heat flow*/
	void set_external_flow(
		const char* grop_name, 
		HeatVector& ext_flow);

private:

	virtual void doSimulate();

	typedef ed::map<ed::string, HeatGroup> SubGroups;
	struct  MasterGroupData 
	{
		SubGroups	groups_;
	};

	float_t getG(const CoefsMatrix& c_matr, size_t i, size_t j) const;
	void remove_c(SubGroups::value_type& group, size_t index);

	MasterGroupData mg_; //master group
};
}}

#endif
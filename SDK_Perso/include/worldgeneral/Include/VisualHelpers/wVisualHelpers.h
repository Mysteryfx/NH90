#pragma once
#include "WorldGeneral.h"
#include "Registry\Registered.h"
class cPosition;

namespace visual_helpers
{
	 ObjectID WORLDGENERAL_API add_gate(double x,
									    double y,
									    double z,
									    double course,
									    double pitch);
	 ObjectID WORLDGENERAL_API add_gate(const cPosition & pos);
	 void WORLDGENERAL_API add_route_gates_for_unit(const ed::string & unit_id,const char * control_flag);
	 void WORLDGENERAL_API route_gates_set_current_point(const ed::string & unit_id,unsigned point);
};
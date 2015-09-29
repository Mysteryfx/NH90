#pragma once
#include "FMDynamics/DynamicBody.h"
#include "CockpitBase.h"

namespace cockpit	{ namespace dbody_utils {

	Math::Matrix3d  COCKPITBASE_API getOrientation	(EagleFM::DynamicBody * body);
	Math::Matrix3d  COCKPITBASE_API getOrientationT	(EagleFM::DynamicBody * body);
	Math::Vec3d		COCKPITBASE_API getPosition		(EagleFM::DynamicBody * body);
	Math::Vec3d		COCKPITBASE_API getVelocity_w	(EagleFM::DynamicBody * body);
	Math::Vec3d		COCKPITBASE_API calcAccelerationInPoint_l(EagleFM::DynamicBody * body,const Math::Vec3d & pnt);

}}
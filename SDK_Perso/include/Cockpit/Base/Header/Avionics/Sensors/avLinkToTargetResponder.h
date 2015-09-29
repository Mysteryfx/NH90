#pragma once
#include "CockpitBase.h"
#include "Registry\Registered.h"
namespace cockpit 
{
	class COCKPITBASE_API avLinkToTargetResponder
	{
	public:
		virtual ~avLinkToTargetResponder();
		virtual bool is_tracking(ObjectID ID, unsigned weapon_type) const= 0;
		virtual ObjectID get_target_to_engage() const = 0;
		virtual double   get_target_range() const = 0;
	};
}
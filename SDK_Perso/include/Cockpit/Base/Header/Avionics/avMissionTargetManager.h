#pragma once
#include "CockpitBase.h"
#include "cLinear.h"
#include "Utilities/avDrawCashedObj.h"
#include "wMovingObject.h"

class ClipVolume;

namespace cockpit
{
    class COCKPITBASE_API mission_target : public avDrawCashedObj
	{
        friend class avMissionTargetManager;
    private:
        woPointer       ptr;
		dVector         point;
        dVector         local;
    public:
        mission_target():avDrawCashedObj(),ID(0){}
       ~mission_target(){}
        unsigned int   ID;
        dVector get_point() const;
	};
	typedef ed::list<mission_target>  mission_target_list;
	class COCKPITBASE_API avMissionTargetManager
	{
	public:
		avMissionTargetManager();
		virtual ~avMissionTargetManager();

		void  initialize();
		void  update();
		mission_target_list get_objects(ClipVolume *volume) const;
        const mission_target_list & get_objects() const { return targets; }
	protected:
		mission_target_list targets;
		bool			    initialized;
	};
};

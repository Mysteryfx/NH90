#pragma once
#include "CockpitBase.h"
#include "Renderer/Camera.h"
#include "Avionics/Sensors/avTrackData.h"

class viObject;
class viSearch;
namespace cockpit
{
	class COCKPITBASE_API avHideChecker
	{
	public:
		avHideChecker(const Graphics::Camera & camera_,double angle_,unsigned ID_);
		bool		hidden(const avTrackData & track_);
	private:
		bool		handler(viObject *item, viSearch *search);
		static bool hider  (viObject *item, viSearch *search,void* instance)
		{
			return static_cast<avHideChecker*>(instance)->handler(item,search);
		}
	private:
		avTrackData		 track;
		Graphics::Camera camera;
		double			 angle;
		unsigned		 ID;
	};
};
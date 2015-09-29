#pragma once

#include "CockpitBase.h"

#include "Avionics/avMissionTargetManager.h"
#include "Avionics/avDevice.h"
#include "Avionics/Sensors/avTrackData.h"

namespace Graphics
{
    class Camera;
}

namespace cockpit
{
	class avPlatform;

	enum  LSS_mode
	{
		LSS_OFF,
		LSS_NO_LSR,
		LSS_LSS,
		LSS_DETECT,
		LSS_TRACK,
	};

	class avLaserSpotDetector;

	class COCKPITBASE_API avSpot_SearchTimer : public avBasicTimer<avLaserSpotDetector>
	{
	public:
		avSpot_SearchTimer(avLaserSpotDetector* _parent,wModelTime _dtime): avBasicTimer<avLaserSpotDetector>(_parent,_dtime){}
		void		 NextEvent();
	};


	class COCKPITBASE_API avLaserSpotDetector
	{
	public:
				 avLaserSpotDetector();
		virtual ~avLaserSpotDetector();
		void	 set_platform(avPlatform *p) { platform = p; }
        void     set_local_position(const dPosition & pos);
        void     set_fov(double angle);
		void     initialize();
		void     update();
		unsigned get_mode() const { return mode;} 
		void	 lss_start(wModelTime Delay);
		void	 lss_clear();
		bool	 lss_search();
		const dVector     & get_spot_position() const { return spot.world_p.p; } 
		const avTrackData & get_spot() const { return spot;}
		bool	 slave() const { return mode != LSS_OFF && mode != LSS_LSS; }
		void	 set_code(unsigned new_code) { code = new_code;}
		unsigned get_code() const			 { return code;}
		void	 set_narrow(bool n) { narrow = n;};
		bool	 get_narrow() const { return narrow; };
		wModelTime get_track_lose_time() const {return track_lose_time;}


	protected:
		void	 lss_detect();
		void	 lss_track();
		void	 lss_no_lsr();
		bool	 update_spot();
		bool	 hidden() const;
		void	 set_no_lsr();
        bool     set_target(unsigned int target);
        
        Graphics::Camera get_camera(double Aperture) const;

        dPosition get_initial_position() const;

        unsigned int   search_spots(const Graphics::Camera & camera) const;
        static bool     search_handler(viObject *item,viSearch *search, void* data);
		unsigned			    mode;
		avPlatform			   *platform;
		avMissionTargetManager  mission_targets;
		unsigned				code;
		avTrackData				spot;
		avSpot_SearchTimer	   *timer;

		bool					narrow;

        double    fov;       // when platform is absent
        dPosition local_pos; // when platform is absent

		double	 track_lose_time;
		double   track_start_time;
	};
};

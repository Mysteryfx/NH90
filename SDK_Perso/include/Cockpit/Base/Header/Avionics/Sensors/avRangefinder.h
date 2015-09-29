#pragma once
#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "wFloat.h"
#include "dPosition.h"
#include "Avionics/avPlatform.h"


class	  woSpot;
namespace cockpit {

	enum enum_LaserRangerMode
	{
		LR_OFF,			// выключен
		LR_RANGEFINDER, // измерение дальности энергопотребление небольшое
		LR_DESIGNATION, // подсвет ракетам с ЛГСН большое энергопотребление
		LR_PATTERN      // подсвет для ракет типа вихрь : создание пространственного растра энергопотребление небольшое
	};
	struct COCKPITBASE_API LaserGuidedWeaponState
	{
		unsigned int ID;
		bool          is_targeted;
		friend inline Serializer& operator << (Serializer& serializer, LaserGuidedWeaponState& data)
		{
			return serializer << data.ID
					<< data.is_targeted;
		}
	};
	typedef ed::list<LaserGuidedWeaponState> launched_laser_homing_list; 
	
	class  COCKPITBASE_API avRangefinder : public avPlatform,
                                           public avBreakable, 
										   public avBasicElectric
                                           
	{
	public:
		avRangefinder(void);
		virtual ~avRangefinder(void);
		void initialize(float abs_max_dist_in);
		void update(double delta);

		bool		isInRelaxation();

		void		setElecPower(bool flag_in);
		void		set_mode(int new_mode);
		void        set_mode_delayed(int new_mode,double when);
		void		set_aim_point(const dVector & aim_point_);
		void		set_turn_off(double time);
		void		set_next_available_turn_on_time(double time);
		void		set_code	(unsigned new_code)		{ code		   = new_code;};
		void        use_history (bool flag_in)			{ save_history = flag_in;}
		void		push_pattern_guided(ObjectID id);
		void		push_designated(ObjectID id,bool targeted = true);
	
		int			get_mode() const { return work_mode;}
		unsigned    get_code() const { return code;}


		double  get_turn_off_time()    const   { return turn_off_time;}
		double  get_next_available_turn_on_time() const;

		const   dVector & get_aim_point() const;
		double			  get_measured_distance()	const {	return get_measured_distance(wTime::GetModelTime()); }
		dVector			  get_measured_point()	const {	return get_measured_point(wTime::GetModelTime());	}

		double			  get_measured_distance(double time) const;
		dVector			  get_measured_point(double time)	const; 
        double            get_last_measured_distance()  const   { return measured_last;}
        const dVector &   get_last_measured_point()     const   { return measured_point.GetNextValue(); }
        bool			  is_valid_measurement()    const   { return measured;}

        float			  get_temperature()      const   { return temperature;}
        float			  get_temperature_normal() const { return Tnorm;}
        wModelTime		  get_time_from_last_switch() const;
		wModelTime		  getLastModeChangeTime() const {return LastModeChangeTime;}
		
        void			  reset();
        void              pause();
        void              unpause();
        void			  switchOff();
        void              set_reserved_mode(bool value_in) { reserved_mode = value_in; };

        ObjectID          get_spot_ID() const;
		void              set_absolute_max_distance(float dist_in) {absolute_max_distance = dist_in;}

        void              set_IR_pointer_on(bool on);
        bool              get_IR_pointer_on() const { return ir_pointer_on; };

		void              clearAutolase();
		void              clearAutolaseStart();
		void              clearAutolaseStop();
		void			  repair() { avBreakable::repair(); Regenerate(); }

	protected:
        bool   get_distance(const Graphics::Camera & cam,float & distance);
		void   Regenerate();
		void   control_launched_missiles();
		void   control_launched_list(launched_laser_homing_list & lst,bool loose_target);
		void   dispatch_mail(woPointer item);
		void   measure_and_dispatch_mail();
		void   on_newmode(int new_mode);
		bool   check_relaxation_count();
		double update_cooling(double delta);
		void   update_life(double delta);
		static bool	searcher(viObject *item, viSearch *search, void* data);

		void   update_spot();

	protected:
		wModelTime LastSwitchTime;
		wModelTime LastWorkTime;
		wModelTime MaxWorkTime;
		wModelTime MaxIdleTime;
		wModelTime timeOverride;
		wModelTime turn_off_time;
		wModelTime next_available_turn_on_time;
		wModelTime next_available_turn_on_time_by_temperature;
		wModelTime LastModeChangeTime;
		float temperature;
		float Tmax;
		float Tnorm;
		float Tzero;

		float Life;
		short countOverride;

		bool		  isOverrideProtect;
		unsigned char work_mode;
		
		launched_laser_homing_list launched_PATTERN;    //пущенные в режиме PATTERN (вихри)
		launched_laser_homing_list launched_DESIGNATION;//остальные

		dVector     aim_point;
		wVector		measured_point;
		bool        measured;
		bool        save_history;

		unsigned    code;
		woSpot		*spot;
        woSpot      *ir_pointer_spot;

		double       delayed_start_time;
		int			 delayed_mode;
		bool         delayed_start;
		bool         paused;
		int          paused_mode;
		float        absolute_max_distance;
        bool         reserved_mode;
        double       measured_last;
        bool         make_pause_after_switch_off;
        bool         use_override;
        bool         ir_pointer_on;
	};
} 

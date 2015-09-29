#pragma once

#include "Avionics\avDevice.h"
#include "Utilities\ccUtilities.h"

namespace cockpit
{
	enum enum_Padlock
	{
		PADLOCK_OFF,
		PADLOCK_GROUND,
		PADLOCK_OBJECT,
	};

    enum enum_ObjectLock
    {
        PADLOCK_OBJECTS_OFF,
        PADLOCK_OBJECTS_GROUND,
        PADLOCK_OBJECTS_AIR,
        PADLOCK_OBJECTS_WEAPON,
    };

	enum enum_MissilesLock
	{
		PADLOCK_ALL_MISSILES,
		PADLOCK_THREAT_MISSILES
	};

	typedef ed::map<unsigned, woPointer> target_map;


	class COCKPITBASE_API avPadlock : public avDevice
	{
	public:
		 avPadlock(void);
		~avPadlock(void);
		void initialize(unsigned char id, const ed::string& name, const ed::string& script);
		void update();
		void release();
		void serialize(Serializer& serializer);
		void SetCommand(int command, float value = 0);
		
		unsigned char		  get_mode() const { return mode;}
		const PolarPosition & get_polar_position() const { return pole_position;}
		const dVector       & get_lock_point() const { return target_point;}
		double				  get_lock_time()  const { return select_time; }

		ObjectID    		  get_target_ID() const {return target_ID;}
        ObjectID              get_preferred_target_ID() const { return preferred_target_ID; }
        void                  set_preferred_target_ID(ObjectID id) { preferred_target_ID = id;}
		void                  set_objects_lock(unsigned what) { use_objects = what;};
        unsigned              get_objects_lock() const { return use_objects;}
	protected:
		void  lock_ground(float h_angle,float v_angle);
		bool  lock_object(float h_angle,float v_angle,bool (*handler)(viObject *item, viSearch *search, void *data));
		void  unlock();

        void  start_tracking(ObjectID id_,float h_angle,float v_angle);

        template <class pos>   const pos & get_position(double h_angle,double v_angle) const;
	    template <class pos>   const pos & get_position() const;
  
        void  insert_target(woPointer & ptr);

        static bool  technics_handler(viObject *item, viSearch *search, void* data);
		static bool  aircraft_handler(viObject *item, viSearch *search, void* data);
		static bool  missile_handler (viObject *item, viSearch *search, void* data);

        dVector       cockpit_local_point;

		target_map search_cache;

		ObjectID target_ID;
        ObjectID preferred_target_ID; 
		dVector       target_point;

		double       update_time;

		double       select_time;

		double       v_angle_shift;
		
        unsigned      use_objects;
		unsigned char mode;
		PolarPosition pole_position;

		static enum_MissilesLock missiles_lock_;
	};
}

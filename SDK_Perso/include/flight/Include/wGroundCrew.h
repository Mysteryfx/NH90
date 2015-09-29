#ifndef __wGroundCrew__
#define __wGroundCrew__

#include "Flight.h"
#include "Comm/wCommunicator.h"
#include "wTime.h"
#include "wResourceProvider.h"
#include "Utilities/Parameter.h"
#include "WorldSound.h"

class wMessage;
class IwoLA;
class wGroundCrew;
class IwWarehouse;

enum GroundCrewCommandType  
{
	COMMAND_VOID = 0,
	COMMAND_REARM, 
	COMMAND_RELOAD_CANNON, 
	COMMAND_REFUEL, 
	COMMAND_CHANGE_PILOT_EQUIPMENT, 
	COMMAND_CHANGE_POWER_SOURCE,
	COMMAND_GROUND_POWER_ON_OFF,
	COMMAND_LADDER,
	COMMAND_REPAIR,
	COMMAND_RUN_INERTIAL_STARTER,
	COMMAND_WHEEL_CHOCKS_ON_OFF,
	COMMAND_TYPE_MAX
};

enum CockpitDeviceType
{
	COCKPIT_DEVICE_HMS,
	COCKPIT_DEVICE_NVG
};

enum PowerSourceType
{
	POWER_SOURCE_TURBO_GEAR,
	POWER_SOURCE_REGULAR_LAUNCH
};

enum FuelMass
{
	FUEL_MASS_25_PERCENT,
	FUEL_MASS_50_PERCENT,
	FUEL_MASS_MAXIMUM_FERRY,
	FUEL_MASS_MAXIMUM_COMBAT,
    FUEL_MASS_75_PERCENT,
    FUEL_MASS_100_PERCENT,
};

struct  FLIGHT_API wGroundCrewCommand
{
	wGroundCrewCommand();
	bool operator == (const wGroundCrewCommand & command) const;
	bool operator != (const wGroundCrewCommand & command) const;
	GroundCrewCommandType type;
	Parameter			param;
};

typedef ed::list<wGroundCrewCommand> GroundCrewCommandQueue;

class wGroundCrewActivity : public wTime
{
public:
	wGroundCrewActivity() {owner = NULL;}
	wGroundCrewActivity(wGroundCrew *owner_) {owner = owner_;}
	virtual void NextEvent();
protected:
	wGroundCrew *owner;
};

class wGroundCrewReload : public wGroundCrewActivity
{
public:
    wGroundCrewReload(wGroundCrew *owner_,
                                 int    _count,
                                 double _dt,
                                 const wGroundCrewCommand & _command):wGroundCrewActivity(owner_),
                                 stage_count(_count),
                                 stage(0),
                                 dt(_dt),
                                 command(_command){}
    virtual void NextEvent();
private:
    int     stage;
    int     stage_count;
    double  dt;
    wGroundCrewCommand command;
};


class wGroundCrewActivityRefuel : public wGroundCrewActivity
{
public:
	wGroundCrewActivityRefuel(wGroundCrew *owner_,
								 double _dt,
								 const wGroundCrewCommand & _command):wGroundCrewActivity(owner_),
								 dt(_dt),
								 command(_command){}
	virtual void NextEvent();
private:
	double  dt;
	wGroundCrewCommand command;
};

class wGroundCrewResourceCheckActivity  : public wGroundCrewActivity
{
public:
	wGroundCrewResourceCheckActivity() : wGroundCrewActivity(), command_type(COMMAND_VOID) {;}
	wGroundCrewResourceCheckActivity(wGroundCrew *owner_, GroundCrewCommandType command_type_) {owner = owner_; command_type = command_type_;}
	virtual void NextEvent();
private:
	GroundCrewCommandType command_type;
};

class wGroundCrewOwner;



class FLIGHT_API wGroundCrew :	public wCommunicator
{
public:
	wGroundCrew();
	wGroundCrew(wGroundCrewOwner * owner_, IwoLA *la_);	
	~wGroundCrew();
	
	void setOwner(wGroundCrewOwner *owner_);
	wGroundCrewOwner *getOwner() const {return owner;}
	void setLA(IwoLA *la_);
	IwoLA *getLA() const {return la;}
	static wGroundCrewCommand getGroundCrewCommand(wMessage * pMessage);
	void onActivityEvent();
    bool reloadActivityEvent (const wGroundCrewCommand & command, int stage);
    void reloadActivityFinish(const wGroundCrewCommand & command);

	bool refuelActivityEvent (const wGroundCrewCommand & command,double dt);
	void refuelActivityFinish(const wGroundCrewCommand & command);
	
	void notify_connect();
	void notify_disconnect();
	void notify_move();
	bool is_connected() const {return connected;}
	void stop();
	//wCommunicator interface begin
	virtual void	setTransmitCommNetFlags(wCommNetFlags flags);
	virtual void	startMessageTransmition(const wMessagePtr & message);
	virtual void	processMessage(const wMessagePtr & message);
	//wCommunicator interface end
    int  get_queue_size() const { return command_queue.size(); }
    wGroundCrewCommand  get_current_task() const { return !command_queue.empty() ? command_queue.front() : wGroundCrewCommand(); }
	
	static bool		acquired_point_check(const dVector & local_pnt,const MovingObject * gto);
	static bool		acquired_point_check(const MovingObject * client	  ,const MovingObject * gto)
	{
		dVector   pnt		  = get_point_of_serviceable_object(client,gto);
		return acquired_point_check(pnt,gto);
	}
	bool			acquired_point_check() const;

	static dVector	get_point_of_serviceable_object(const MovingObject * client,const MovingObject * gto);
	dVector			get_point_of_serviceable_object() const;

    const dVector & acquired_point() const { return where_acquired; }
	virtual bool isMessageLegible(const wMessagePtr & message, bool moment) const;

    void request_task(wGroundCrewCommand tsk);
private:
	bool  checkRefuelCommandAvailability(const wGroundCrewCommand & command,float * requested_fuel_volume = NULL,float * available_fuel = NULL);
	float getRequestedFuel(const wGroundCrewCommand & command) const;

    bool is_busy() const;
    void next_command();
    void start_command(const wGroundCrewCommand & command);
    void free_activity();
    void generic_message(wMsg msg);
	wGroundCrewOwner*	owner; //helipad, airdrome, aircraft carrier
	GroundCrewCommandQueue	command_queue;	
	wtPointer				activity;
	wtPointer				resource_check_activity;
	IwoLA*					la;
	bool					connected;

    int                     move_notifier_counter;
    int                     move_notifier_time;
    dVector                 where_acquired;
    bool                    have_acquired_point;


	bool                    ground_power_connected;
	bool                    wheel_chocks_installed;
    bool                    turbo_gear_connected;


	Sound::Host				_sound_host;
	Sound::Source			_speech;
	void sound_init();
	void sound_update();
	void sound_get_params(Sound::HostParams* params);
};

typedef ed::list<wGroundCrew*> wGroundCrewList;

enum wGroundCrewOwnerAction {ACTION_NONE, ACTION_ADD_GROUND_CREW, ACTION_REMOVE_GROUND_CREW};

class wGroundCrewOwnerActivity : public wTime
{
public:
	wGroundCrewOwnerActivity() : owner(NULL), param(NULL), action(ACTION_NONE) {;}
	wGroundCrewOwnerActivity(wGroundCrewOwner * owner, wGroundCrewOwnerAction action_, void * param = NULL);
	virtual void NextEvent();
//private:
	wGroundCrewOwner * owner;
	wGroundCrewOwnerAction action;
	void * param;
};

class FLIGHT_API wGroundCrewOwner
{
public:
	class ResourceProvider : public wResourceProvider
	{
	public:
		struct FLIGHT_API Params : public wResourceProvider::Params
		{
		public:
			typedef ed::vector<float> wMaintenanceDurationVector;		
		public:
			Params() { clear(); }
			virtual void load(Lua::Config & config);
		private:
			void load_maintenance_duration_();
		public:
			wMaintenanceDurationVector	maintenance_duration;		
		};
		enum ResourceName { RESOURCE_AMMO, RESOURCE_FUEL, RESOURCE_ELEC_POWER, RESOURCE_GROUND_SERVICE, RESOURCE_MAX };
		ResourceProvider();
		virtual	const wResourceProvider::Params * get_params() const { return &params; }
		virtual viFlag				is_objects_required() const;
		virtual const unsigned int	get_resource_qty() const;
		virtual	void				clear_cash() const;
		virtual	void				after_update() const;	
		virtual bool				get_resource(unsigned char resource_name, unsigned int * obj_ID, float * value = NULL) const;
		virtual float				on_resource_expenditure(unsigned char resource_name, float value = 0.0f);

        void                        set_warehouse(IwWarehouse * warehouse_) { warehouse = warehouse_;}

#ifdef DEBUG_TEXT
		virtual ed::string			get_resource_name_str(unsigned char resource_name) const;
#endif
		static FLIGHT_API Params	params;
	private:
        IwWarehouse                * warehouse;
	};

	wGroundCrewOwner();
	~wGroundCrewOwner();
	virtual wGroundCrew *		call_ground_crew(IwoLA *la, bool now = true);
	virtual void				free_ground_crew(IwoLA *la);
	wGroundCrew *				add_ground_crew(IwoLA * la);
	void						remove_ground_crew(wGroundCrew * groud_crew);
	wGroundCrew *				get_ground_crew(IwoLA *la) const;	
	void						on_activity_event(wGroundCrewOwnerActivity * a);
	void						clear();
	virtual bool				check_resources_by_command(GroundCrewCommandType command_type, float * value = NULL) const;
	virtual bool				is_operational() const;
	virtual float				on_resource_expenditure(GroundCrewCommandType command_type, float value);
	virtual float				on_resource_expenditure(unsigned char resource_name, float value = 0.0f);
	virtual bool				check_resource(unsigned char resource_name, unsigned int * obj_ID = NULL, float * value = NULL) const;
	virtual MovingObject*		getObject();


    void                        setWarehouse(IwWarehouse * warehouse,bool set_as_owner = false);

    const IwWarehouse *          getWarehouse() const { return warehouse; };
    IwWarehouse *                getWarehouse()       { return warehouse; };
protected:
	static ResourceProvider::ResourceName get_resource_name(GroundCrewCommandType command_type);
protected:
	ResourceProvider*			resourceProvider;
	wGroundCrewList				ground_crew_list;
	typedef ed::list<wGroundCrewOwnerActivity*> wGroundCrewOwnerActivities;
	wGroundCrewOwnerActivities	activities;
    
    ResourceProvider            groundCrewResourceProvider;


    IwWarehouse                * warehouse;
    bool                        warehouse_owned;

};

#endif __wGroundCrew__
#pragma once

#include "wMovingObject.h"
#include "IwInfo.h"
#include "wsInitData.h"
#include "iInternalCargo.h"

class IWing;
class IwoAIPilon;
typedef ed::vector<IwoAIPilon*> Pylons;
struct StaticObjectData;

#include "Math\Vector.h"

enum  woLA_light_collection_enum
{
    WOLALIGHT_STROBES    ,//must be collection
    WOLALIGHT_SPOTS      ,//must be collection
    WOLALIGHT_NAVLIGHTS  ,//must be collection
    WOLALIGHT_FORMATION_LIGHTS,//must be collection
    WOLALIGHT_TIPS_LIGHTS,//must be collection
};

namespace plane_lights
{
    class woLA_LightCollection;
}

class wAircraftGunMount;
class wCommunicator;
class IwTransiver;
class IwWarehouse;
class wAirbase;
class wCargo;


struct ConstantForceBuffer
{
	ConstantForceBuffer():
	world_space_moment(0,0,0),
	local_space_moment(0,0,0)
	{	
		life_time = DBL_MAX - 1.0;
	}
	struct entry
	{
		entry():
		val(0,0,0),
		pos(0,0,0) 	{}
		Math::Vec3d val;
		Math::Vec3d pos;

	};
	entry   world_space_force;
	entry   local_space_force;

	Math::Vec3d	world_space_moment;
	Math::Vec3d	local_space_moment;

	double		life_time;
};

enum wcCoalitionName;

class IwoLA 
{
public:
	virtual ~IwoLA(){}

	virtual const Aircraft_Descriptor * getDescriptor() const = 0;
	virtual bool				GetFlagEjection() = 0;
	virtual const ed::string &	GetBoardNumber(void) const = 0;
	virtual int					GetWingNumber() = 0;
	virtual const wcCoalitionName GetCountry() const = 0;
	virtual const unsigned char	GetState()  = 0;
	virtual	bool				getPersonage() = 0;
	virtual int					getN_obj() const = 0;		// Номер самолета в звене
	virtual bool                isUnmanned() const = 0; //БПЛА

								//задать отказ с вероятностью на интервале времени (для триггеров, инфу из редактора ЛА сам считывает)
	virtual void				setFailureProbability(const ed::string& name, double probability, double start_, double interval_) = 0;
								//задать массу груза в грузовом отсеке
	virtual void				setInternalCargoMass(double cargo_mass) = 0;
								//снять отказ
	virtual void				resetFailure(const ed::string& name) = 0;

	virtual void				killPilot() = 0;

	virtual cVector			 getN() = 0;		// Вектор перегрузки
	virtual unsigned		 GetHookedCords() = 0;	// битовая маска зацепленных тросов
	virtual const   Vector3& GetHookPos() = 0;	// координаты конца крюка в ЛСК самолета

	virtual void			 RadarModeMessage	   (ObjectID client, unsigned int RadarType_, unsigned int RadarMode_, float dist_) = 0;
    virtual void			 LaserModeMessage	   (ObjectID client, unsigned int LaserType_ ) = 0;

    virtual float            getEngineRelativeRPM(int num, bool Core = true) const = 0;
    virtual float            getEngineRelativeThrust(int num) const = 0; //1 - left , 2 - right
	virtual bool			 isEngineStarted() const = 0;

	virtual MovingObject*	 getMovingObject() = 0;
	virtual const MovingObject*	 getMovingObject() const= 0;

	virtual int		  getFlagExternalGun() = 0;
	virtual void      setFlagExternalGun(int flag) = 0;

    virtual const plane_lights::woLA_LightCollection * get_lights() const = 0;
    virtual       plane_lights::woLA_LightCollection * get_lights()       = 0;

	virtual bool	get_gun_active(int8_t station /*on board = -1*/, int8_t gun_num = 0/*gun num*/) const = 0;
	virtual	bool	get_gun_fire(int8_t station /*on board = -1*/, int8_t gun_num = 0/*gun num*/) const = 0;

	virtual void	start_gun_fire(int8_t station /*on board = -1*/, int8_t gun_num = -1/*gun num*/) = 0;
	virtual void	stop_gun_fire(int8_t station /*on board = -1*/, int8_t gun_num = -1/*gun num*/) = 0;
	virtual void	sync_gun_temperature(int8_t station /*ofn board = -1*/, int8_t gun_num = -1/*gun num*/) = 0; //send the real gun temperature to the network fantom

	virtual cPosition get_gun_position(int8_t station = -1, int8_t gun_num = 0, wModelTime time = 0.0) = 0;
	virtual cPosition get_gun_position_local(int8_t station = -1, int8_t gun_num = 0) = 0;
	virtual void	  damage_gun(int8_t gun_number, int failure, bool failure_status, float azimuth_deviation = 0.0, float elevation_deviation = 0.0) = 0;

	virtual wAircraftGunMount* getDefaultGunMount() = 0;
	virtual wAircraftGunMount* get_gun_mount(int8_t gun_mount_ind, int8_t gun_num = 0) const  = 0;

	virtual float get_gun_firerate(int8_t station, int8_t gun_num = 0) const = 0;
	virtual void select_gun_firerate(uint8_t rate_index, int8_t station, int8_t gun_num = 0) = 0;

	virtual void acceptBulletImpulse(const dVector & pos, const dVector & imp) = 0;

	virtual void on_gun_fired(int8_t gun_mount_ind, int8_t gunIndex_) = 0;
	virtual void on_burst_started(int8_t gun_mount_ind, int8_t gunIndex_) = 0;
	virtual void on_burst_finished(int8_t gun_mount_ind, int8_t  gunIndex_) = 0;

	/* PILONS */
	virtual cPosition get_pilon_position(int8_t station) = 0;
	virtual cPosition get_pilon_local_position(int8_t station, int8_t substation = -1) = 0;

	virtual Pylons & getPylons() = 0;
	virtual void	 rotate_pilon(uint8_t station, uint8_t axis, float angle) = 0;
	virtual void	 rotate_pilon_force(uint8_t station, uint8_t axis, float angle) = 0;
	//////////////////////////////////////////////////////////////////////////

	virtual IWing	* getWing() = 0; 

	virtual ObjectID  FireWeapon(ObjectID id_, unsigned char clip_number,const cVector& t_vec, ObjectID t_id,int substation, bool armed = true)= 0;
	virtual ObjectID  FireMissileSubstation(ObjectID id_, int station,  int substation, ObjectID t_id_, const cVector* t_vec = 0, bool armed = true) = 0;

	virtual ObjectID	getCurrentLandPlaceID() const = 0;
	virtual bool		isPilotPresent() const = 0;
	virtual bool		checkTechCommandCondition(int command) = 0;// проверить возможность выполнения техниками данной команды
	virtual void        send_tacan_status_message(bool, unsigned long long, float) = 0;	//по сети
	virtual void        sendDatalinkTransiver(const ed::string & TN, int number, bool leader, bool EPLRS, bool listen, const wCommunicator & datalinkCommunicator) = 0;	//по сети
	virtual void		sendVoiceCommTransmitter(IwTransiver * transiver) = 0;
	
	virtual wCommunicator *		getCommunicator() = 0;
	virtual int					getCallSignNumber()	= 0;
	virtual bool can_takeoff(wAirbase * base) const = 0; //проверить возможность взлёта с base

	virtual void setSignalFlareCount(unsigned char color, unsigned char count) = 0;
    virtual unsigned char getSignalFlareCount ( unsigned char color ) const abstract;
	virtual void dropSignalFlare(unsigned char color) = 0;	

    virtual float       getCanopyValue() const = 0;
	virtual int         getCanopyMoveDirection() const = 0;
	virtual void        setCanopyMoveDirection(int valIn) = 0;
	virtual float		getCockpitAudibility() const = 0;

	virtual float		getGearPostPos(int post_id) const = 0;

	virtual void        ApplyPayload(const rearm_payload * p_rearm) = 0;
	// isJettison == false means the canopy was teared off
	virtual void		dropCanopy(bool isJettison) = 0;
    virtual float       Mfuel_full() const = 0;
	virtual float		Mfuel()  const = 0;
	virtual float		getM_Fuel_0() const = 0;
	virtual void		destroy(bool immediately = false) = 0;

    virtual float		Mfull()  const = 0; // полный вес = пустой + топливо + подвески
	virtual float		Mempty()  const = 0; // полный вес = пустой + топливо + подвески
    
    virtual iInternalCargo * getInternalCargo() = 0;

	virtual void		return_resources_to_warehouse(IwWarehouse * warehouse,bool return_self = true) = 0;

	virtual void		player_made_a_payload_choice(const players_payload_choice & payload) = 0;

	virtual bool		isUnderCivilRegistration() const = 0;

	virtual const Position3		*getConnectorPos(EffectConnectorType type) = 0;


	virtual ConstantForceBuffer		&   getNamedConstantForceBuffer(size_t key)
	{
		static ConstantForceBuffer  dummy;
		return dummy;
	}

	virtual void	removeNamedConstantForceBuffer(size_t key)
	{
	}

	virtual const IwoAIPilon * get_weapon_station(unsigned i) const = 0;

    //External Cargo
    virtual void  startCargoHooking(ObjectID cargoID) = 0; // вызывается ыпри выборе груза в меню
    virtual void  stopCargoHooking() = 0; // вызывается при отмене выбора груза
    virtual void  choosedCargo(ObjectID cargoID) = 0; // вызывается при подцепе груза
    virtual void  unhookCargo() = 0; // вызывается при удачной/неудачной отцепке груза
    virtual void  clearCargoObjects() = 0;
    virtual void  tearCargoOff() = 0; // вызвает при аварийном сбросе груза
};

#ifndef __wRadioBeacon__
#define __wRadioBeacon__

#include "WRadio.h"
#include <ed/list.h>

#include "cPosition.h"
#include "IwRadioBeacon.h"
#include "LandObjects/lSiteElement.h"

#include "Utilities/wMorzeString.h"

#include "Physic/wRadioTransmitter.h"
#include "Physic/wRadioAntenna.h"

class wRadioBeaconsBuilder;
class Parameter;
class wCommNet;

namespace Lua
{
	class Config;
}

struct BeaconElementData
{
	unsigned int ID;
	ed::vector<unsigned int> device_dependencies;
};

#define IID_WRADIOBEACON Common::identify<wRadioBeacon>()

//Radio beacon
class  WRADIO_API wRadioBeacon :public IwRadioBeacon,
								public Registered,
								public lSite,
								public lSiteElement,
								public Suicide
{
private:
	//Beacon's device
	class Device
	{
	public:
		Device();
		Device(wRadioDevice * pRadioDeviceIn);
		virtual void	activate(bool on);
		wRadioAntenna	radioAntenna;
		wRadioDevice *	pRadioDevice;
	};
	//Beacon's transmitter
	class Transmitter : public Device, public wMessage::Sender
	{
	public:
		Transmitter(wRadioSignals signalsIn, wRadioDevice::Place * place);
		virtual ~Transmitter();
		virtual void	activate(bool on);
		void			setMessage(const wMessagePtr & pMessageIn);
	public:
		wRadioTransmitter	radioTransmitter;
		wRadioSignals		signals; //Signal to transmit
	protected:		
		wMessagePtr			pMessage; //Message to transmit
		static wCommNet *	beaconsNet;
	};
	typedef ed::vector<Transmitter*>	Devices;
public:
	static RandomObject randomEvenly;
	wRadioBeacon(void);
	~wRadioBeacon(void);
	//IwRadioBeacon interface
	void			activate(bool flag_in, bool first = false);
	BeaconType		get_type() const  { return beacon_type; }
	double			get_frequency() const	   { return frequency;}
	bool			activated() const		   { return active;}	
	const char  *	get_callsign(bool morze = true) const;
	const char  *	get_name() const {return beaconName.c_str();}
	int				get_system_id() const { return systemID;}
	cPosition  &	Position(wModelTime t);
	//lSite interface
	virtual float	get_param_value(unsigned int param_num);
	virtual void	clear_site(bool full = false);
	virtual void	on_element_disable(lSiteElement*);
protected:
	friend class	wRadioBeaconsBuilder;
	void			set_owner(viObject * pOwnerIn)				{ pOwner = pOwnerIn; }
	void			set_beacon_type(BeaconType type_)			{ beacon_type = type_;}
	void			set_frequency(float f)						{ frequency = f;}
	void			set_position(const cPosition & posIn)		{ pos = posIn; }
	void			set_name(const ed::string & str);
	void			set_callsign(const ed::string & str);
	void			set_phase_shift(float phase_shift_)			{phase_shift = phase_shift_;}	
	//Beacon's devices
	void			init(int system_ID); //Initialize the transmitters by system identifier
	void			clear_devices();
	Transmitter &	addDevice(unsigned int device_ID, wRadioSignals signalsIn);
	void			removeDevice(unsigned int ID);
	static void		set_beacons_builder(wRadioBeaconsBuilder * beacons_builder_);
private:
	ed::string		getFullName_() const;
	double			frequency;
	bool			active;
	BeaconType		beacon_type;
	int				systemID;
	ed::string		beaconName;
	wMorzeString	callsign_morze;	
	ed::string		callsign_alphabetical;
	float			phase_shift; //Message phase shift
	viObject *		pOwner; //Beacon's owner, usually owner is an unit
	cPosition		pos; //Position of the beacon: the middle position of site elements or position of the owner or specific point
	class RadioDevicePlace;
	RadioDevicePlace * radioDevicePlace;
	Devices			devices;
	//Site elements
	ed::map<lSiteElement*, BeaconElementData> site_elements;
	static wRadioBeaconsBuilder * beacons_builder;
};

struct lua_State;
class lSiteElement;

typedef ed::map<unsigned int, ed::string> wBeaconSiteShapesMap;
typedef ed::map<unsigned int, lSiteElement*> wBeaconSiteElementsMap;

class wRadioBeaconsBuilder
{
public:
	wRadioBeaconsBuilder();
	~wRadioBeaconsBuilder();
	void	loadBeacons(); //Load beacons data from database (Beacons.lua)
	void	activateBeacons(); //Activate beacons
	void	clearBeacons();
	
	//Create beacon and create objects of its beacon site
	IwRadioBeacon *	create_beacon(const ed::string & name,const ed::string & callsign, BeaconType type, float frequency, const cPosition & pos, int system_ID, int site_ID, wBeaconStatus status = BEACON_ACTIVE, bool dynamic_site_building = false);
	
	//Create beacon with the pOwnerIn as a body
	IwRadioBeacon *	create_child_beacon(const ed::string & name,const ed::string & callsign, BeaconType type, float frequency, viObject * pOwnerIn, int system_ID);

	void	check_and_set_system_ID(BeaconType beacon_type, bool child, int & system_ID);
	void	check_and_set_site_ID(BeaconType beacon_type, int & site_ID);

	//Build beacon site site_ID
	void	build_site(wRadioBeacon * beacon, int system_ID, int site_ID, wBeaconSiteElementsMap * shapes_map = NULL);
	void	build_site(BeaconType beacon_type, const cPosition & pos, int system_ID, int site_ID, wRadioBeacon * beacon = 0, bool killed = false, wBeaconSiteElementsMap * shapes_map = 0);	
	
	lua_State * get_lua_State() const {return L;}
private:
	wRadioBeacon * create_beacon_(const ed::string & name,const ed::string & callsign, BeaconType type, float frequency, int system_ID);
	wBeaconSiteShapesMap get_site_shapes_(int system_ID, int site_ID);
	bool	find_beacon_site_map_objects_(const cPoint & pos, int system_ID, int site_ID, wBeaconSiteElementsMap * elements = NULL);
	lua_State * L;
	wModelTime morze_dot_duration;
	wModelTime morze_repeation_cycle_length;
	typedef ed::list<IwRadioBeacon*> Beacons;
	Beacons beacons;
};

#endif
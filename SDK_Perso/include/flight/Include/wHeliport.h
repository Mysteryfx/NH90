#ifndef _W__Heliport_h__
#define _W__Heliport_h__

#include "Flight.h"
#include "wAirbase.h"
#include "wMovingObject.h"
#include "LandObjects/lLandHeliport.h"
#include "wGroundCrew.h"
#include "Exception.h"

#define IID_IHeliport Common::identify<wHeliport>() 

class woATC;

enum wModulation;

namespace Lua
{
    class Config;
}

class FLIGHT_API wHeliportActivity : public wTime
{
public:
    wHeliportActivity(const woPointer & o):obj(o){}
	virtual ~wHeliportActivity() {}
	virtual void NextEvent();
private:
	woPointer obj;
};

struct wHeliportState
{
	wcCoalitionName			coalitionID;
	unsigned char			stateID;
	ed::string				missionID;
	E2::Vector				point;
	float					yaw;
	int						callsign;
	int						modulation;
	float					frequency;
	ed::vector<ObjectID>	inZoneAircrafts;
};

class FLIGHT_API wHeliport : public wAirbase
{
public:
	SIMPLE_SUBEXCEPTION(Exception, ::Exception);
	class ResourceProvider : public wResourceProvider
	{
	public:
		enum { RESOURCE_LIGHT_ILLUMINATION = 0 };
		struct FLIGHT_API Params : public wResourceProvider::Params
		{
		public:
			virtual void load(Lua::Config & config);
		};
		static FLIGHT_API Params params;
		inline bool							check_resource() const { return wResourceProvider::check_resource(RESOURCE_LIGHT_ILLUMINATION); }
	private:
		virtual viFlag						is_objects_required() const;
		virtual const unsigned int			get_resource_qty() const;
		const wResourceProvider::Params	*	get_params() const;		
#ifdef DEBUG_TEXT
		virtual ed::string					get_resource_name_str(unsigned char resource_name) const;
#endif
	};
public:
	wHeliport();
	virtual ~wHeliport();
    static wHeliport* create(Lua::Config & config, wcCoalitionName coalitionID, int stateID);
	static wHeliport* create(ObjectID id, const wHeliportState & state);
	
	virtual bool netGetCreate(Mail::Message& msg) const;
	void	getNetState(wHeliportState & state) const;

	int		Class(void)		{ return woClass_Heliport; }
	void	Control();

	void	contolIllumination();
	void	switchIllumination(bool on_off);
	
	bool	isRunWayOpen() const;
	const cPosition & GetRunwayPosition(int iRunWay = 0) const;
	virtual void	onMessage(ObjectID client, ATCMessage message);
#ifdef DEBUG_TEXT
	virtual const char * debug_text();
#endif	
	inline lLandHeliport * getHelipad() const { return landObject; }
protected:
    void	setHeliport(const wHeliportState & state);

	//Указатель, на объект отвечающий за отрисовку площадки
	void	setLandHeliport(lLandHeliport*);
	lLandHeliport *			landObject;
	ed::vector<ObjectID>	inZoneAircrafts;
	ResourceProvider		heliportResourceProvider;
	wtPointer				pHeliportActivity; 
#ifdef DEBUG_TEXT
	mutable char			debugTextStr[2000];
#endif
};

#endif

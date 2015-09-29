#pragma once

#include "Inter.h"
#include <ed/string.h>
#include <ed/vector.h>

struct lua_State;

class doCockpit;
class doDifficulty;
namespace Lua { class Config; }

// Graphics preferences
struct INTER_API doGraphics 
{
	unsigned char	gTexturing;				// Уровень использования текстур
    int             screenX;
    int             screenY;
    bool            fullscreen;
	ed::string		civTraffic;
} ;

struct INTER_API doTerrain
{
  enum { trHigh, trMedium, trLow } ;
  int textureResolution ;

  enum { dvFar, dvMedium, dvNear } ;
  int detailsVisibility ;

  enum { edHigh, edMedium, edLow, edNone } ;
  int effectsDensity ;

  doTerrain() ;
  doTerrain( const doTerrain& terrain ) ;

} ;

// Константы для заполнения doGraphics::gTexturing
enum dogTexturing_e {
	dogTexturing_Off,						// Выключены
	dogTexturing_2,							// 2 Mb
	dogTexturing_6,							// 6 Mb
	dogTexturing_12,						// 12 Mb
	dogTexturing_256						// Unlimit
};

class INTER_API IOptionsUser
{
public:
    virtual void setOptions(Lua::Config& config) = 0;
};

struct INTER_API OptionsUserEntry
{
    IOptionsUser* user;
    ed::string path;
};

enum units_visibility_enum
{
    UNIT_VIS_ONLYMAP,
    UNIT_VIS_ONLYSELF,
    UNIT_VIS_ALLIES,
	UNIT_VIS_ONLYALLIES, // panov возможно понадобится.
    UNIT_VIS_ALL,
};

class INTER_API  doDifficulty
{ 
public:
    doDifficulty()
    { 
        padlock         = false;
        easyRadar       = false;
        setGlobal       = false;
        immortal        = false;
		alliedFlightReports = true;
        geffect         = "realistic";
        easyFlight      = false;
        permitCrash     = false;
        radio           = false;
        tips            = true;
        fuel            = false;
        aftSwitching    = false;
		easyComm		= false;
        cockpitLanguage = "russian";
        units           = "metric";
        minihud         = false;
        iconsTheme      = "russian";
        map             = true;
        weapons         = false;
        labels          = false;
		birds_probability = 100;

        units_visibility  = UNIT_VIS_ALL;

		combatView = true;
		externalViews   = true;
		awacsView = true;
		freeCameraView = true;

        ground_aim_helper = true;
        ground_platform_shake = false;
        ground_target_info = true;
        ground_indicator = 1;
		cockpit_visual_recon_mode = true;
    }
public:
    bool padlock;
    bool easyRadar;
    bool setGlobal;
    bool immortal;
	bool alliedFlightReports;
    bool easyFlight;
    bool permitCrash;
    bool radio;
    bool tips;
    bool fuel;
    bool aftSwitching;
	bool easyComm;
    bool minihud;
    bool map;
    bool weapons;
    bool labels;
	int	 birds_probability;

    int  units_visibility;

	bool freeCameraView;
	bool combatView;
	bool awacsView;
	bool externalViews;

    // опции сложности в наземке
    bool ground_aim_helper;       // прицельная марка
    bool ground_platform_shake;   // тряска платформы
	bool ground_target_info;      // инфо о цели
	int  ground_indicator;        // тип индикатора компас-корпус-башня
	bool ground_automatic_transmission; // автоматическая коробка передач
	bool cockpit_visual_recon_mode;

    ed::string geffect;
    ed::string units;
    ed::string iconsTheme;
    ed::string cockpitLanguage;
};

class  INTER_API  doCockpit 
{
public:
    doCockpit():
    m_Mirrors(false),
    m_MirrorsResolution(1)
    {}
public:
    bool			m_Mirrors;		// Включение зеркал в кабине (пока не работают)
    int				m_MirrorsResolution;
};

class  INTER_API  doMiscellaneous 
{
public:
    doMiscellaneous():
      synchronize_controls(false),
      head_movement_initial(false),
	  accidental_failures(true)
      {}
public:
    bool synchronize_controls;
    bool head_movement_initial;
	bool subtitles;
	bool accidental_failures;
    ed::string coord_display;
};


class INTER_API IOptions 
{
public:
	virtual ~IOptions() = 0;
	virtual void			ParseGraphicsOptions() = 0;
	virtual int	            GetResolutionX() = 0;
	virtual int             GetResolutionY() = 0;
	virtual bool            GetNoEffect() = 0;
	virtual void            SetNoEffect(bool) = 0;
	virtual bool            Init() = 0;
	virtual bool            FullScreen() = 0;
	virtual short           GetCockpitMirrorsResolution() = 0;
	virtual bool            GetCockpitMirrors() = 0;
    virtual bool            isMetricSystem() const = 0;
	virtual const doGraphics	  & GetGraphicsOptions() = 0;
    virtual const doCockpit       & GetCockpitOptions() const = 0;
    virtual const doDifficulty    & GetDifficultyOptions() const = 0;
    virtual const doMiscellaneous & GetMiscellaneousOptions() const = 0;
    virtual void            SetConfig_(lua_State * L, const ed::vector<OptionsUserEntry>& users) = 0;
} ;

extern INTER_API IOptions *globalOptions;

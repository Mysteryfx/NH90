#ifndef __IwInfo_h__
#define __IwInfo_h__

#include <ed/map.h>
#include <ed/list.h>
#include <ed/vector.h>
#include <ed/unordered_map.h>
#include <ed/set.h>


#include "WorldGeneral.h"
#include "wsType.h"
#include "cLinear.h"
#include "../../math/Include/GraphMath/vector.h"
#include "wSFM_Data.h"

struct TaxiRoute;
struct TaxiPoint;

struct Aircraft_Descriptor;
struct Mach_H;
struct Rocket_Const;
struct Bomb_Const;
struct NURS_Const;
struct Vehicle_Const;
struct Ship_Const;
struct TaxiRoute;
struct wcAWACS_info;
struct WeaponGuidance_Const;
class wsPilonLoad;
struct wShape;

typedef ed::unordered_map<ed::string, unsigned int>		ObjectType;
typedef ed::map<unsigned int, ed::string>				TypeObject;
typedef ed::map<unsigned char, wsType>					wsTypeInfoMap;

// информация об анимации элементов подвески в процессе пуска
struct PylonDrawInfo
{
	int		arg_num;
	float	rate_out;
	float	rate_in;
	float	arg_ch_range[2];
	float	init_value;
};

struct container_shape
{
	container_shape()
	{
		animation_info = nullptr;
	}
	PylonDrawInfo * animation_info; 
	wsType		    type;
	ed::string      shape;
};

struct pos_and_arg_info
{
	pos_and_arg_info()
	{
		arg = 0;
	}
	Vector3 pos;
	Vector3 rot;//degrees	
	float   arg;
	ed::string connector_name;
};


// Постоянные балочных держателей
struct RackInfo
{
    wsType							loadout_;
	container_shape					adapter;// adapter 
	int								Num_of_Units_;   //  amount of subelements
	ed::vector<pos_and_arg_info>	pos_and_arg;
	ed::vector<container_shape>		second_level_adapters;// adapter 
	bool							show_only_container;
	float							container_arg_value;
};

typedef ed::unordered_map<ed::string,RackInfo*> wRacksMap;


// информация об анимации элементов подвески
struct ContainerDrawInfo
{
	// диапазоны вращения элемента(ов)
	float rot_x[2];
	float rot_y[2];
	float rot_z[2];
	
	float arg_x_ch_range[2];
	float arg_y_ch_range[2];
	float arg_z_ch_range[2];

	// номера аргументов
	int arg_x;
	int arg_y;
	int arg_z;

	int arg_c;
	float init_c;

	// скорость изменения аргумента (градусы/сек или единицы аргумента/сек)
	float rate_x;
	float rate_y;
	float rate_z;
	 
	// начальное состояние подвески по этому аргументу (состояние готовности)
	bool init_state;
};

typedef ed::map<unsigned char, ContainerDrawInfo*> Containers_draw_Map;

//////////////////////////////////////////////////////////////////////////
struct RocketLauncherDrawInfo
{
	ed::vector<int> rockets_lau_args_; //arguments of the launcher tubes
};

typedef ed::map<unsigned char, RocketLauncherDrawInfo*> RocketLauncher_draw_Map;

// постоянные условий разгрузки КМГУ
struct ClusterInfo
{
	float prelaunch_time;
	float block_unload_time;
	int   blocks_count;
	int   elements_in_block;
	unsigned char element;
	unsigned char element_void;
};

typedef ed::map<unsigned char, ClusterInfo*> ClusterMap;

// цвета и координаты дымогенераторов
struct SmokeInfo
{
	int alpha;
	int r;
	int g;
	int b;
	float dx;
	float dy;
};

typedef ed::unordered_map<ed::string, SmokeInfo*> SmokeMap;

// информация об аэродинамике в SFM
//typedef ed::list<float> SFM_aerod_list;

struct cells_properties;

typedef ed::unordered_map<ed::string, int> damage_cells_map;

//Коэффициенты для модели повреждений летательных аппаратов
typedef struct _damage_la_data
{
	_damage_la_data(){max_distance = 100.0; curvature = 300.0f; total_damage = 0.5; use_new_damage = false; use_shield = true; pow = 0.333f;}

	float	max_distance;
	float	curvature;
	float	total_damage;
	float	pow;
	bool	use_new_damage;
	bool	use_shield;
}
damage_la_data;

// Коэффициенты алгоритма прицеливания AI в БВБ
//
// Элементы массива (тангаж/крен/путевое управление):
// controller[0] - коэфф. усиления
// controller[1] - коэфф. демпфирования

// информация об АВАКС
typedef ed::map<unsigned char,wcAWACS_info*> awacs_info_map;

// тип боевой части
enum WarHeadType
{
	htNone,			// не наносит повреждений
	htKinetic,		// болванка - только кинетический удар
	htExplosive,	// нормальный снаряд с тратилом
	htCumulative,	// кумулятивный заряд (бронебойный)
	htBetab,		// бетонобойный заряд
};

struct rearm_pylon_data
{
    ed::string LauncherCLSID;
    int         Pylon;
};

struct FlightFormation
{
	bool allowZInverse;
	enum { MAX_VARIANTS = 3 };
	size_t variantCount;
	size_t defaultVariantIndex;
	float pos[3][3][MAX_VARIANTS];
};

typedef ed::vector<FlightFormation> FlightFormations;
typedef ed::list<rearm_pylon_data>  rearm_payload;

struct players_payload_choice
{
	unsigned		numChaff;
	unsigned		numFlare;
	float			currentAmmo;
	unsigned		ammoType;
	double			fuel;
	rearm_payload	pylons;
};

class MovingObject;
struct lua_State;

class WORLDGENERAL_API IwInfo
{
public:
    virtual lua_State * getDatabaseState() = 0;
	virtual ~IwInfo() = 0 {}

	virtual void reload() = 0;

	virtual const Aircraft_Descriptor*		GetAircraftDescriptor(const wsType & type) = 0;
	virtual const Aircraft_Descriptor*		GetAircraftDescriptor(const ed::string & unit_name) = 0;
	virtual const Aircraft_Descriptor*		GetAircraftDescriptorPlaceHolder(const ed::string & unit_name,const wsType & type) = 0;

    virtual Rocket_Const*		GetRocketData(const wsType & type) = 0;
    virtual Bomb_Const*			GetBombData(const wsType & type) = 0; 
	virtual RackInfo*           Get_RackInfo(const ed::string & launcher_clsid) = 0;

	virtual float				Get_ChangeTargetPrbCoeff(int i) const = 0;

	virtual Vehicle_Const*		GetTechnicsData(unsigned char A_Name) = 0;

    virtual const wsPilonLoad & getLoadoutInfo(const ed::string & clsid) = 0;
	virtual wsType				BombWsType (unsigned char Type) = 0;
	virtual wsType				ContainerWsType (unsigned char Type) = 0;
    virtual wsType				Get_wsType(int itbl, int ind) = 0;
	virtual wsType				getCivilWsType(const char* name) = 0;
	virtual wsType				MissileWsType (unsigned char Name) = 0; // дает тип ракеты по имени
	virtual wsType		        getWsTypeByName(const ed::string & name) const = 0 ;
    // Возвращает локализованое имя
    virtual ed::string         GetDisplayName(MovingObject* obj) = 0;
    // Возвращает нелокализованое имя
	virtual const char*			GetName(const wsType & type, bool uName = true) = 0; 
	virtual bool				Get_BigAC(const wsType & type) = 0; // true - большой самолет 
	virtual int					getCellIndex(const wsType & type, const ed::string& partName) = 0;
	
	virtual WarHeadType			getWarHeadType(const wsType& weapon) = 0;
	virtual const FlightFormation & getFlightFormation(unsigned char id) const = 0;
	virtual unsigned char		getDefaultFormationId(int groupType) const = 0;
	
	virtual ContainerDrawInfo*  Get_Control_container_draw_Info(unsigned char) = 0;
	virtual ContainerDrawInfo*  Get_Weapon_container_draw_Info(unsigned char) = 0;
	virtual RocketLauncherDrawInfo*  Get_Rocket_Launcher_draw_Info(unsigned char)= 0;
	virtual ClusterInfo*		Get_Cluster_Info(unsigned char) = 0;
	virtual SmokeInfo*			Get_Smoke_Info(const ed::string & clsid) = 0;

 	virtual	bool				is_MER(const wsType &) = 0;
	virtual	bool				is_dispenser(const wsType &) = 0;
	virtual	bool				is_MER_or_dispenser(const wsType &) = 0;
	virtual	bool				is_heavy_unguided(const wsType &) = 0;
	virtual	bool				is_mid_unguided(const wsType &) = 0; // С-13
	virtual	bool				is_marker_unguided(const wsType &) = 0; // маркерные НУРС
	virtual	bool				is_radar_pod(const wsType &) = 0;
	virtual	bool				is_pattern_guided(const wsType &) = 0;
	virtual	wcAWACS_info*       get_AWACS_info(const wsType &) = 0;
 
	virtual const damage_la_data *get_damage_la_data()const = 0;

	virtual float				  getMassByType(const wsType & tp) const = 0;
};

extern	WORLDGENERAL_API IwInfo* globalInfo;

#endif

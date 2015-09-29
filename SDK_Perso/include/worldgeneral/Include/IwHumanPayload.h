#pragma  once 

#include "WorldGeneral.h"
#include "cLinear.h"

class wAircraftGunMount;
class wsType;

struct  targetForLaunch
{
	unsigned int  id;
	cVector		  pnt;
};

class wAmmunition;
class wWeapon;

typedef std::function<void(wAmmunition*,wWeapon*)>  fuzingDataSet;

class WORLDGENERAL_API IwHumanPayload
{
	public:
		virtual ~IwHumanPayload() = 0;
		// Поиск
		virtual	void FindAA(void)				= 0;	// Поиск оружия типа воздух-воздух
		virtual	void FindFI0(void)				= 0;	// Поиск оружия для FI0
		virtual	void FindAG(void)				= 0;	// Поиск оружия типа воздух-земля
		virtual	void FindAntiradar(void)		= 0;	// Поиск оружия типа ПРГСН
		virtual	void FindSemiActiveRadar(void)	= 0;	// Поиск оружия типа ПАРГСН
		virtual	void FindRadarGuided(void)		= 0;	// Поиск оружия типа ПАРГСН || АРГСН
		virtual	void FindStandAloneAA(void)		= 0;	// Поиск оружия типа ТГСН || АРГСН
		virtual void FreeSelection(void)		= 0;
		// Перебор
		virtual	void NextAA(void)               = 0;	// Выбор следующего типа воздух-воздух
		virtual	void NextAG(void)               = 0;	// Выбор следующего типа воздух-земля
		virtual	void NextFI0(void)              = 0;	// Выбор следующего оружия для FI0
		virtual	void NextAntiradar(void)        = 0;	// Выбор следующего оружия ПРГСН
		virtual	void NextSemiActiveRadar(void)  = 0;	// Выбор следующего оружия ПАРГСН
		virtual	void NextRadarGuided(void)      = 0;	// Выбор следующего оружия ПАРГСН || АРГСН
		virtual	void NextStandAloneAA(void)     = 0;   // Выбор следующего оружия ТГСН || АРГСН
		virtual void NextWeapon(void)			= 0;	// любое следующее вооружение

		virtual void NextBomb(void)				= 0;	// любое следующее вооружение
		virtual void NextUnguidedRockets(void)	= 0;	// любое следующее вооружение

		// Информация
		virtual	const wsType &	Type		 (int n = -1) const = 0;	// Тип подвески на пилоне (с аргументом по умолчанию - на выбранном)
		virtual	const wsType &	ContainerType(int n = -1) const = 0;	// Тип контейнера на пилоне (с аргументом по умолчанию - на выбранном)
		virtual	const wsType &	AdapterType  (int n = -1) const = 0;	// Тип адаптера на пилоне (с аргументом по умолчанию - на выбранном)

		virtual	const ed::string &	LauncherCLSID  (int n = -1) const = 0;	// Тип адаптера на пилоне (с аргументом по умолчанию - на выбранном)

		virtual	int		Count		         (int n = -1) const = 0;	// Общее количество оружия этого типа
		virtual	int		ContainerCount       (int n = -1) const = 0;		// Общее количество контейнеров этого типа
		virtual	int		MaximumCount         (int n = -1) const = 0;	// Общее количество оружия этого типа
	
		virtual	int		get_current_weapon_count_on_pilon(int = -1) const = 0;	// Общее количество оружия этого типа

		virtual	int		get_maximum_weapon_count_on_pilon(int = -1) const  = 0;
		virtual int		get_current_unloaded_count_on_pilon(int = -1) const  = 0;	// количество оружия на пилоне
		virtual cVector	get_current_pilon_coord(int = -1) const = 0; //координаты пилона

		//по текущему пилону
		virtual	bool	isAirToAir(int i = -1) const			= 0;
		virtual	bool	isPinpoint(int i = -1) const			= 0;	// высокоточное оружие (laser, TV homing ракеты и бомбы, противокарабельные ракеты)
		virtual	bool	isAntiship(int i = -1) const			= 0;	// противокарабельные ракеты
		virtual	bool	isTVhoming(int i = -1) const			= 0;	// TV homing ракеты и бомбы
		virtual	bool	isSemiActiveRadar(int i = -1) const		= 0;	// ПАРГСН
		virtual	bool    isActiveRadar(int i = -1) const			= 0;	// АРГСН
		virtual	bool    isRadarGuided(int i = -1) const			= 0;	// АРГСН || ПАРГСН
		virtual	bool    isInfraredSeeker(int i = -1) const		= 0; // ТГСН
		virtual	bool    isStandAloneAA(int i = -1) const		= 0; // ТГСН || АРГСН
		virtual	bool	isLaserHoming(int i = -1) const	= 0;	// Laser homing ракеты  и бомбы
		virtual	bool	isAntiradar(int i = -1) const			= 0;	// antiradar missile
		virtual bool    isUnguidedRocket(int i = -1) const      = 0; // НАР'ы
		virtual bool    isBomb(int i = -1) const				= 0; // бомбы
		virtual bool    isPatternGuided(int i = -1) const       = 0; // ракеты с наведением по растру лазерно-лучевой системы
		virtual bool    isGunPod(int i = -1) const    = 0;
		virtual int     get_current_pilon() const =0;

		// Пуск
		virtual	unsigned int	Launch(unsigned int target, bool salvo)					    = 0;	// Пуск выбранного оружия по цели
		virtual	unsigned int	Launch(unsigned int target, const cVector &  target_pnt ,bool radio_fuse_armed = false,bool immediate_explode = false) = 0;	// Пуск выбранного оружия по цели

		virtual	unsigned int	LaunchAG(unsigned int target, bool salvo,const cVector & target_pnt) = 0;	// Пуск выбранного оружия по цели
		virtual	unsigned int	LaunchStation(unsigned StationIn, unsigned int target, const cVector & target_pnt) = 0;	// Пуск/сброс с произвольной подвески
		virtual	unsigned int	LaunchStation(int station,int substation,unsigned int target, const cVector & target_pnt, bool armed = true) = 0; // Пуск/сброс с произвольной подвески
		virtual void            stop_unloading() = 0;

		virtual	int		get_nFlare(int group = -1) const = 0;
		virtual	int		get_nChaff(int group = -1) const = 0;
		virtual int     get_pilon_count() const = 0;
		virtual bool    isTheSameLoad(int i) const = 0;

	//	virtual void    change_pilon_state(int pilon,uint8_t axis,float value) = 0;
		virtual void    set_pilon_argument(int pilon_number,int argument,float value) = 0;
		virtual float   get_pilon_argument(int pilon_number,int argument) const = 0;

		virtual void    set_active_station(int new_station) = 0;
		virtual int     launch_multiple_pilons(const cVector & target,int count_of_pilons) = 0;
		virtual int     launch_multiple_pilons(const cVector & target,const ed::vector<int> & pilons) = 0;
		virtual void    update_target_point(const cVector & target) = 0;

		virtual bool    emergency_jettison()=0;
		virtual void    emergency_jettison(int pilon)=0;
		virtual void    emergency_jettison(int pilon, int count_to_drop)=0;
		virtual void    emergency_jettison_rack(int pilon)=0; //for A10
		virtual bool    get_unlimited_weapon() const =0;
		virtual void    set_unlimited_weapon(bool flag_in)=0;


		virtual wAircraftGunMount*   getDefaultGunMount() = 0;
		virtual wAircraftGunMount*   getGunMount(int8_t gun_number, int8_t station = -1/*-1 - onboard*/) = 0;
		virtual	int8_t				 get_gun_mount_cnt(int8_t station = -1) = 0; /*returns gun mount on the [station]*/

		virtual unsigned int		 LaunchStation(int station,int substation,const targetForLaunch & target,fuzingDataSet settings)= 0;
};

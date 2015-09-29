//                    file woCar.h
#ifndef __woCar_h__
#define __woCar_h__

#include "Transport.h"
#include "woMovingVehicle.h"
#include "wDetectable.h"
#include "wActivityManager.h"
#include "ArgumentAnimation.h"
#include "wWeaponSystemManager.h"
#include "wFloat.h"
#include "GraphicEffect.h"
#include "wNetObj.h"
#include "gTimer.h"
#include <ed/vector.h>
#include "Comm/wTransiverOwner.h"
#include "Comm/wNetCommunicator.h"
#include "Comm/wDatalinkCommunicator.h"
#include "CarsSound.h"
#include "../../Flight/Include/wGroundCrew.h"
#include "wWarehouse.h"
#include "Transportable.h"

#include "HumanPlayable.h"

#include "Crew/CrewManager.h"

#include "InfantryAnimator.h"

class CarSounderWrapper;
class InfantrySounderWrapper;

class cLauncher;

namespace Lua { class Config; }

class wcColumn;
class woCar;
class wCommunicator;
class wDatalinkCommunicator;

const wModelTime EPLRS_UPDATE_INTERVAL = 15.0;

#define IID_ICar Common::identify<woCar>()
#define CAR_DRAW_ARGS_MAX	51 //50 // 23
#define	MAX_ARM_CAR			8

namespace PSEffects {class EffectHolder;}
class TRANSPORT_API woCar: public woMovingVehicle, public wWeaponSystemManager, wDetectable
{
private:

    class woCarWarehouse : public wGroundCrewOwner
    {
        woCar* _owner;

    public:
        woCarWarehouse(woCar* owner) : _owner(owner) {  }
        virtual ~woCarWarehouse() {	if (warehouse) delete warehouse; }
        void create_warehouse(const ed::string& mission_id);
    };

    bool                _is_warehouse;
    woCarWarehouse*     _warehouse;
protected:
    bool searchForAvailableWarehouse();
#ifdef _ED_HIDDEN_DEBUG 
	ed::string DebugClassName; // Имя объекта для отладки
#endif
public:

	woCar();
	virtual ~woCar();

	struct InitCar
	{
        ed::string     type;
		int				N_obj;
		wcCoalitionName	coalition;
		unsigned char	state;
		int				skill;
		double			beginX, beginZ, beginYaw, beginV;
		wModelTime		startTime;
		ed::string     name;
        ed::string     missionID;
        float           life;
        float           life0;
		bool			visible; // видимость шейпа машинки до старта
		// andreev {
		bool			playerCanDrive;
		// andreev }
        wsFlags         flags;
		bool			isRandomTransportable;

        InitCar()
        {
            flags = 0;
        }
	};

	static woCar*		create(InitCar* dataCar, wcColumn *pC);

    //physCar interface
	int                 getGearNum()		const;
	bool                getHandbrake()		const;
	int					getEngineRPM()		const;
	int					getNextGearNum()	const;
	bool				getCruiseControl()	const;
	bool				getAcceleratorState() const;
	bool				getBrakeState()     const;

	void				resetPhysCarParams();
	void				syncPhysCarParams();

	struct PhysCarParams
	{
		bool turboFlag;
		bool isAutomaticTransmission;
	};

	PhysCarParams* physCarParams;

    void                setColumn(const InitCar* dataCar, wcColumn *pC);
    void                setColumn(wcColumn *pC){pColumn = pC;};
    int					Class(void) {return woClass_Vehicle;}
	const bool			LinkToTarget(const unsigned int TargetID, const unsigned int weapon_type = 0);
  	void				PostCollisionProcessing(wsCollisionData *, unsigned int);

	void				setTimeWaitExt(wModelTime time);

	virtual RadarMode   isRadiatedByRadar(ObjectID targetID, float & radar_detection_distance) const;

	cPosition&			Position(wModelTime t);
	cPosition&			Position();
	const cPosition&	Position(cPosition& p);
    dVector		        GetPoint();
    dVector		        GetPoint(wModelTime t);
    virtual void        get_swings(double& swing_p, double& swing_v, double& swing_r) const;

    virtual inline bool     can_manual() const;
	bool				isPlayerCanDrive() const;
    virtual const ed::string&   getCockpitName() const;
    bool                hasDriverView() const;

	//машинка управляется игроком (на своём компьютере или в сети)
	bool				isHumanControlled() const;
	//оружие машинки управляется игроком в сети или локальным
	//если для машинки включён автопилот, то isHumanControlled() == false
	//isWeaponHumanControlled() == true
	int					weaponControlHostId() const;
    virtual void PrintTrace();	// Печать текущего состояния объекта в wTime

	// парсинг
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects=0);
	const Graphics::DParamList& GetDrawArguments() { return dparams; }

	void				setDrawArgumentValue(int index, float value);

	unsigned char		getName(void)			{return obj_type.Level4();}
	const float			Roll()					{return roll;}
	const float			Tang()					{return pitch;}
	const float			Course()				{return yaw;}
	float				turnSpeed()				const {return Wy;}
    bool				OnRoadCar(void)			{return OnRoad;} // Движение объекта по дороге или вне дороги
	bool				GetCaterpillar()		const {return (Gear_Type == 2);}
    bool                can_TacticalDir()       const {return myMobility;}
    virtual float		getHeight()	const       {return Height;}
	wControl*			GetCommander(void) const;
	wCommunicator *		getCommunicator();
	virtual IwDatalinkTerminal*	getDatalinkTerminal() const;
	int					getN_obj() const		{return N_obj;}
	wcColumn*			GetColumn()				{return pColumn;}
	bool				getMyMobility()			{return myMobility;}
	void				SetIdPusk(unsigned int id_);
    void				netDestroy();
    void				netActivate( int32_t flag );
	void				netActivateAI( int32_t flag );

    void                RegisterCar(){viRegister();}
    void                UnregisterCar(){viUnregister();}
    void                initPositionCar(float x, float z, float bearing, bool checkObjectsHeight){initPosition(x, z, bearing, checkObjectsHeight);}

	void				sendDatalinkStatusMsg(bool onOff, const ed::string & ownTN);
	void				procDatalinkStatusMsg(bool onOff, const ed::string & ownTN);
	void				commCreateUpdate(int callsign, float power, float frequency, float bandWidth, wModulation modulation);
	void				netCommCreateUpdate(int callsign, float power, float frequency, float bandWidth, wModulation modulation);
	void				commChangeCommNetFlags(wCommNetFlags transmitFlags, wCommNetFlags receiveFlags);
	void				netCommChangeCommNetFlags(wCommNetFlags transmitFlags, wCommNetFlags receiveFlags);
	void				commStartMessage(const wMessagePtr & message);
	void				commFinishMessage(bool onTime);
	void				netCommStartMessage(const wMessageData & messageData);
	void				netCommFinishMessage(bool onTime);

    virtual bool        master_arm_on();
    virtual bool        master_arm_off();
	virtual bool        is_master_arm_on() const;
    virtual bool        is_master_arm_off() const;

    virtual dVector     getDetectionPoint(); // положение точки детектирования в мировых координатах
    virtual dVector     getAimPoint(); // положение точки прицеливания в мировых координатах
    virtual dPosition   getCameraViewPoint(bool);

	float				GetKillRadius(const cVector & pos_, const wsType & type_, float * height_max = NULL);
    virtual bool        IsTarget(unsigned id) const { return IsTargetAssigned(id);}

	float				getMinRangeFindingTarget()const{return minRangeFindingTarget;}
	float				getHeightFindingTarget()const{return heightFindingTarget;}
	float				getMinHeightFindingTarget()const{return minHeightFindingTarget;}

    ////// FOR F10 view only, don't use it for AI code
    virtual float           getKillingDistance()const{return wWeaponSystemManager::getKillingDistance();} //used for F10 circles only!
    virtual float           getKillingDistanceMin()const{return wWeaponSystemManager::getKillingDistanceMin();} //used for F10 circles only!
    virtual float		    getFindingDistance()const{return wWeaponSystemManager::getFindingDistance();} // used for F10 circles only!
    /////

	virtual const Ammo & getAmmo() const;

    int                 startRearming();
	int				    reArmingFromWH();
    virtual int         RearmingAllowed() const;

	virtual bool		Is_Target();			// true - цель
	virtual float		Life() const;
	virtual const float	Life(float l);
    virtual const float getRelativeLife() const { return (life-1.0)/(life0-1.0); }
	virtual void		destroy(bool immediately = false);
	virtual int			getSkill() const { return skill; }

	// ручное управление
	virtual void		SetCommand(const SwitchEPLRS * pSwitchEPLRS);

	virtual void		CommandSet(int command);
	virtual void		CommandSet(int command, float value);

	// отдача
	virtual void		recoil(int, const cVector& power,const cVector & point);

	struct ImpactData
	{
		Vector3 point1, point2;
		MovingObject* pGun;
	};
    void				createControlActivity( int32_t flag = 0 ); // стартоая функция для инициализации деятельностей. вызывает колонной.

    virtual void		formDrawArgument_sim(); // формирование аргументов, критических для симуляции
    virtual void		formDrawArgument_nonsim(); // формирование аргов, которые на симуляцию не влияют
    float               getScanRadarDirection() const; // для отрисовки положения сканирующего луча на радаре в кабине
    void				nextTime(wModelTime& timeStep);
    void				uninitObject();

	virtual wDetectable* getDetectable();
	//wDetectable interface begin
	virtual bool  getVolumeEffectBox(Math::Box2f * box = NULL) const;
	virtual float getIllumination(const cVector & vec) const;

	float getTemperature(float cos_target_aspect_angle) const
	{
		return 20.0f;
	}

	float getTemperature(const cVector & vec) const
	{
		return getTemperature(1.0);
	}

	virtual float getRCS(float lambda, const cVector & vec) const;
	//wDetectable interface end

	virtual void        setAI_ON();
	virtual void        setAI_OFF();

    void                Degrade(); // тихо убиваемся и создаём битую модельку

    Transportable       getTransportable(){return transportable;}

private:
	// ----------------------- Datalink ----------------------------------
	bool                    is_EPLRS_enable;
	wTransiverOwner	        dlinkTransiver;
	wDatalinkCommunicator * datalinkCommunicator;
	wModelTime              datalink_time;
	void                    sendPPLI();
	void                    createDatalink(const ed::string & ownTN); // if get_old_id = true then use old SADL identification

	// ----------------------------------------------------------------------------

    wCommunicator *     m_pCommunicator;

	void                explode();
	void				resetMoveActivity();
	void				destroy_();

	void				humanNextStep(wModelTime& timeStep);
	void				onNetHumanTakeControl(bool, bool, int);

	//машинка управляется игроком на своём компьютере
	inline	bool		humanDriver() const;

	void				humanCheckImpact(wModelTime timeStep);
    //table consts
    osg::Vec3f			fireOffset;
	osg::Vec3f			dustOffset;
    float               m_fireSize;
    float               m_fireTime;
    float				weaponDistance;
    float				airWeaponDistance;
	float				weaponHeight;
    float				minRangeFindingTarget;
    float				heightFindingTarget;
    float				minHeightFindingTarget;
    float				Height;
    unsigned char		Gear_Type;
    float				R_max;
    float				R_track;
    float				toggleAlarmStateInterval; // период приведения в боевое состояние

	dVector				startPos;//стартовая позиция машинки (по сети передаётся отступ от неё)

	wModelTime			humanTimeRemainder;

    dVector             CustomAimPoint;
    bool                bCustomAimPoint;

    // водительское место
    static ed::string   driverViewCockpitName;
    ed::string          driverViewConnectorName;
    static Graphics::ModelConnector* driverViewConnector;
    dVector             driverViewPoint;

	//tmp data
	cPosition			posVisualize;

    ed::string         dstr_shape;
    ed::string         normal_shape;

	ArgumentAnimationCyclic	anmRadarRotation;
    wFloat			    argument5; // агония человечка

    bool                rotate_radar_to_default; //нужно ли приводить радар в исходное положение перед остановкой

    Transportable       transportable;//транспортабельность машинки

	// номера аргуметов анимации машинок
	 int			alarm_state;		// приведение в боеготовность
	 int			locator_rotation; 	// вращение поискового радара
     int            infantry_first_step; // аргумент анимации первого шага пехоты
     int			body_swing_pitch; 	// качание при движении по тангажу
	 int			body_swing_vertical; // качание при движении вверх-вниз
	 int			body_swing_roll;	// качание при движении по крену
	 int			paint_scheme;		// схема раскраски
	 int			agony_animation;	// умирание человечка
	 int			wheels_rotation;	// вращение колес
	 int			wheels_turn_angle; 	// угол поворота поворотных колес
	 int			tracks_rotation; 	// вращение гусениц
	 int			rollers_rotation; 	// вращение катков
     int            crewPresence; // наличие экипажа в машинке

	typedef				ed::vector<unsigned int> IdObject;
	typedef				wActivity<woCar> wActivityCar;
	wActivityManager	activities;

	bool				initObject(const InitCar* dataCar);
	void				humanControlTake();
    void				humanControlRelease();
    void                humanDriverControlTake( int controllerHostID );
    void                humanDriverControlRelease();

    void                rearming(wModelTime& timeStep);
    void				control(wModelTime& timeStep);
	void				netNextTime(wModelTime& timeStep);
    void				smoke_up(wModelTime& timeStep);
    void                manageLocatorRotation(wModelTime& timeStep);

	void				effects();
	void				drawLocator(wModelTime dTime);
    void				startLocator();
    void				stopLocator();
    void				stopLocatorToDefault();
	void				drawWheels(wModelTime dTime);
	void				drawTracks(wModelTime dTime);
	void				drawSwingBody(wModelTime dTime);
    void                calcSwingBody(wModelTime dTime);

	bool				check_fire_on_run(); // Определение собственной боеготовности
	void				SetDrawArg(unsigned int, float, float, float);
    void                SetDrawArg_cyclic(unsigned int ind, float alfa, float min, float max);

	bool				checkWorldObject(cVector targetPos);

	void				takeDamage(float deltaLife, ObjectID shooter, unsigned int weapon);
	void				beginAgony();
	void				agony(wModelTime& timeStep);
    void				agonyHuman(wModelTime& timeStep);
	bool				getAllowedDustEffect(ed::string& eff);

    wtPointer           activity_rearming;
	wtPointer           activity_control;
    wtPointer           activity_soundUpdate;
	wtPointer           activity_move;
    wtPointer           activity_smokeup;
    // запуск и остановка вращения тарелки обзорного радара перед приведением в походное/после приведения в рабочее положение
    wtPointer           activity_manageLocatorRotation;

	wcColumn*			pColumn;
	wNetObj				theNetCar;
	int					N_obj;
	bool				playerCanDrive;//разрешение/запрет управлять этой машинкой

	wModelTime			timeFromNetStop;
	wModelTime			lastNetUpdateTime;//время последнего апдейта,который дошёл до клиента

	// recoil
	float				recoil_cur, recoil_remaining;

	float				oldPsi;
	float				Wy;
	wModelTime			currentTimeAgony;
	wModelTime			lastOvercharge; // время последней перезарядки
	wModelTime			timeOvercharge; // интервал между выстрелами
	wModelTime			timeWaitExt;	// ожидание без движения по просьбе внешнего инициатора

	ed::string			lastDustTailEffect;
	Mem::Ptr<PSEffects::EffectHolder>		theDustTail;
	Mem::Ptr<PSEffects::EffectHolder>		theFire;

	float				RecoilComp [3];		// rx, ry, rz

	float				SuspensionParam [3];			//Параметры для подвески
	float				dSuspensionParam_dt [3];
	float				d2SuspensionParam_dt2 [3];
	float				old_v;						//Для вычисления продольного ускорения машинки
	bool				BangBang;					//флажок выстрела

	bool				myMobility;

	unsigned int		id_pusk;
	bool				engineStop;
	wModelTime			timeStop;
	bool				OnRoad;	// true - движение по дороге; false - внедорожное движение
	wModelTime			timeFormDrawArgument;
    wModelTime			timeFormDrawArgument2;

	wModelTime			timeBraking;
	wModelTime			timeAgony;

	int					humanDriverHostId;		//id клиента управляющего машинкой (для netGetHost)

	float				handBrake;
	double				steeringWheel;
	int					steeringWheelDir;	//дискретное управление рулем

    //bool         ground_platform_shake;   // тряска платформы, копия глобальной опции

    float       drawingPhase;
    float       swing_pitch; 	// качание при движении по тангажу
    float		swing_vertical; // качание при движении вверх-вниз
    float		swing_roll;	// качание при движении по крену

	Graphics::DParamList dparams;	// Новое  хранилище для аргументов рисования

	float lastImpactTime;
	float currentImpactTime;
	float deltaImpactTime;

#ifdef DEBUG_TEXT
	ed::string			dbgTextOut;
#endif
// SOUND {
	friend class CarsSound; // Дабы не быть Пабликом Морозовым

	CarsSound _sound;
    Sound::Source sndSrc_Hit;
	Sound::Source sndSrcSmoke;

    void sound_load(Lua::Config& conf);
    void sound_init();
    void sound_update(wModelTime& timeStep);
    void sound_update();
    void sound_uninit();

	void sound_smoke_play();

	// Новый звук

	friend class CarSounderWrapper;
	friend class InfantrySounderWrapper;

	CarSounderWrapper* carSoundWrapper;
	InfantrySounderWrapper* infantrySoundWrapper;

	// Новый звук

// } SOUND

// MAIL {
public:
	struct CarStatus
	{
		int offPos[3]; //offset from startPos - fixed point 24.8
		cVector vel;
		float time, yaw;

		unsigned char additionalDataMask;
		enum DataMask: unsigned char
		{
			hasPitchRoll = 0x1,
			hasWeaponStatus = 0x2,
			hasPosition = 0x4,
			isOnRoad = 0x8,
		};

		//Additional position data
		float pitch, roll;
        //Is emission enabled (EM, laser)
        bool emissionEnabled;
		//Addition weapon system pos data (for human controlled cars)
		WeaponStatusVec weaponStatus;
	};

	typedef CarStatus Status;

	cVector _vPrevPos;
	float _fPrevYaw;
	wModelTime _fPrevTime;
	bool _bStartPath;
	bool _bUpdateFromServer;

	struct InitNetCar
	{
		// netCreate
        ed::string	    type;
        ed::string	    missionID;
		ObjectID		objId;
		ObjectID		columnId;
		wcCoalitionName	coalition;
		unsigned char	state;
		unsigned char	skill;
		dVector			startPos;
		CarStatus		beginStatus;
		VoiceCommNetState voiceCommNetState;
		bool			voiceMessageTransmission;
		wMessageData	voiceMessageData;
		DatalinkState	datalinkState;
		// updateStatus
		float			deltaLife;
        bool            active; // создавать или нет модель для групп с отложеным стартом. по умолчанию false
        float           life;
        float           life0;
        __int32         flags;
		unsigned char	arms[MAX_ARM_CAR];
		int				humanDriverHostId;
		int				humanWeaponControlHostId;
		// netCreate
		bool			playerCanDrive;
	};

	void				netHackCoalition(lua_State* conf, const ed::string &strName, InitCar *pInitCar); // Hack!!!!!!
	static woCar*		netCreate(const InitNetCar&, Mail::Reader);
	void				netDiscard();
	int					netGetHost() const;

	virtual	void		updateStatus(const InitNetCar&);
	virtual bool		netGetCreate(Mail::Message&) const;
	virtual bool		netSendUpdate(int dest, int prio, int& cookie);

    // MAIL {
    void	            getDiffInitStatus(Mail::Writer& stream);
    void	            setDiffInitStatus(Mail::Reader& stream);
    // } MAIL

	bool				checkForUpdates( wModelTime fTime = 5.0f );
	bool				checkForChanges( wModelTime fTime = 0.0f );

protected:

	bool				setStatus(const Status&);
    void				getStatus(Status& data);
    void                setWeaponStatus(const Mail::Reader msg);
    void                getWeaponStatus(Mail::Writer& writer);

#ifdef DEBUG_TEXT
    virtual const char* debug_text();
    virtual const char* customlabel();
#endif
private:
	friend class woCarMail;
	static woCarMail	mail;

	ed::map<int, int> colorSmoke;

public:
	bool takeColorSmoke(int smokeId);

	int getOrangeSmokeCount() { return colorSmoke[1]; }
	int getRedSmokeCount() { return colorSmoke[2]; }
	int getGreenSmokeCount() { return colorSmoke[3]; }

	void sendColorSmoke(cVector point, int smokeCode); // Отправляем информацию по дыму.
	void receiveColorSmoke(cVector point, int smokeCode); // Получаем информацию по дыму и ставим дым.

	void spawnColorFlare(int flareColorCode);
	void receiveColorFlare(int flareColorCode);

// Управляемые игроком от первого лица человечки.
	friend class HumanPlayable;

	InfantryAnimator* getInfantryAnimator();

// Доступ к менеджеру экипажа
	CrewManager* getCrewManager() { return crewManager; }
private:

	HumanPlayable* humanPlayable;

	friend class InfantryAnimator;
	InfantryAnimator* infantryAnimator;

	bool canBeHumanPlayable;

// Управляемые игроком от первого лица человечки.

// Экипаж
	CrewManager* crewManager;
// Экипаж

}; // class woCar

#endif

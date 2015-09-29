#ifndef __woShip_h__
#define __woShip_h__

#include "Transport.h"
#include "wWeaponSystemManager.h"
#include "woShipEnum.h"
#include "wNetObject.h"
#include "../../Flight/Include/wAirbase.h"
#include "MovingPlatform.h"
#include "wDetectable.h"
#include "CoalitionInfo.h"
#include "wsFlightPlan.h"
#include "wConst.h"
#include "../../../WorldGeneral/Include/AI/Controller/Common/Tasks.h"
#include "../../Frogfoot/Include/Aircrafts/wNetPathData.h"

namespace Graphics
{
	class RenderParser;
	class Context;
	class Model;
}

namespace Effects
{
	class GraphicEffect;
}

namespace PSEffects {class EffectHolder;}

struct PathPointsNAV
{
	ObjectID unitID;
	ObjectID destID;
	ObjectID taskID;
	ed::vector<osg::Vec2d> vPointsPosition;
};


class wWorld;
class wcSquadron;
class wAirService;
class wNetObject;
class woShip;

struct wsCollisionData;

#define IID_IShip Common::identify<woShip>()
#define	MAX_ARM_SHIP		49
#define MAX_ELEMENT_SHIP	28
#define SPEEDUP_MAX			10000.0f
#define SHIELD_FLAGS_COUNT (4)
#define ARRESTING_WIRES_COUNT (4)

class woShipMail;

struct TargetPointNAV
{
	ObjectID unitID;
	ObjectID targetIconID;
	ObjectID targetLinkID;
	ObjectID taskID;
	ObjectID unitNearID;
	int nDetected;
	int nColumn;
	wsType	wTypeTarget;
	wConst	wTask;
	cVector vPosTarget;
};


//порядок наследования имеет значение, т.к. в деструкторах подсистем wWeaponSystemManager
//используется указатель на MovingObject (woPointer host) из wAirbase
class TRANSPORT_API woShip : public wAirbase, public wWeaponSystemManager, public MovingPlatform, public wDetectable
{
protected:
#ifdef _ED_HIDDEN_DEBUG 
	ed::string DebugClassName; // Имя объекта для отладки
#endif
#ifdef DEBUG_TEXT
    virtual const char * debug_text();
#endif
public:
	static const double RadarOffset;

	woShip();
	virtual ~woShip();

	struct InitShip
	{
        ed::string     type;
		int				N_obj;
		wcCoalitionName	coalition;
		unsigned char	state;
		int				skill;
		double          beginX,
						beginZ,
						beginYaw,
						beginV;
		float			offsetLiaderX,
						offsetLiaderZ;
		wModelTime		startTime;
		ed::string     name;
        ed::string     missionID;
		bool			visible; // видимость шейпа машинки до старта
        wsFlags         flags;

		InitShip()
		{
			coalition		= COALITION_NEUTRAL;
			state			= 0;
			skill			= 0;
			beginX			= 0.0f;
			beginZ			= 0.0f;
			beginYaw		= 0.0f;
			beginV			= 0.0f;
			offsetLiaderX   = 0.0f;
			offsetLiaderZ   = 0.0f;
			startTime		= 0.0;
			visible			= false;
            flags           = 0;
		}
	};

    virtual void        PrintTrace();	// Печать текущего состояния объекта в wTime

    static woShip*		create(InitShip* dataShip, wcSquadron* pSq);

    bool                setSquadron(const InitShip* dataShip, wcSquadron* pSq);
    void                setSquadron(wcSquadron* pSq) { pSquadron = pSq; };
    int					Class()				{ return woClass_Ship;}
	const bool			LinkToTarget(const unsigned int TargetID, const unsigned int weapon_type = 0);
  	void				PostCollisionProcessing(wsCollisionData *, unsigned int);

	const Position3& GetSystemPos(int index) {return pos3;}
	cPosition&			Position(wModelTime t);
	cPosition&			Position()	{return pos;}
    virtual void		recoil(int, const cVector& power, const cVector& pnt) {return;};
    void				netDestroy();
    void				netActivate();

	// получить точную координату объекта
	virtual dVector     GetPoint();
    virtual dVector		GetPoint(wModelTime t);

	// парсинг
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects=0);
	const Graphics::DParamList& GetDrawArguments() { return dparams; }

	void				setDrawArgumentValue(int index, float value);

	const float			Omx() const			{ return Omega.x;}
	const float			Omy() const			{ return Omega.y;}
	const float			Omz() const			{ return Omega.z;}
	const float			Roll()				{ return roll;}
	const float			Tang()				{ return pitch;}
	const float			Course()			{ return yaw;}

	virtual void		destroy(bool immediately = false);

	wControl*			GetCommander()  const;
	wcSquadron*			GetSquadron()					{ return pSquadron;}
	int					getN_obj() const				{ return N_obj; }
	wAirService*		getServiceSystem()				{ return theServiceSystem;}
	float				GetKillRadius(const cVector & pos_, const wsType & type_, float * height_max = NULL)	{ return airWeaponDist;}
    virtual float		getHeight()	const               { return Height;}
	float				getMaxShipVelocity()			{ return kvMax*vMax;}
    virtual void        setTopSpeedHandicap(double relMaxSpeed) {};
	void				setLunaPlane(MovingObject *plane);
	void				clearLunaPlane(MovingObject *plane);

	void				setTaskMotion(ShipTaskMotion);
	void				setIdLeader(unsigned int idLeader_) {idLeader = idLeader_;}
	void				ControlDisturbance(int new_letter);
	void				ControlActive(int new_letter);
	void				SetShieldFlag(int num, bool value);
    virtual bool		allowShoot() const ;

    virtual RadarMode   isRadiatedByRadar(ObjectID targetID, float & radar_detection_distance) const;
    float               getScanRadarDirection() const; // для отрисовки положения сканирующего луча на радаре в кабине

    virtual float       getKillingDistance()const{return threatRange;} //used for F10 circles only!
    virtual float		getFindingDistance()const{return detectionRange;} // used for F10 circles only!

	virtual const Ammo & getAmmo() const;

	virtual bool		Is_Target();			// true - цель
	virtual wDetectable* getDetectable();
	virtual float		getTemperature(cVector & vec) const;
	virtual float		getTemperature(float cos_target_apect_angle) const;
	virtual float		getRCS(float lambda, const cVector & vec) const;
	// ручное управление
	virtual void		CommandSet(int command);

// Цветные ракеты

	void spawnColorFlare(int flareColorCode);
	void receiveColorFlare(int flareColorCode);

// Цветные ракеты

    void				createControlActivity();

    virtual dVector     getAimPoint(); // положение точки прицеливания в мировых координатах

	//wAirbase interface begin
	const cPosition &	GetRunwayPosition(int iRunWay = 0) const;
	//
	bool				Is_FreeHelLanding(ObjectID id);
	bool				Glide_Slope(ObjectID id = 0) const;
	bool				isRunWayOpen() const;
	//
	void				AddATC(int callsign_, float frequency);
	woATC*				getATC() const;
	//wAirbase interface end
    int                 getNumParking() {return numParking;}; // количество позиций для старта самолетов

	void				onTouchDown(const woPointer & ptr);

    ////  MovingPlatform interace
	double				tacticalSpeed() const;
	void				setTacticalSpeed(double speed);

	void				setTacticalDir(const osg::Vec2d& dir);
	const osg::Vec2d&	tacticalDir() const;
	bool                tacticalDirArrived() const;
    bool                tacticalDirFixed() const;
	bool				isStaying() const;
    bool				isStayingEx() const;
    bool                can_tacticalSpeed() const {return false;};
    bool                can_TacticalDir() const {return false;};
    int                 getGearNum() const {return 1;};
	cVector				getGotoWaypoint();
    virtual void        get_swings(double& swing_p, double& swing_v, double& swing_r) const { swing_p = 0.0; swing_r = 0.0; swing_v = 0.0; };
    ////////////////////////////////////////////////////////////////////////

	virtual void        setAI_ON();
	virtual void        setAI_OFF();

    virtual void        crutchStopPitchingForHuman();



	virtual const RunWay *	Get_LandingRW() const;
	virtual const RunWay *	Get_TakeOff_RW() const { return Get_LandingRW(); }
	virtual TaxiRoute	 *	Get_Taxis();


	virtual TaxiPoint	 *	getCurrentRoute();

    virtual const float getRelativeLife() const { return (life-1.0)/(life0-1.0); }

protected:
    wModelTime          resumePitchingTimer;
	typedef				ed::list< Link<MovingObject> > ObstaclesList;
	typedef				wActivity<woShip> wActivityShip;
	wActivityManager	activities;
	wtPointer			NextTimeActivity;
	wtPointer			controlDrawArgumentActivity;
	wtPointer           activity_control;

	ed::list<woPointer> touch_down_planes;

	void				initTrail();
	bool				initObject(const InitShip* dataShip);
	void				uninitObject();

	void				control(wModelTime& timeStep);

	void				nextTime(wModelTime& timeStep);
	void				netNextTime(wModelTime& timeStep);

	void				formDrawArgument();
    virtual void		formDrawArgument_sim() {}; // формирование аргументов, критических для симуляции
	void				controlDrawArgument(wModelTime& timeStep);

	bool				bigShip() {return M > 750000;}

	void				executeTask(wModelTime timeStep);
	void				nextStep(wModelTime timeStep);
	void				motion(wModelTime timeStep);
	void				beginFlood();
	void				flood(wModelTime timeStep);
	void				waiting(wModelTime timeStep);
	void				controlLinearVelocity(wModelTime timeStep);
	void				controlAngularVelocity(cVector vectorStrivePoint, wModelTime timeStep);
	void				setOmega(cVector omega);
	void				takeDamage(float deltaLife, unsigned char element, const ed::string& area, ObjectID shooter, ObjectID weapon);

	void				controlLuna();
	void				drawCords(Graphics::RenderParser& parser, const Position3& pos, Graphics::effectState* effects);
	bool				drawCords(const woPointer		& ptr,
		 						  Graphics::RenderParser& parser,
								  const Position3&		  pos,
								  Graphics::effectState*  effects);

	void				initCords();
	unsigned			getCordCount();		// получить количество остановочных тросов
    void                loadCordAnchors(); // загрузка из скрипта
	bool				getCordAnchors(int cord, Vector3& left, Vector3& right); // получить координаты концов остановочного троса в ЛСК авианосца

	void				obstaclesProcessing();
	void				getListObstacles();
	void				analizeObstacles();
	void				checkCollisionWithSeeshore();
	bool				sitOnShoal();
	bool				isOnWater(unsigned char SurType);
	void				checkCollision(MovingObject *obj);
	void				updateObstacles(MovingObject *obj);
	void				runaroundFromCollision(MovingObject* theObstacle);

	virtual bool        master_arm_on();
	virtual bool        master_arm_off();
    virtual bool        is_master_arm_on() const;
    virtual bool        is_master_arm_off() const;
    virtual int         RearmingAllowed() const { return rearmerr_IsMoving; }

    mutable double	    aimingYCorrection;

    wcSquadron*			pSquadron;		// Указатель на эскадру
	int					N_obj;
	wAirService*		theServiceSystem;
	wNetObject			theNetShip;

	Mem::Ptr<PSEffects::EffectHolder>	theFire;
	Mem::Ptr<PSEffects::EffectHolder>	theSmoke;

	Effects::GraphicEffect	*shipTrail;
	BOOL				shieldFlags[SHIELD_FLAGS_COUNT];	// Флаги рисования щитов
    float				xOff;			// Смещение от флагмана по координате X в связанной с.к.
	float				zOff;			// Смещение от флагмана по координате Z в связанной с.к.
    float				kvMax;			// Коэффициент полноты хода
    float				kvMaxLast;
	cVector				Omega;			// Вектор угловой скорости.
    float				velReq;		// Заданная скорость
	BOOL				submersionStern;// true - дифферент на корму
	int					idLeader;		// id корабля лидера

	ArgumentAnimationCyclic	anmRadar1Rotation;
	ArgumentAnimationCyclic	anmRadar2Rotation;
	ArgumentAnimationCyclic	anmRadar3Rotation;


//  Из таблицы характеристик кораблей

	float				M;				// Водоизмещение, тонны
	float				levelDeck;		// Высота палубы, m
	float				v_gon;			// Гоночная скорость, узлы
	float				v_econ;			// Экономическая скорость, узлы
	float				Dist_econ;		// Запас хода при v_econ_
	float				Dist_gon;		// Запас хода при v_gon_
	float				speedup;		// Горизонтальное ускорение
	float				Gamma_max;		// Максимальный крен
	float				Om;				// Максимальная угловая скорость
	float				R_min;			// Минимальный радиус разворота
	float				X_nose;			// Координата буруна у носа, м
	float				X_tail;			// Координата начала пенного следа, м
	float				Tail_Width;		// Ширина пенного следа, m
	float				vMax;			// Максимальная скорость
	float				shipLength;	    // Длина корабля.
	float				airWeaponDist;	// Дальность действия оружия, km
	float				widthShip;			// Ширина корабля, m
	float				Height;			// Высота корабля, m
	float				RotateAcceleration;

	ed::string         normal_shape;

	cVector				vectorStrivePoint;
	cVector				velocityXYZ;
	float				distWarning;
	float				distFindObstacles;
	unsigned char		taskMotion;
	ObstaclesList		obstacles;
	wModelTime			currentTimeFlood;
	wModelTime			timeFormDrawArgument;
	cVector				firePos;
	BOOL				onShoal;
	BOOL				blast;
	cPosition			posVisualize;
    unsigned char		Plane_Num;
    unsigned char		Helicopter_Num;
    int                 numParking; // количество мест старта (катапульт)
	woPointer			luna_plane;	// самолет, который ведет ЛУНА
	Graphics::RenderObject *draw_cords[ARRESTING_WIRES_COUNT];	// тросы Авианосца

	// номера аргументов анимации корабликов

	int			        alarm_state;		// приведение в боеготовность
	float		        toggleAlarmStateInterval; // время приведения в боеготовность
	int			        radar1_rotation; 	// вращение 1-го радара
	int			        radar2_rotation; 	// вращение 2-го радара
	int			        radar3_rotation; 	// вращение 3-го радара
	int			        flag_animation; 	// размахивание флагом
	int			        water_propeller; 	// вращение винтов cудна на воздушной подушке
	int			        nav_lights; 		// Навигационные огни
	int			        luna_lights; 		// огни системы "Луна"

	int			        blast_fences[SHIELD_FLAGS_COUNT];
	int			        arresting_wires_arguments[ARRESTING_WIRES_COUNT];
    int                 crewPresence; // наличие экипажа

    struct ArrestingWirePoints{
        Vector3 left;
        Vector3 right;
    };
    ed::vector<ArrestingWirePoints> arrestingWires;

    Graphics::DParamList dparams;	// Новое  хранилище для аргументов рисования

#ifdef DEBUG_TEXT
	ed::string				dbgTextOut;
#endif


	void	StateSetUndefined();
	BOOL	IsStateUndefined();

// MAIL {
public:
	struct ShipStatus
	{
		float time, x, z, yaw, pitch;
		ShipStatus ()
		{
			pitch = time = x = z = yaw = 0.0f;
		}
	};
	typedef ShipStatus Status;

	struct InitNetShip
	{
		// netCreate
        ed::string     missionID;
        ed::string     type;
		ObjectID		objId;
		wcCoalitionName	coalition;
		unsigned char	state; // WTF? seems 'country' is really a 'coalition'
		unsigned char	skill;
		ShipStatus		beginStatus;
		float			beginV;
        bool            active;
		// updateStatus
		int				deltaLife;
        float           life;
        float           life0;
        __int32         flags;
		unsigned char	arms[MAX_ARM_SHIP];
		unsigned char	elements[MAX_ELEMENT_SHIP];
	};

	static woShip*		netCreate(const InitNetShip&, Mail::Reader);
	virtual	void		updateStatus(const InitNetShip&);
	virtual bool		netGetCreate(Mail::Message&) const;
	virtual bool		netSendUpdate(int dest, int, int&);

	//
	// IN: fTime - время, больше которого все равно делать update
	//
	// OUT: true  если изменилась позиция или направление или прошло времени с последнего обновления больше чем fTime
	//    : false если предыдущие данные такие же как и текущие и прошло времени с последнего обновления меньше чем fTime.
	//
	bool checkForUpdates( float fTime );


	virtual void setPathToAI( const PathPointsNAV&  );
	virtual void setPathUpdateAI( AI::Route * pRoute );
	virtual void setGotoWaypointIdx( int nIdx );
	virtual void sendGotoWaypointIdxAI( int nIdx );

	const AI::NetAIPath &getNetRoute();
	AI::NetAIControl &getNetControl();

	virtual void setRoeToAI( int nRoe );
	virtual void setRoeUpdateAI( int nRoe );
	virtual void setSpeedToAI( double dSpeed );
	virtual void setSpeedUpdateAI( double dSpeed );
	virtual void setFormationToAI( int nFormation );
	virtual void setFormationUpdateAI( int nFormation );

	virtual void setTargetToAI( const TargetPointNAV & );
	virtual void removeTargetToAI( ObjectID targetID );

	void _addPath( AI::Route *pRoute );
	void _addTask( AI::Task *pTask, wConst wZad, ObjectID targId, const cVector &vPos );
	AI::Task * createEmptyRouteMission( const cVector &vPos, ObjectID destinationID ) ;

protected:
	bool				setStatus(const Status&);
	void				getStatus(Status& data);
	void				destroy_();

	friend class woShipMail;
	static woShipMail	mail;

	Status  prevStatus;

	AI::NetAIPath _netAIPath;
	AI::NetAIControl _netAIControl;

// } MAIL

// SOUND {
    // TODO: refactor, woCar has the same set of sounds
    struct {
    Sound::Source       engine;
    Sound::Source       move;
    //Sound::Source       turn;
    Sound::Source       hit;
    } _snd;

    ed::string         fnameEngine;
    ed::string         fnameMove;
    void sound_load(Lua::Config& conf);
    void sound_init();
    void sound_update();
    void sound_uninit();
// } SOUND
};	// class woShip

#endif

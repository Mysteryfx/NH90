#ifndef __wMovingObject_h__
#define __wMovingObject_h__

#include "wsType.h"
#include "lPointOnSurface.h"
#include "cLinear.h"
#include "edTerrain/PointOnSurfaceAdapter.h"
#include "viMovingObject.h"
#include "wMovingObjectConst.h"
#include "wsFlags.h"

#include "IwNetObject.h"
#include "WorldSound.h"
#include "../../ed_api/public/ed/SounderAPI.h"

#include "wAttribute.h"
#include <memory>

class wINamedAmmunitionDescriptor;
class wControl;
struct wsCollisionData;
class wCommunicator;
class wDetectable;
class IwDatalinkTerminal;
enum wcCoalitionName;
enum RadarMode
{
	RADAR_MODE_NO  = -1, //no radar
	RADAR_MODE_OFF,
	RADAR_MODE_SCAN,
	RADAR_MODE_CENTERED_SCAN,
	RADAR_MODE_TRACKING
};

namespace AI
{
	class wPlayerTaskIndicator;
}

struct WORLDGENERAL_API wCamera : public Suicide, public Common::FakeUnknown<Serializable>
{
	wCamera();
	~wCamera();

	float dist;			// Расстояние до камеры внешнего вида (F2, F4, F5, F8)
	float hAngle;		// Горизонтальный угол камеры внешнего вида
	float vAngle;		// Вертикальный угол камеры внешнего вида
	float distChase;	// Расстояние до преследующей камеры (F6, F7)
	float dxChase, dyChase, dzChase;	// Смещения по осям
	bool  bFrom;
	bool  bLocal;
	float hAngleLocal;	// Углы локального вращения
	float vAngleLocal;
	float hAngleLocalChase;
	float vAngleLocalChase;
	float hViewAngleExternal;
	float hAngleRear;
	float vAngleRear;
	bool  bFromChase;
	bool  bFromRear;
    bool  bChaseWasSet;

	void serialize(Serializer& serializer);
};

class MovingObjectCommand;
class SwitchEPLRS;

struct AmmoItem
{
	const wINamedAmmunitionDescriptor * desc;
	wsType			type;
	unsigned int 	count;
};

typedef ed::vector<AmmoItem> Ammo;

class WORLDGENERAL_API MovingObject :	public viMovingObject, 
									    public IwNetObject,
                                        public wAttributeOwner,
                                        public Suicide
{
	// Пришлось декларировать иначе под интелом не линкуется.Mitka
	MovingObject(const MovingObject& arg);
	MovingObject& operator =(const MovingObject& arg);

public:
	MovingObject(); 
	MovingObject(bool master);

	virtual ~MovingObject();

	void log();
	virtual void PrintTrace(); // Печать текущего состояния объекта в wTime

	// поддержка интерфейса viObject
	virtual cPosition&	Position(wModelTime t);
	virtual void		ChangePos(const cPosition& newPos); // также обновляет информацию surfInfo

	virtual cPosition&			Position();
	virtual const cPosition&	Position(cPosition& p);
	virtual const Position3&	GetPosition();
	virtual	const float Hrad();
	virtual	const float Hbar();
	virtual const float Roll();
	virtual const float Tang();
	virtual const float Course();
	// получить точную координату объекта
	virtual dVector		GetPoint(wModelTime t);
	virtual dVector		GetPoint();

	const edterrain::PointOnSurfaceAdapter &	getSurfInfo() const { return surfInfo; }
	edterrain::PointOnSurfaceAdapter &	getSurfInfo() { return surfInfo; }

    // Cохранение и восстановление
	virtual void		serialize(Serializer &serializer);

	virtual	void		setDrawArgumentValue(int index, float value);

	virtual	int			Class();
	const wsType	&	Type() const;
	const wsType	&	Type(const wsType& t);
	const wcCoalitionName Coalition() const;
	const wcCoalitionName Coalition(const wcCoalitionName c);
	
	virtual	void		  setInitCoalition(const wcCoalitionName coalition)
	{
		Coalition(coalition);
	}

	const unsigned char	State(void) const;
	const unsigned char	State(const unsigned char s);

	wCamera* const		Camera(void);
	//Returns object's controller
	virtual wControl*	GetCommander(void) const;
	//Returns object's communicator
	virtual wCommunicator * getCommunicator();

	virtual IwDatalinkTerminal*	getDatalinkTerminal() const;
	virtual AI::wPlayerTaskIndicator * getPlayerTaskIndicator() const;
	virtual int	getN_obj() const;

	const bool			IsFlag(__int32 f) const;
	void				SetFlag(__int32 f);
	void				ResetFlag(__int32 f);
	void				ResetFlag();
	const wsFlags		Flags() const;

	virtual wDetectable* getDetectable() { return NULL; }
    virtual dVector     getDetectionPoint(); // положение точки детектирования в мировых координатах
    virtual dVector     getAimPoint(); // положение точки прицеливания в мировых координатах
    virtual dPosition   getCameraViewPoint(bool); // кастомная камера для каждого типа объекта

	virtual void		PostCollisionProcessing(wsCollisionData*, ObjectID);

	virtual const unsigned int	Target_ID() const;
    virtual       bool          IsTarget(unsigned id) const { return false;}
	virtual		  unsigned int	GunId() const;	// кто выпустил (eсли obj - снаряд, ракета...) Id
	virtual	const int			Task_Number();
	virtual int					getSkill() const { return 0; }

	virtual const bool	In_Air_() const;

	// связь объекта с целью (способность сопровождать и т.п.)
	virtual const bool	LinkToTarget(const unsigned int TargetID, const unsigned int weapon_type = 0);

	virtual const float Nx() const;
	virtual const float Ny() const;
	virtual const float Nz() const;

	virtual const float Omx() const;
	virtual const float Omy() const;
	virtual const float Omz() const;

	virtual	float		ScalarVelocity() const;
	virtual const cVector&	VectorVelocity() const;
	virtual const cVector& VectorVelocity(const cVector& v);
	const float Vy() const;

	virtual float		GetKillRadius(const cVector & pos_, const wsType & type_, float * height_max = NULL);
	virtual float		getFindingDistance() const;
	virtual float		getKillingDistance() const;
    virtual float		getKillingDistanceMin() const;

	//Is this radiated ID_ by radar and what is the radar detection distance (for detection by RWR)
    virtual RadarMode   isRadiatedByRadar(ObjectID ID_, float & radar_detection_distance) const;

	virtual float		Life() const;
	virtual const float	Life(float l);
	virtual float		Life0() const;
    virtual const float getRelativeLife() const;
	virtual bool		IsDead() const;
	virtual void		destroy(bool immediately = false);
    // Может ли объект совершать осмысленные действия (например, стрелять)
    // Пришло из wVulnerable, но луче бы ему тут не быть
    virtual bool        isEffective() const;
	virtual float		getFuel(bool internalOnly = false) const;
	virtual const Ammo & getAmmo() const;

	// Флаг для отключения\включения активностей юнита, через wsFlag_AI_ON. с отключеным AI юнит превращается в статик. 
	// для внешнего управления через триггеры.
	virtual bool        isAI_ON() const;
	virtual void        setAI_ON();
	virtual void        setAI_OFF();


	virtual bool		isHumanControlled() const {return false;};//управляется пользователем по сети

	// формирует позицию на поверхности по вектору нормали и направлению
	virtual void		formPositionOnSurface(cPosition* outPos, cVector* p, cVector* x);

// ASYNCNET {
	// IwNetObject::
	virtual int			netGetHost() const { return ID() & 0xFF; }
	virtual void		netDiscard();

	// utility functions
	bool				netIsMaster() const { return m_master; }
	void				netSetMaster(bool isMaster) { m_master = isMaster; }

	int					netGetPeer() const { return netIsMaster() ? 0 : netGetHost(); }
// } ASYNCNET

	// Флаги
	virtual bool		Is_Target();			// true - цель

	// ручное управление
	virtual void SetCommand(const MovingObjectCommand * command);
	virtual void SetCommand(const SwitchEPLRS * pSwitchEPLRS);
	virtual void CommandSet(int command);
	virtual void CommandSet(int command, float value);

#ifdef DEBUG_TEXT
	virtual const char * debug_text()  { return ""; }
	const char * getFullName() const;
#endif

	void setName(const ed::string & name);
	const ed::string & getName() const;

	bool ForceID(viFlag, ObjectID, const ed::string& misID=ed::string());

	//Returns object id in the mission file
	ObjectID            getMissionId() const { return m_missionID; }
    void                setMissionId(ObjectID objId) { m_missionID = objId; }
    void                setMissionId(const ed::string & missId);

	// old sound
	Sound::Host&		sound_host() { return _sound_host; }
	void				sound_init(const char *class_name);
	void				sound_update();
	void				sound_exit();
		
	// new sound
    ed::SounderRef&	    getSounderRef() { return _sounder; }
	bool				soundInit(const char* sounderName, const char* sounderResource);
	bool				soundInitWithAutoName(const char* sounderResource);
	bool                soundUpdate();

	//взорван ли объект
	virtual bool IsExplode(){ return m_bExplode; };

	//взрыв обработан
	virtual void ExplProcess(){ m_bExplProcess = true; };

    virtual const char * customlabel()  { return ""; }

protected:

	//состояние объекта(взорван)
	bool m_bExplode;
	bool m_bExplProcess;
	
	void				createCrashEvent();
	void				createKillEvent();

	virtual void		calculateYawPitchRoll();
	virtual void		setOrientation();
	virtual void		formOrientation(cPosition& outPos, float yaw_, float pitch_, float roll_);

	virtual void        doSoundUpdate();

	ed::string			name;

	wsFlags				flags;		// Особенности объекта (см. wsFlags.h) - bitset<32>
	cPosition			pos;		// Матрица положения в мировых координатах - 12 * float
	wsType				obj_type;	// Тип объекта (см. wsType.h) - unsigned int
	wcCoalitionName		coalition;	// Свой/чужой ?
	unsigned char		state;      // cтрана             
// camera - хорошо бы убрать в визуализатор
	wCamera				camera;		// Параметры камеры

	Position3			pos3;		// Матрица положения в мировых координатах - 12 * float

    dVector             pos_p;      // точная позиция
    dVector             detection_point; 


	edterrain::PointOnSurfaceAdapter	surfInfo;   // Свойства поверхности в pos

	float               life;			// Уровень живучести
	float				life0;			// Начальный Уровень живучести

	mutable float		v;	// Скалярная скорость
	mutable cVector		V;	// Вектор скорости - 3 * double?	
	wModelTime			PosUpdateTime;  // последнее модельное время обновления позиции
	wModelTime			netRefreshTime;	// Модельное время принятых данных из сети
	bool				m_master;		// false - сетевой

	float				pitch;
	float				yaw;
	float				roll;

	//old sound
	// SOUND {
	//void				sound_init(const char *class_name);
	//void				sound_exit();
	//void				sound_update();
	// } SOUND

#ifdef DEBUG_TEXT
	mutable	ed::string fullName;
#endif

private:
	ObjectID            m_missionID;

	// old sound
	// SOUND {
	Sound::Host			_sound_host;
	void				sound_get_params(Sound::HostParams*);
	// } SOUND

	// new sound
	ed::Sounder _sounder;
};

//Returns object group
WORLDGENERAL_API wControl * getGroup(const MovingObject * movingObject);

//Returns object top group
WORLDGENERAL_API wControl * getTopGroup(const MovingObject * movingObject);

typedef cPointerTemplate<MovingObject>	woPointer;

class WORLDGENERAL_API DatabaseHelper
{
    static ed::vector<ed::string> flag_filename;
    static ed::vector<ed::string> country_name;
    static ed::vector<ed::string> country_name_short;
    static ed::map<ed::string,double> rates;

public:
    static void load(lua_State * database);
    static const char * get_flag(unsigned char country_);
    static const char * get_flag(const woPointer & unit_);
    static const char * get_country_name(const woPointer & unit_,bool get_short_ = false)
	{
		return get_country_name(unit_->Coalition(),unit_->State(),get_short_);
	}
	static const char * get_country_name(unsigned char coalition,unsigned char country,bool get_short_ = false);
    static double get_unit_rate(const ed::string& name_);
};

#endif

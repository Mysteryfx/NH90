//                    file woMissile.h
#pragma once
#include "Weapons.h"
#include "wWeapon.h"
#include "wDetectable.h"
#include "GraphicEffect.h"
#include "ObjectLight.h"

#include "wRocketData.h"
#include "DebugGeometry/GlobalPrimitiveCache.h"

#include "Math/IIR_Filter.h"

class woMissile;
class cLauncher;

class woMissileActivity : public wTime
{
public:
	woMissileActivity::woMissileActivity(woMissile* p, wModelTime dt);
	virtual ~woMissileActivity() {}

	void NextEvent();
	woMissile* boss;
};

class woMissileDynamic : public wTime
{
public:
	woMissileDynamic::woMissileDynamic(woMissile* p, wModelTime dt);
	virtual ~woMissileDynamic() 
	{
		#ifdef DEBUG_GEOMETRY
			GlobalPrimitiveCache::instance()->removeCache(_cacheIndex);
		#endif
	}
	void NextEvent();
	woMissile* boss;

private:

	#ifdef DEBUG_GEOMETRY
		GlobalPrimitiveCache::Cache::size_type _cacheIndex;
	#endif
};

class woMissileNextTime : public wTime
{
public:
	woMissileNextTime::woMissileNextTime(woMissile* p, wModelTime dt);
	virtual ~woMissileNextTime(){;}

	void NextEvent();
	woMissile* boss;
};

#define IID_IwoMissileTail Common::identify<woMissileTail>() 

typedef enum wsMissileDynamics // Виды динамики
{
	wsMD_Null = 0,
	wsMD_AA,			// воздух-воздух
	wsMD_SA,			// зенитная
	wsMD_Aeroballistic, // аэробаллистическая
	wsMD_Antiship,      // противокорабельная
	wsMD_Wing,          // крылатая
	wsMD_AS_Regular,    // прочие 
	wsMD_Maximum        // крылатая
};

wsMissileDynamics WEAPONS_API getDynType(const wsType & type);

class SmokeTail;

#define IID_IMissile Common::identify<woMissile>() 


struct wRocketHotJoin;
class WEAPONS_API woMissile : public wWeapon, wDetectable // Родительский класс wRocket & wnMissile.
{         
public:
	
    woMissile();
    ~woMissile();

    virtual void    setMissile(	unsigned int Gun_, unsigned int A_T, const cVector& n2,
								wsType M_Type,  const cVector& V_, const cPosition& pos_,
								int LT, const Graphics::DParamList & lst = Graphics::DParamList(), bool alwaysGuideOnTargetIn = false);
    int				Class(void) { return woClass_Rocket;}
	void			NextTime();
	void			Control(); 
	virtual void	serialize(Serializer &serializer); 
	virtual void	PostCollisionProcessing(wsCollisionData*, ObjectID);

	unsigned int	GunId()	const { return Gun; }	// кто выпустил (eсли obj - снаряд, ракета...)
	woPointer		getHost()	{	return pGun;}

	const Graphics::DParamList& GetDrawArguments() { return drawArgs; }

	const float		Omx(void) const      { return Ordme.x;}
	const float		Omy(void) const      { return Ordme.y;}
	const float		Omz(void) const      { return Ordme.z;}
	const float		Roll(void)      { return Gamma;}
	const float		Tang(void)      { return Tangaj;}
	const float		Course(void)    { return Psi;}
	void			SetNewGun(unsigned int Gun_);
	const bool		In_Air_(void)  const { return true;}			// В воздухе ли объект
	const float		Nx(void) const	   { return N.x;}
	const float		Ny(void) const	   { return N.y;}
	const float		Nz(void) const	   { return N.z;}
	const float		Alfa(void) const	   { return alfa ;}

	const float		Hrad()		   { return (pos.p.y - H_Rel);}

	cVector&		GetTargetPosition(void) { return T_pos; }	// радиус-вектор цели			
	cVector&		GetN2Position(void) { return n2; }			// координаты цели			
	bool			GetNoTarget(void)	{ return No_Target; }
	void			SetNoTarget(bool _no)	{ No_Target = _no; }

	virtual const unsigned int  Target_ID(void) const { return A_Target;}
    bool                         IsTarget(unsigned id) const
    {
        return id == Target_ID();
    }
	unsigned int	GetTargetID(void) { return A_Target; }
	unsigned char	Get_Family(void)  {return Family;}
	unsigned char	Get_Name(void)    {return Name;}
	int				Get_Head_Type(void)	{return Head_Type;} 
	void			Set_Head_Type(int a);
	int				Get_H_max(void)	{return H_max;} 
	virtual cPosition&			Position(wModelTime t);
//	virtual void	setLauncher(cLauncher* launcher_) {launcher = launcher_;}
    unsigned int	getGun()		{	return Gun;	  }
    wModelTime		getControlTimeStep()	{	return dtime1; }
    wModelTime		getDynamicTimeStep()	{	return dtime2; }
    void			Select_Dynamics();  // Выбрать динамику полета
	virtual void	MissileSetTarget(const cVector& tpos);
	void			setTime(float a)	{	Time = a;}
	void			set_ic70(bool a)	{	ic70 = a;}
	float			getM_miss()			{	return M_miss;}

	virtual wDetectable* getDetectable();
	//wDetectable interface begin
	virtual bool  getVolumeEffectBox(Math::Box2f * box = NULL) const;
	virtual float getIllumination(const cVector & vec) const;
	float getTemperature(float cos_target_aspect_angle) const
	{
		return 100.0f * (2.0 + cos_target_aspect_angle) * 0.33f;
	}

	float getTemperature(const cVector & vec) const
	{
		return getTemperature(vec.ort() * pos.x);
	}

	virtual float getRCS(float lambda, const cVector & vec) const;
	//wDetectable interface end

    virtual float   getNy_max() const { return Ny_max; };

#ifdef DEBUG_GEOMETRY
	virtual const char * debug_text();
	cVector start_point ;
	float distance ;
#endif

protected:

    void			Init(void);            // инициализация
	void			InitExhaust(void);     // инициализация
	void			initialization();	   // инициализация
	void			DrawArgument_(wModelTime dt_);  //Определение аргументов рисования
    void			Rocket_Limits(void);    // ограничения и настройки 
    void			Get_Target_Performances(void); 	
    void			Follow_Direction(const cVector &nv); 
    void			Control_Vel_Req(void);  // Вычисление скорости.

    void			Attack_Air_Target(void); 	
    void			SAttack_Air_Target(void); 	
    void			Aeroballistic_Missile(void); 
    void			AS_Regular_Missile(void);
    void			Antiship_Missile(void); 	
    void			Wing_Missile(void); 	

	bool			isAirTargetVisible(woPointer P_Target); //видна ли цель
	float			DefineObjectReflection(woPointer); //определить ЭПР объекта
	float			TimeToTargetImpact(); //вычисление времени упреждения для наведения

    void			Ground_Proximity(float dh_min, cPoint * point = NULL); // Предупреждение о близости земли
    void			Control_Gamma_Req(void);// Отработка заданного крена 
	void			Get_Gamma_Req(const cVector &nv); // Вычислить заданный крен.
	float			Get_Cx(float); // Вычисление коэффициента сопротивления Cx
	void			Set_No_Target(void);
	virtual void	Get_T_pos_Error(void) {;} // вычислить ошибку положения цели
	virtual void	Control_Spec(void); // Управление особенностями  
	void			CreateLight(void);	// Создать источник света  
	void			Dyn_Type(); // установить вид динамики
	void			Target_Warning(void);   // Предупредить цель об опасности	

	float			GetNyzAvailable(); // вычисление располагаемой перегрузки
	float			GetAlfa() ; // вычисление угла атаки 
	void			GetCxCy(float alfa, float* Cx , float* Cy) ; // вычисление текущего коэф. аэрод. силы
	void			GetCx0CyaA(float* Cxa , float* Cya, float* A);

	virtual void			doExplode(const cPoint& , ObjectID id);

	               // Переменные. Variables.Save
	Rocket_Const *	desc_;
	cVector			Ordme;	
	int				Laun_Type;		// тип пилона: 0-рельсовый, 1-катапультный,
    wModelTime		Time_01;		// счетчик времени на 0.1c
    wModelTime		Time_1;			// счетчик времени на 1c
	cVector			T_pos;			// радиус-вектор цели			
    unsigned int	A_Target;		// мировой ID захваченной цели
    unsigned int	Primary_Target;	// мировой ID подсвечиваемой (радаром самолета) цели
    wModelTime		Time;			// Время от создания.
	wModelTime		dtime1;			// шаг времени Control.
	wModelTime		dtime2;			// шаг времени Select_Dynamics().
    bool			No_Target;		// признак отсутствия (потери) цели
	bool			alwaysGuideOnTarget; //всегда наводится на цель вне зависимости ни от чего - для отладки
	cVector			nv;				// единичный вектор заданного направления			
	float			H_Rel;			// высота рельефа под ракетой
	float			vx;				// величина скорости вдоль оси x 
	float			vy;				// величина скорости вдоль оси y
	float			vz;				// величина скорости вдоль оси z ракеты
	float			Dist0;			// начальная горизонтальная дистанция
    float			dV;				// Линейное ускорение.
	float			Om_max;			// Максимальная угловая скорость
	float			Om_max_Req;		// Требуемая максимальная угловая скорость
    cVector			n2;				// Координаты цели
    cVector			n3;				// Вектор направления на цель
    float			v_L;			// Cкорость цели 
    float			Dist;			// Расстояние до цели
	bool			ic1;			// true - при первом обращении
	bool			ic70;			// false - после первого прохождении высоты 70м
    int				N_v_old;		// Номер режима на прошлом шаге.
	Graphics::DParamList	drawArgs;	// Xранилище для аргументов рисования
    bool			Ground_Pro;		// true, если близка земля
	cVector			N;				// вектор перегрузки
	float			alfa ;			// угол атаки
	float			alfa_y ;		// угол атаки в плоскости силы тяжести и оси Х
	float			alfa_z ;		// угол атаки в плоскости горизонта (скольжение)
	float			alfa_yr   ;			// что и выше но отфильтрованное
	float			alfa_zr   ;			// что и выше но отфильтрованное
	Math::IIR_Filter		alfa_y_filter ;
	Math::IIR_Filter		alfa_z_filter ;
	bool alfa_filter_initialized  ;

	float			H_0;			// высота пуска
	float			H_min_t;		// минмальная высота полета цели
	bool		    mail_flag;		// true - если сообщение цели еще не было послано
	wModelTime		last_target_warning_time;
    bool			TWasActive;		// true, если был включен радар цели
    // Restore
    unsigned char	Family;			// Вид ракеты: AA, AS
	unsigned char	Name;			//  имя ракеты	
	double			Gamma_max;		// максимальный крен
		// Характеристики ракеты. Rocket_Limits() 
	int				Escort;			//  сопровождение:0 - нет,1 - ЛА пуска... 
	int				Head_Type;		// тип гoловы наведения 
    float			H_max;			//  максимальная высота полета. 
    float			H_min;			//  минимальная высота полета.
    float			Diam;			//  диаметр корпуса в мм 
	float			Life_Time;		//  время жизни
	float			Ny_max;			// Максимальная разворачивающая перегрузка 
    float			v_min;			// Минимальная скорость. 
    float			Mach_max;		// Максимальное число Маха.
	float			t_b;			// время включения двигателя
    float			t_acc;			// время работы ускорителя
    float			t_marsh;		// время работы в маршевом режиме
	float			Fi_start;		// угол визирования при пуске и сопровождения 
	float			M_miss;			// полная масса в кг
	float			Expl_Massa;		// эквивалентная масса взрывчатки кг
	float			X_back;			// координаты центра сопла в осях ракеты
	float			Y_back;			// 
	float			Z_back;			// 
    bool            hasDetachableAccelerator; // наличие отделяемого ускорителя
	float			Nr_max;			// Максимальная перегрузка при разворотах
	float			cos_Viz_lim;	
	float			cos_Rak_lim;
	float			cos_Search_lim;	// предельный угод свободного поиска цели
	float			OmViz_max;		// предельная скорость линии визирования
    cVector			scale;
	float			t_start;		// длительность старта (рельс./катап. тип)
    float			t_control;		// время включения управления
	float			t_yz;			// время списывания vy & vz 
	bool			Head_Form;		// false - полусферическая форма головы, 
	float			S_mid;			// площадь миделя, м*м
	float			Psi;			// угол курса
	float			Gamma;			// угол крена
	float			Tangaj;			// угол тангажа
    float			R_turn;			// Радиус разворота
	float			H_building;		// Высота домика на пути максимальная
    bool			Snar_flag; 
	wsMissileDynamics	DynType;	// вид динамики
	bool			EngineOnNotification;	// true - доклад о вкл.двигателя
	bool			EngineOffNotification;	// true - доклад о выкл.двигателя
	                    // Хвосты
	bool			Tail_Boost;		// Есть ли короткий хвост
	bool			Tail_Boost_Yes;	// true - создан короткий хвост
	bool			Tail_March;		// Есть ли длинный хвост
	bool			Tail_March_Yes;	// true - создан длинный хвост
    
	float			Gamma_Req;		// Требуемый крен - local
    cVector			L_V;			// Вектор скорости цели - local
	float			t_radSpeed;		// радиальная скорость цели (запоминает для отброса ложных целей)
	cVector			TposE;			// ошибка координат цели
	
	cVector         Nav;			// вектор наведения (используется "Вихрем")
	cVector         integr;         // интегратор, используется для устранения статической ошибки наведения
	float			angleN;			// угол отклонения линии визирования от оси ракеты про свободном поиске (исп.для моделирования спирали поиска)
	float			dAngleSp;			// фаза спирали свободного поиска 
	cPosition		pos_time;
	bool			NeedDraw;		// false - рисовать

//	cLauncher*	    launcher;
	woPointer		pGun;			// указатель на пусковую установку
	woPointer		T_ptr;			// Указатель на цель
	gePointer		wObj;
	gePointer		wObj1;
	wtPointer		pMissileActivity;
	wtPointer		pMissileNextTime;
	wtPointer		pMissileDynamic;
    Effects::ObjectLight*	pLight;

	int				exhausts_count;
	Rocket_Const::Exhaust exhausts[2];

    double          KillDistance;

	double			ccm_k0;				//базовый к-т вероятности ухода на другую цель (ловушку)

	float			ModelData[mdpParamCount] ;
	int				mode_interval ;		
	float			trust_Ho ; // тяга РДТТ на высоте Но = 0 м
	float           trust ; // мгновенна тяга РДТТ с учетом высотной характеристики
	float			Nr_req ; // модуль требуемой поперечной пеергрузки
	float			Nr_avail ; // модуль располагаемой поперечной перегрузки
	cVector			X_ort ; // орт строительной оси ракеты

	typedef struct { float time ;  float  fuel_flow_rate; float trust ;} RocketEngineCyclogramm ;
	RocketEngineCyclogramm cyclogramm[7] ;

	bool	fuze_activated ;

	cPoint			ground_proximity_pnt;

// MAIL {
public:
	virtual bool netGetCreate(Mail::Message&) const;
	bool woMissile::netSendUpdate(int dest, int prio, int& cookie);
	void get_wRocketHotJoin(wRocketHotJoin& s) const;
// } MAIL

private:
	void sound_init();
};

extern WEAPONS_API float IR_Missile_Lock_Dist(const woPointer & P_Target,const Rocket_Const * missile_desc, float cosRak);

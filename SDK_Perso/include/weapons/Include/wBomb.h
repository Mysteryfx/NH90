//                    file wBomb.h
#pragma once

#include "Weapons.h"

#include "wWeapon.h"
#include "GraphicEffect.h"
#include "Randomizer\RandomObject.h"
#include "ObjectLight.h"

typedef ed::vector<Vector3> coords_vec;

const unsigned SmallBombNum = 40;

// горизонтальные ускорения торможения управляемых бомб
const float  GuidedBombBrakingMax = 5.0f; // m/ss
const float  GuidedBombBrakingMin = 1.0f; // m/ss

class wBomb;
class wBombActivity : public wTime
{
public:
	wBombActivity::wBombActivity(wBomb* p, wModelTime dt);
	virtual ~wBombActivity();

	void NextEvent();
	wBomb* boss;
};

#define IID_IwBombSmall Common::identify<wBombSmall>() 

class WEAPONS_API wBombSmall : public viObjectNode
{
private:

	cPosition pos0, pos;
	cVector V0, V;
	wModelTime LTime0;
	int i;
	wBomb* parent_bomb;
	
public:
	static RandomObject randomOrderly;

	wBombSmall();
	~wBombSmall();
	void	setBombSmall(wBomb *owner, int i_);
	virtual void	serialize(Serializer &serializer);

	// поддержка интерфейса viObject
	virtual cPosition&			Position(wModelTime t);

	unsigned char   cluster_type;
	bool			ClasterY;
};

struct Bomb_Const;
class viSearch;
class SmokeTail;

#define IID_IBomb Common::identify<wBomb>() 

struct wBombHotJoin;
class WEAPONS_API wBomb : public wWeapon     // Бомбы.
{      
public:
	static RandomObject randomOrderly;

	wBomb(); 
    ~wBomb();

	static ObjectID create(	ObjectID carrier_, ObjectID target_,	const cVector& n2_,
							wsType type_, const cPosition& pos_, cVector& VV, ObjectID master_,const Graphics::DParamList & lst = Graphics::DParamList(), bool armed = true);

	// контейнер с суббоеприпасами
	static ObjectID create(	ObjectID carrier_, ObjectID target_,	const cVector& n2_,
							wsType type_, const cPosition& pos_, cVector& VV, ObjectID master_, const unsigned char, const coords_vec &, const int, bool armed = true);

	void            setBomb(ObjectID carrier_, ObjectID target_,
							const cVector& n2_, wsType type_, const cPosition& pos_, const cVector& VV, const Graphics::DParamList & lst);

	void			substituteShape(int wsTypeLevel4);

    int				Class(void) { return woClass_Bomb;}
	void			Control(); 
	void			NextTime(); 

	unsigned int	GunId() const { return Gun; } // кто выпустил
	woPointer		getHost()	{	return pGun;}

	const Graphics::DParamList& GetDrawArguments() { return drawArgs; }

//	const float		Omx(void)       { return Ordme.x;}
//	const float		Omy(void)       { return Ordme.y;}
//	const float		Omz(void)       { return Ordme.z;}
	const bool		In_Air_(void)   { return true;} // В воздухе ли объект
	virtual const unsigned int  Target_ID(void) const { return A_Target;}
    bool            IsTarget(unsigned id) const
    {
        return id == Target_ID();
    }
	cVector&		GetN2Position(void) { return T_pos; } // координаты точки-цели полета			
	void			Update_nv(void); // Пересчитать nv.
	cPosition&		Position(wModelTime t);
	virtual bool	UseChildren(wModelTime t, viArgumentPurpose purpose);

	virtual void	serialize(Serializer &serializer);
	virtual void	MissileSetTarget(const cVector& tpos);
	void			setTime(float a)	{	Time = a;}

	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);

	cPosition&		get_cluster_rot_pos(){return cluster_rot_pos;};
	cPosition&		get_cluster_rot_pos0(){return cluster_rot_pos0;};

	unsigned char   cluster_type;
	bool			ClasterY;		// true - нужно высыпать начинку кассетной б.
	coords_vec		coords;
	int				burst_num;

	virtual ed::vector<cVector> get_impact_points();
private:

    void		Init(void);			// инициализация
	void		initialization();   // инициализация
	void		Bomb_Limits(Bomb_Const* Bomb_Ch);
	void		Control_wBomb(void); // специфика несетевой бомбы.
	void		Set_Ag(void);		// Установить Ag.
    void		Control_Vel(void);	// Управление скоростью
	void		DrawArgument_(void);// Определение аргументов рисования			
	wBombSmall* AddBombSmall(wBomb *owner, int i_); //+ BombSmall
	void		CreateLight();		// Создать источник света  
			// Переменные. Variables.
    unsigned int	A_Target;		// ИД цели
	wModelTime		Time;			// время.
	wModelTime		Time1;			// счетчик на 1 секунду.
	float			vg;				// величина горизонтальной скорости  
    float			vy_hold;		// Установившаяся вертикальная скорость
	float			Ag;				// Горизонтальное ускорение 
	float			H_Rel;			// высота рельефа в точке падения
	cVector			T_pos;			// радиус-вектор цели. Save
	cVector			nv;				// единичный вектор заданного направления			
	unsigned char	B_Name;			// имя бомбы (level 4)
	int				N_LUU_2;		// количество выпущенных бомб 
	bool			SuppressExplosion; //подавление взрыва
		// Рисование	
	Graphics::DParamList	drawArgs;	// Xранилище для аргументов рисования
	bool			Bomb_Tail;		// Хвост пламени.
	bool			Bomb_Plumage;	// Оперение и парашют.
	// Restore
	int				Bomb_Type;		// тип бомбы
	float			X_back;			// координаты центра сопла в осях бомбы
	float			Y_back;			// 
	float			Z_back;			//
	float			Diam;			// диаметр бомбы в мм
	cVector			scale;
	bool			Retire;

	cPosition		pos_time;
	
	gePointer		wObj;	
//	wBombTail*		pBombTail;
	wBombSmall*		pBombSmall[SmallBombNum];

	wtPointer		pBombActivity;
	woPointer		pGun;			// Указатель на пусковую установку.
	Effects::ObjectLight*	pLight;	// источник света

	float			dPi, drandTime;

	// поворот субэлементов относительно единичной позиции
	cPosition		cluster_rot_pos0;
	// поворот субэлементов относительно реальной позиции
	cPosition		cluster_rot_pos;

// MAIL {
public:
	virtual bool		netGetCreate(Mail::Message&) const;

	void get_wBombHotJoin(wBombHotJoin& s) const;
	static ObjectID netCreate(const wBombHotJoin& s);

	virtual void			doExplode(const cPoint& , ObjectID id_=0);

// } MAIL
};

extern WEAPONS_API float BombFlyTime(float AirY, const cVector& Vel, float TagY, float Vy_Hold);
extern WEAPONS_API float BombMaxHt(float AirY, const cVector& Vel, float TagY, float Vy_Hold);
extern WEAPONS_API float BombLaunchDist(float ty, const cVector& Vel, float Ag);


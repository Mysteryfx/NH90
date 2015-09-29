//                    file wRocket.h
#pragma once

#include "Weapons.h"
#include "woMissile.h"
#include "Randomizer\RandomObject.h"

class lLandObject;

#define IID_IRocket Common::identify<wRocket>() 

class WEAPONS_API wRocket : public woMissile       // Ракеты.
{         
public:
	static RandomObject randomEvenly;
	static ObjectID Create(	ObjectID Gun_, ObjectID A_T, const cVector& n2, 
							wsType M_Type, const ed::string& db_id_,const cVector& V_, const cPosition& pos_, 
							int LT, ObjectID id_,const Graphics::DParamList & lst = Graphics::DParamList(), bool alwaysGuideOnTargetIn = false);

    wRocket();
    void       setMissile(	unsigned int Gun_, unsigned int A_T, const cVector& n2,
							wsType M_Type, const cVector& V_, const cPosition& pos_,
							int LT,const Graphics::DParamList & lst = Graphics::DParamList(), bool alwaysGuideOnTargetIn = false);

	virtual void	serialize(Serializer &serializer); 
	virtual void	MissileSetTarget(const cVector& tpos);

private:

    void        Change_AirTarget(void); // Сменить воздушную цель.
    void        SetAirTarget(ObjectID T_ID); // (Зеленский) Назначить воздушную цель.
	void		MissileCheckImpactByDistance(); // Проверка столкнoвений по расстоянию. 
	void		Get_T_pos_Error(void);// вычислить ошибку положения цели
	void		Init_TposE(void);	  // Инициализация ошибок положения цели.
	void		Control_Spec(void);		// Управление особенностями

    double      getKillDistance_();     // радиус поражения

			// Переменные. Variables.
	bool			mail_farewell_flag;
	float			TposE_x;		// Ошибка местоположения цели, ось x
	float		    TposE_z;		// Ошибка местоположения цели, ось z
	wModelTime		Time_oldCAT;	// запомненное время
	wModelTime		Time_old;		// запомненное время
	bool			Yes_Dis;  

    friend bool MissileCheckImpactByDistanceHandler(viObject *item, 
                                                    viSearch *search, 
                                                    void* data);


// MAIL {
public:
	static ObjectID netCreate(const wRocketHotJoin& s);
// } MAIL
	friend class wWeaponMail;
};



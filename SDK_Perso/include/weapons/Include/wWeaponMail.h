#ifndef W_WEAPON_MAIL_H
#define W_WEAPON_MAIL_H


#include "WorldMail.h"
#include "wWeapon.h"


struct wBombHotJoin
{
	ObjectID		carrier_;	// ID носителя	
	ObjectID		target_;	// ID цели
	cVector			n2_;		// вектор цели
	unsigned char	lev4;		// имя оружия
	cVector			point_;		// координаты
	float			Psi_;		// курс, град.
	float			Tangaj_;	// тангаж, град.
	cVector			V_;			// скорость 
	ObjectID		id_;		// ID объекта
	float			Time_;		// локальное время объекта
	bool			Supress_Explosion; //подавление взрыва
};

struct wRocketHotJoin : public wBombHotJoin
{
	int		LT_;	// тип пилона
	bool	ic70_;
};


class wBomb;
class woMissile;

class wWeaponMail {
public:
		wWeaponMail();
		~wWeaponMail();

	static void explode(wWeapon* nuke, const cPoint& where, ObjectID whom=0);
	static void destroy(wWeapon* nuke);
	static void retarget(wWeapon* nuke, ObjectID newtarget);
	static void setheadtype(woMissile* mis, int type);
	static void updatetarget(int host, const wWeapon* mis, const cVector& pos);

	static bool baseline(const wBomb* bomb, Mail::Message& msg);
	static bool baseline(const woMissile* mis, Mail::Message& msg);

}; // class wWeaponMail


namespace Mail {
	Stream& operator >> (Stream& stream, wBombHotJoin& data);
	Stream& operator >> (Stream& stream, wRocketHotJoin& data);
}

#endif /* W_WEAPON_MAIL_H */

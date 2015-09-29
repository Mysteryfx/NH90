#pragma once

#include "WeaponsBase.h"

#include "Shells/shell_coeffs.h"
#include "PropertyTable.h"

#include <ed/vector.h>

class wAmmoSupply;
class wIGunCarriage;

namespace Mail { class Stream; }

namespace weapons_gun {

class wGunDescriptor;
class wGun;
class wGunShellActivity;
class wGunDescriptor;

class SignalBase
{
public:
	virtual void Invoke() = 0;
};

typedef ed::vector<SignalBase*> Vector;

template<typename T>
class Signal : public SignalBase
{
	typedef void (T::*MemberFuncPtr) ();

	T* object_;
	MemberFuncPtr member_ptr_;

public:

	Signal(T* object, MemberFuncPtr member_ptr )
		: object_(object), member_ptr_(member_ptr)
	{ }

	virtual void Invoke()
	{
		(object_->*member_ptr_)();
	}
};

class WEAPONSBASE_API wGun
{
	friend class wGunShellActivity;
	friend class GunTrigger;
	friend class DelayedGatlingTrigger;
	friend class GunSynchronizer;
	friend class wAircraftGunMount;

public:

	enum Gun_States
	{
		GUN_IDLE = 0x00,
		GUN_ACTIVE = 0x01,
		GUN_FIRE = 0x02,
		GUN_HOLD_SHOT = 0x4
	};

// #ifdef _ED_HIDDEN_DEBUG 
// #define LOG_GUN_TEMP
// 	int gun_n_;
// #endif

	wGun(const wGunDescriptor& desc, wIGunCarriage* carriage, wAmmoSupply* supply,  bool isNetMaster);
	virtual ~wGun();

	void selectFireRateIndex(int fireRateIndex);
    int  getFireRateIndex() const;

	//задать темп явно (== -1 - будет использоваться FireRateIndex)
	void setFireRate(double fireRate);
	double getFireRate() const;

	// burstLength == -1 - пока не придет endBurst или не кончатся патроны или не сработает отсечка
	// net_data - сюда записываются (или считываются) сетевые данные
	void beginBurst(Mail::Stream& stream);
	void endBurst();
	void sync_T(Mail::Stream& stream); /*synchronizes gun temperature*/

	void on_shot();
	// Установить длину очереди (возможно обрезав по ограничению пушки, если не указано обратного)
	// length = -1 - бесконечно длинная очередь
	void setBurstLength(int length, bool disable_gun_cutoff = false);
	int getBurstLength() const { return burstLength_; }

	bool isFiring() const;		// Стреляем или перезаряжаемся после последнего выстрела?
	bool isActive() const;

	bool isReadyToFire() const;	// Перезарядились

	double get_shell_wakeup_time() const;

	void set_on_shot_hndl(SignalBase* signal_ptr);
	void set_fire_stop_hndl(SignalBase* signal_ptr);

	void fill_shell_coeffs();

	unsigned char get_flags() const { return state_f_; }

	void set_flag(unsigned char flag, bool set) {
		 if(set) state_f_ |= flag; else  state_f_ &= (~flag);
	}

	PropertyTable& get_property_table() { return properties_; }

	const wGunDescriptor & get_descriptor() const { return desc_; };

protected:
	void open_fire();
	void stop_fire();

	void on_fire_start();
	void on_fire_stop();

	wIGunCarriage* carriage_;
	wAmmoSupply*  supply_;
	const wGunDescriptor& desc_;

	int burstLeft_;			// сколько осталось выстрелить (0 - не стреляем, но перезаряжаемся, -1 - готов)
	int burstLength_;		// длина очереди, установленная пользователем (-1, если не установлена)
	double fireDelay_;		// задержка между выстрелами, установленная пользователем (0.0, если не установлена, тогда используется fireDelay_)
	int fireDelayIndex_;

	wGunShellActivity* shell_act_;

	bool isNetMaster_;

	ed::vector<SignalBase*> on_fire_hndl_;
	ed::vector<SignalBase*> on_fire_stop_hndl_;

	wShellCoefficients shell_c_;

	unsigned char state_f_; // state flags

	PropertyTable properties_; //custom properties map. F.e. model argument value

	double*& T_ptr_; //temperature ptr
	double*& ext_heat_model_;
};
}


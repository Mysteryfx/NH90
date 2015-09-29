#ifndef __AMMUNITION_H__
#define __AMMUNITION_H__

#include "moving_object_proxy.h"
#include "Math/Vector.h"
#include "simulation_wires.h"
#include "WeaponsBase.h"

#include "Ammunition/ammunition_name.h"

#include "IWorldNet.h"
#include "WorldMail.h"
#include "SoundTools.h"

const double g_cWeaponsT = 0.02;

// Forward declarations
class wSimulationSystem;
class wAmmunitionDescriptor;

namespace Mail { class Message; class Stream; }
struct wNetCreateInfo;

// Класс боеприпаса или держателя
// Может работать как мастер и как клиент
class WEAPONSBASE_API wAmmunition : public wMovingObjectProxy, public wINamedAmmunition
{
private:
	wAmmunition(const wAmmunition&);
	wAmmunition& operator = (const wAmmunition&);

public:
	// Поскольку это moving object, вынужден сделать конструктор по умолчанию, а всю
	// инициализацию запихнуть в init
	wAmmunition();
	virtual ~wAmmunition();

    void initServer(MovingObject * launcher,const wAmmunitionDescriptor* desc, Lua::Loader* runtime_config);
	void initClient(MovingObject * launcher,const wAmmunitionDescriptor* desc, Lua::Loader* runtime_config, const wNetCreateInfo& info, Mail::Stream& stream);

	virtual int Class();

	virtual void destroy(bool immediately = false);

	// Симуляция
	virtual double simulate();

	wSimulationSystem* getSystem() { return system_; }

	// Установка координат (на подвесе, например)
	// Это ТЯЖЕЛЫЕ операции (каждый раз приходится находить порт)!!!
	void setPosition(const Math::Vec3d& p);
	void setRotation(const Math::Rot3d& r);
	void setVelocity(const Math::Vec3d& v);
	void setOmega(const Math::Vec3d& omega);

	// IwNetObject
	// Только создание, больше ничего не надо
	virtual bool netGetCreate(Mail::Message&) const;

	// From wINamedAmmunition
	virtual const wINamedAmmunitionDescriptor* getNameDescriptor() const;
    virtual ObjectID getLauncherId() const;
    
    virtual void setLauncher(MovingObject* obj);
    virtual void setLauncherId(ObjectID id);
    virtual void setCoalition(wcCoalitionName coalition);
	virtual void setState(int state);

    virtual viFlag getViFlag();

protected:
	// Инициализировать что есть общего у клиента и сервера
	virtual void initCommon_(MovingObject * launcher,const wAmmunitionDescriptor* desc);
	
	friend class wWeaponMail;

	void netDestroy();

	void initDrawArgs_(const ed::vector<int>& drawArgs);

	// Вызывается из порта
	void die_(bool);

	//void sendCreateMessage_();

protected:
	double birthTime_;

	// Весь экшн - там
	wSimulationSystem* system_;

	// Выходы (относительно схемы)
	const Math::Vec3d	*position_, *velocity_, *omega_;
	const Math::Rot3d	*orientation_;
	ed::vector<const double*> drawArgWires_;

	// Индексы draw аргументов
	ed::vector<int> drawArgInds_;

    ObjectID launcherId_;

	// Порты для сериализации
//	wInputPortObjPtrBase<Mail::Stream>* port_stream_;

	// Эти порты выходные для схемы (для это класса входы)
	wInputPortImpl<bool, wAmmunition>		port_died_;

	// дескриптор
	const wAmmunitionDescriptor* desc_;

private:
	void sound_init();
};

namespace {
	typedef Mail::GenMessage<wWeaponMsgID2 + 1, Mail::WeaponMsg, ObjectID> MsgWeaponDestroy;
}

class wWeaponMail
{
public:
	wWeaponMail()
	{
		globalMail->reg( Mail::GenWorldHandler<MsgWeaponDestroy, wAmmunition>()(&wAmmunition::netDestroy) );
	}

	~wWeaponMail()
	{
		globalMail->unreg(MsgWeaponDestroy::ID);
	}

	void destroy(wAmmunition* ammo)
	{
		globalWorldNet->netSend(0, MsgWeaponDestroy(ammo->ID()));
	}

};

///////////////////////////////////////////////
// будильник для боеприпаса
///////////////////////////////////////////////
#ifndef WEAPONS_BENCH
class wAmmunitionActivity : protected wTime
{
public:
	wAmmunitionActivity(wAmmunition* host, double dt0)
	{
		host_ = host;
		wTime::wakeUpTime = wTime::GetModelTime() + dt0;
		PutToSleep();
	}

protected:
	// Напрямую удалять wTime нельзя
	virtual ~wAmmunitionActivity() {};

protected:
	// from wTime
	virtual void NextEvent()
	{
		if (host_ == 0)
			KillMe();
		else
		{
			wakeUpTime = host_->simulate();
		}
	}

private:
	// Пусть таймер живет, пока родительский объект не сдох
	cPointerTemplate<wAmmunition> host_;
};
#endif


#endif

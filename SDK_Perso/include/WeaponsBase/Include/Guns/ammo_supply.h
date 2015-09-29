#ifndef __AMMO_SUPPLY_H__
#define __AMMO_SUPPLY_H__

#include "WeaponsBase.h"

#include "descriptor_base.h"

#include "Math/Vector.h"

class wShellDescriptor;

namespace Mail { class Stream; }
namespace Lua  { class Loader; }

// Место, где хранится информация о боеприпасе для пушки
class WEAPONSBASE_API wAmmoSupply
{
public:
	wAmmoSupply();
	virtual ~wAmmoSupply();

	// Установить бесконечные патроны
	virtual void setUnlimited(bool unlimited);

	// Установить количество патронов
	virtual void setAmmoCount(int ammoCount) = 0;

	// Подготовиться к очереди (считать/записать данные для сети)
	virtual void prepareBurst(Mail::Stream& stream) = 0;

	// Если патронов нет, результат undefined!
	virtual const wShellDescriptor* getCurrentShell() const = 0;

	virtual const wShellDescriptor* getShellForSight() const { return getCurrentShell(); } ;

	virtual const wShellDescriptor* getShellByName(const ed::string & shells_name) const = 0;
	virtual const wShellDescriptor* popNextShell() = 0;	// 0 если кончилось

	virtual bool isEmpty() = 0;

	// Получить массу боеприпасов
	virtual double getMass() = 0;
	// получить центр масс
	virtual Math::Vec3d getCenterOfMass() = 0;		

	// Получить количество боеприпасов 
	// Это все-таки костыль, поскольку семантика сильно завист от потомка
	virtual int getRoundsCount() = 0;
	// Получить начальное количество боеприпасов
	// См. выше
	virtual int getInitialRoundsCount() = 0;

	// Перезарядить
	virtual void reset(float gun_load_factor = 1.0f)
	{
		setAmmoCount(getInitialRoundsCount() * gun_load_factor);
	}
    // Разрядить
    virtual void discharge() = 0; 

    virtual void     setAmmoType(unsigned t) {}
    virtual unsigned getAmmoType() const  { return 0;}



protected:
	bool unlimited_;
};

class WEAPONSBASE_API wAmmoSupplyDescriptor : public wDescriptorBase
{
public:
	wAmmoSupplyDescriptor(const ed::string& name) : wDescriptorBase(name) {}

    // cfg - дополнительные параметры (например, количество снарядов)
    virtual wAmmoSupply* create(Lua::Loader* cfg) const = 0;
	virtual const wShellDescriptor* getShellDescriptorByName(const ed::string & shells_name) const = 0;
};

#endif 

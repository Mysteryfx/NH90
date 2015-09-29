#ifndef __ACTIVITY_GROUP_H__
#define __ACTIVITY_GROUP_H__

#include <queue>

#include "wTime.h"

#include "WeaponsBase.h"

// Базовый класс члена ActivityGroup
class WEAPONSBASE_API wActivityGroupMember
{
public:
	enum eSimulationResult
	{
		RES_KILL,			// Все, убить
		RES_NORMAL,			// Нормальный шаг
		RES_PARTIAL,		// Частичный шаг. Этим лучше не злоупотреблять - тяжелая операция
		RES_PRE_KILL		// Пуля уже взорвалась но нужен 1 тик чтоб это зафиксировать в других системах
	};

	wActivityGroupMember() {};
	virtual ~wActivityGroupMember() {};

	double getUptodateTime() const { return uptodateTime_; } 
	void setUptodateTime(double t) { uptodateTime_ = t; }

protected:
	void advance_(double dt) { uptodateTime_ += dt; }

private:
	double uptodateTime_;
};

class WEAPONSBASE_API wActivityGroup : public wTime
{
public:
	wActivityGroup(double dt);
	~wActivityGroup();

	void setAutoDelete();

protected:
	// из wTime
	virtual void NextEvent();

	void add_(wActivityGroupMember* obj);		// Чтобы потомки могли контролировать тип добавляемых объектов
	void simulate_(wActivityGroupMember* m);

	// Сдвинуть объект на один шаг вперед
	// Вынесено в отдельную функцию, чтобы можно было более полно реализовать паттерн flyweight
	virtual wActivityGroupMember::eSimulationResult simulateObject_(wActivityGroupMember* m, double dt) = 0;

protected:
    // "Нормальные" объекты (то есть прошедшие крайний раз полноценный шаг)
	std::queue<wActivityGroupMember*> objects_;
    // Объекты, прошедшие нестандартный шаг.
    ed::vector<wActivityGroupMember*> partialObjects_;
	bool auto_delete_;				// Умереть ли после того, как все объекты сдохнут
	double dt_;						// Шаг моделирования
};

template <class ContType, class MemType>
class wActivityGroupEnvelope : public wActivityGroup
{
public:
	wActivityGroupEnvelope(double dt) : wActivityGroup(dt) {}

	void add(MemType* t) { wActivityGroup::add_(t); }

	void setAutoDelete() { wActivityGroup::setAutoDelete(); }

protected:
	virtual wActivityGroupMember::eSimulationResult simulateObject_(wActivityGroupMember* m, double dt)
	{
		return static_cast<ContType*>(this)->makeObjectStep_(static_cast<MemType*>(m), dt);
	}
};

#endif

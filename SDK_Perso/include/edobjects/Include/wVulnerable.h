#ifndef wVulnerable_H
#define wVulnerable_H
#include "edObjects.h"

#include "Registry\Registered.h"

// причина повреждения
enum DamageCause
{
	dcUnknown,		// причина неизвестна
	dcObject,		// столкновение с другим объектом
	dcExplosion,	// воздействие взрыва
	dcGround,		// столкновение с землей
};

// полное описание повреждения
class EDOBJECTS_API DamageInfo
{
public:
	float amount;		// сила повреждения
	DamageCause cause;	// причина повреждения (если известна)
	ObjectID damager;	// кто повредил (если есть)
	int part;			// номер части объекта, по которому пришлось повреждение

	DamageInfo(float _amount, int _part, DamageCause _cause = dcUnknown, ObjectID _damager = 0);

	Registered *GetDamager() const;
};

#endif // wVulnerable_H
#ifndef DistanceKillerManager_H
#define DistanceKillerManager_H

#include "edObjects.h"
#include "vector.h"
#include <ed/set.h>

class DistanceKiller;

// Менеджер для оповещения объектов о покидании ими объема
class EDOBJECTS_API DistanceKillerManager
{
friend class DistanceKiller;
private:
	void Register(DistanceKiller *killer);
	void Unregister(DistanceKiller *killer);

protected:
	float threshold;
	Vector3 last_pos;
	bool lock;

	typedef ed::set<DistanceKiller *> killerList;
	killerList objects;

public:
	DistanceKillerManager(float threshold);
	~DistanceKillerManager();

	// изменить порог для Update
	void SetThreshold(float threshold);

	// произвести оповещение
	// произведется только если положение сместилось болше чем на threshold
	void Update(const Vector3& pos);
};

// менеджер по умолчанию
extern EDOBJECTS_API DistanceKillerManager cameraKiller;

#endif // DistanceKillerManager_H
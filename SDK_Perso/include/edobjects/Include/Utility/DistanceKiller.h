#ifndef DistanceKiller_H
#define DistanceKiller_H

#include "edObjects.h"
#include "vector.h"

class DistanceKillerManager;

// Интерфейс для объектов, существующих только в некотором объеме (например вокруг камеры)
// Как только объект покидает этот объем ему приходит сообщение и он удаляется из списка
class EDOBJECTS_API DistanceKiller
{
private:
	float distance;
	DistanceKillerManager *manager;
	bool registered;

public:
	// по умолчанию - вокруг камеры
	DistanceKiller(float distance, DistanceKillerManager *manager = 0);
	~DistanceKiller();

	void SetKillerDistance(float distance);
	float GetKillerDistance();

	// зарегистрировать
	void RegisterKillByDistance();
	void UnregisterKillByDistance();

	// получить координаты объекта
	virtual const Vector3& GetKillerPos() = 0;

	// при покидании зоны
	virtual void OnKillByDistance() = 0;
};

#endif // DistanceKiller_H
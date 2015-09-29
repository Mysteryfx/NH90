#ifndef ICivilianActivityManager_H
#define ICivilianActivityManager_H

#include "Inter.h"
#include "Registry\Registered.h"

class wcaActivity;

// Менеджер глобальных гражданских деятельностей мира:
// перевозка товаров, почты и т.д.
class ICivilianActivityManager
{
public:
	typedef ed::vector<wcaActivity *> activityList;
    virtual ~ICivilianActivityManager() {};

	// Инициализация мирных деятельностей (при старте программы или изменении настроек)
	// загружает конфигурацию и маршруты из пути path
	virtual void Init(const char *path) = 0;
	virtual void Uninit() = 0;

	// Запуск/остановка мирных деятельностей (при старте/выходе из миссии)
	virtual void Start() = 0;
	virtual void Stop() = 0;

	// Приостоновка работы 
	virtual void Pause() = 0;

	// Возобновление работы
	virtual void Resume() = 0;

	// Регистрация раненых
	virtual void registerDamage(ObjectID car, ObjectID hitman, float damage) = 0;
	// Регистрация умерших
	virtual void registerDeath(ObjectID, double modelTime, ObjectID hitman) = 0;
	// Регистрация остановленных
	virtual void registerStop(ObjectID, double modelTime) = 0;
	
	virtual void OnRoadObjectDestroyed(ObjectID bridgeId) = 0;

	virtual wcaActivity* getActivity(int actId) = 0;
	virtual activityList& getActivities() = 0;
};

extern INTER_API ICivilianActivityManager *civilianActivities;

#endif // ICivilianActivityManager_H

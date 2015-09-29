#ifndef wcaManager_Implement_H
#define wcaManager_Implement_H

#include <ed/string.h>

#include "Transport.h"
#include "wcaActivity.h"
#include "ICivilianActivityManager.h"
#include "wcaRouteConst.h"

// менеджер также является пространственным хранилищем для миражных объектов
#include "HierarchyStorage\HierarchyLayer.h"

#include "IwNetObject.h"
#include "WorldMail.h"


class wcaManager_Implement
	: public ICivilianActivityManager
	, public HierarchyLayer
	, private IwNetObject
{
private:
	bool inited;
	bool paused;

	ed::string routepath;
	activityList activitiesById;

	typedef ed::map<ed::string, wcaRouteDescriptor> routeTypeList;
	routeTypeList routeTypes;

protected:
	// из интерфейса HierarchyLinearNode
	// должна вернуть true если object должен попасть в nodes
	// если false => воткнуть в одного из детей
	virtual bool PreserveHere(HierarchyNode *object);

	// загрузка типов маршрутов
	virtual bool LoadRouteTypes(const ed::string& config);

	// Подгрузка маршрутов
	virtual void LoadRoutesR(const ed::string& path);

public:

	TRANSPORT_API wcaManager_Implement();
	~wcaManager_Implement();

	// Инициализация мирных деятельностей (при старте программы или изменении настроек)
	// загружает конфигурацию и маршруты из пути path
	virtual void Init(const char *path);
	virtual void Uninit();

	// Запуск/остановка мирных деятельностей (при старте/выходе из миссии)
	virtual void Start();
	virtual void Stop();

	// Приостоновка работы 
	virtual void Pause();

	// Возобновление работы
	virtual void Resume();

	// Добавление/удаление деятельности
	virtual void AddActivity(wcaActivity *activity);
	virtual void RemoveActivity(wcaActivity *activity);
	wcaActivity* getActivity(int actId);

// ASYNCNET {
	virtual void registerDamage(ObjectID car, ObjectID hitman, float damage);
	virtual void registerDeath(ObjectID, double modelTime, ObjectID hitman);
	virtual void registerStop(ObjectID, double modelTime);

	void OnRoadObjectDestroyed(ObjectID bridgeId);
	activityList& getActivities() {return activitiesById;}

private:
    // Демедж объектов, который надо послать серверу
	struct DamageEntry {
		ObjectID obj;
		float damage;
		ObjectID hitman;

			DamageEntry() : obj(0), damage(0), hitman(0) {}
			DamageEntry(ObjectID o, float d, ObjectID h)
				: obj(o), damage(d), hitman(h) {}
	};
    ed::vector<DamageEntry> _damage_list;

	// Трупики машинок.
	ed::vector<std::pair<ObjectID, float> > _dead_list;
	ed::vector<std::pair<ObjectID, double> > _stop_list;

	static void procDamageList(Mail::Reader);
	static void procInitData(Mail::Reader);

	virtual bool netGetCreate(Mail::Message&) const;
	virtual bool netSendUpdate(int dest, int prio, int& cookie);

	void regMail();
	void unregMail();
// } ASYNCNET
};

#endif // wcaManager_Implement_H

#ifndef wcaMirageActivity_H
#define wcaMirageActivity_H

#include "Transport.h"
#include "CivilianActivity\wcaActivity.h"
#include "CivilianActivity\wcaCar.h"
#include "SurfaceGroup\wcRoute.h"
#include "Misc\misc_plFile.h"
#include "CivilianActivity\wcaRouteConst.h"
#include "Randomizer\RandomObject.h"
#include "dPosition.h"
#include "wActivityManager.h"

// деятельность также является пространственным хранилищем для своих объектов - миражей
#include "HierarchyStorage\HierarchyLinearNode.h"

class lRoadDestroyedObject;

// деятельность - миражи объектов, едущие по маршруту
class TRANSPORT_API wcaMirageActivity : public wcaActivity, public wcRoute, public HierarchyLinearNode
{
public:
	struct RoadObject
	{
		RoadObject(ObjectID bridgeId,  bool dirSign, double startPar, double endPar)
			:bridgeId(bridgeId), dirSign(dirSign), startPar(startPar), endPar(endPar)
		{}

		ObjectID bridgeId;
		bool dirSign;
		double startPar, endPar;
	};
	typedef ed::vector<RoadObject> RoadObjects;

	struct StopZone
	{
		StopZone(ObjectID bridgeId, int wcaActId,bool dirSign, 
			double startPar, double endPar)
			:bridgeId(bridgeId), wcaActId(wcaActId), dirSign(dirSign), 
			startPar(startPar), endPar(endPar)
		{}
		ObjectID bridgeId;
		int wcaActId;
		bool dirSign;
		double startPar, endPar;
	};
	typedef std::multimap<ObjectID, StopZone> AllStopZones;
	typedef ed::map<double, const StopZone*> RouteStopZones;

	static RandomObject randomEvenly;

	wcaMirageActivity();
	~wcaMirageActivity();

	// инициализация маршрута
	virtual void Init(const wcaRouteDescriptor &desc);

	// загрузить маршрут из r-файла
	virtual bool LoadRoute(NM_ROAD::Road& road);

	static const AllStopZones& allStopZones() {return _allZones;}
	static void setStopZones(const AllStopZones& zones);
	double AppendStopZone(const wcaMirageActivity::StopZone* zone, double carParam, double carLength);
	double stopZoneCarOffset() const {return _stopZoneCarOffset;};
	const StopZone* CheckStopZones(double par);
	void OnRoadObjectDestroyed(ObjectID bridgeId);
	// если count = 0 => создать сколько поместится на маршруте
	// если delay = 0 => распределить по маршруту равномерно
	virtual void CreateCars(int count=0);
	
	// создание машинки (передается номер и начальный параметр)
	virtual wcaCar *CreateCar(int n, double initpar);

	// получить эффективное модельное время (с вычетом пауз)
	virtual wModelTime GetEffectiveTime(wModelTime t) const;

	void Start();
	void Stop();

	void Pause();
	void Resume();

	int Id() {return id;}
	void SetId(int id) {this->id = id;};

	const wcaRouteDescriptor& getInfo() const {return info;};
	// мы примем только ожидаемую машинку
	virtual bool AddObject(HierarchyNode *object);
	virtual void DeleteObjects();

	// сбор объектов (из интерфейса HierarchyLinearNode)
	virtual bool GetObjects(viSearch_Implement *search, IntersectionType it);

protected:
	//id уникален только среди дорожных активностей, а не всех объетов мира
	//фактически - индекс в массиве активностей
	int id;

	// константы маршрута
	wcaRouteDescriptor info;
	float turnR;
	float width;

	// для остановки
	bool paused;
	wModelTime pause_t;
	wModelTime spent_in_pause;

	// пересчитать описанный бокс
	virtual void CalculateBox();
	void InitRoadObjects();

	// получить максимальную ширину и радиус разворота
	virtual void GetStats();
	void update(wModelTime& t);

private:
	RoadObjects _roadObjects;
	RouteStopZones _routeZones;
	static AllStopZones _allZones;
	wActivityManager _activities;
	double _stopZoneCarOffset;
};

#endif // wcaMirageActivity_H

#ifndef wcRoute_H
#define wcRoute_H

#include <ed/map.h>
#include <utility>

#include "Transport.h"
#include "Common.h"
#include "wiPath.h"
#include "Paths\lPath.h"
#include "Mem\Shared.h"
#include "GroupFormations\Formation.h"

class Road;

namespace NavMeshData
{
	class PathPoints;
	struct PathPoint;
	struct SurfInfo;
}

class wcRoute;
namespace Mail 
{ 
	class Stream; 
	Stream& operator>>(Stream& stream, wcRoute*& data);
}

const float GroupWidthToMovOnRoad = 10.f;

// флаги для передвижения
const int MOVE_ON_SEA		= 1;
const int MOVE_ON_LAND		= 2;
const int MOVE_BY_ROAD		= 4;
const int MOVE_BY_RAIL		= 8;

// дистанция при объезде (до края препятствия)
const float DiversionOffset = 2.f;

// точка на маршруте (par заполнится при создании пути)
struct /*TRANSPORT_API*/ RoutePoint 
{
	RoutePoint ()
	{
		id = -1;
		x = 0./*f*/;
		z = 0./*f*/;
		flags = 0;
		par = 0./*f*/;
		speed = 0./*f*/;
		delay= 0./*f*/;
		formation = 0;
		formChangeable = true;
		hasTask = false;
		etaLocked = false;
		eta = 0.;
		alt = 0.;
	}

	~RoutePoint();

	int id;//Номер точки установленный в редакторе миссий
	//для точек не из редактора id == -1
	double x, z, alt;
	unsigned flags;
	double par;

	// дополнительные параметры, не используемые напрямую в wcRoute
	double speed, delay;
	ed::string formTemplId;
	
	//Вместо speed можно задать время прибытия
	//etaLocked оределяет на какой параметр ориентироваться - скорость или время прибытия
	double eta;//== wModelTime в секундах
	bool etaLocked;

	//на этом участке пути формация может быть изменена (например на мостах не может быть)
	//formChangeable учитывается только при выставленной formation
	bool formChangeable;
	// в этой точке колонна должна выполнить задачу
	bool hasTask;
	Mem::Ptr<Formation> formation;
	CarFormationType formationType;
	bool isHumanForm;//FIXME поле isHumanForm только для сериализации формаций, сделать нормальную сериализацию

private:
//	RoutePoint (const RoutePoint&);
//	const RoutePoint& operator=(const RoutePoint&);
	friend Serializer& operator<<(Serializer&, RoutePoint& data);
};

#define IID_IRoute Common::identify<wcRoute>() 

// класс для создания маршрута и отслеживания передвижения по нему
class TRANSPORT_API wcRoute : public Common::FakeUnknown<Serializable>
{
public:
	typedef ed::map<float, RoutePoint>							RouteType; // map<par, point>
	typedef ed::vector<cVector>								ApproximatedPath;
	typedef wiPath												MappedApproximatedPath;
protected:
	RouteType route;
	NM_ROAD::Road *path;

	// создать новый путь
	bool CreatePath(RoutePoint &start, RoutePoint &finish, float turnR, float width, unsigned flags);

	// получить отступ от центра дороги
	virtual float PathOffset(float width, float roadWidth, unsigned flags);

	void insertRP(double pathPar, const osg::Vec2d& p, CarFormationType form, bool isHumanColumn, double speed, bool formChangeable, int flags);
	void addRoadSegments(const NavMeshData::PathPoint& p1, const NavMeshData::PathPoint& p2, double maxCarWidth,
		ed::vector<NM_ROAD::Segment>& segs, double& pathLength);
	void addCrossroadSegments(const NavMeshData::PathPoint& p1, const NavMeshData::PathPoint& p2, double maxCarWidth,
		ed::vector<NM_ROAD::Segment>& segs, double& pathLength);
	bool addRoadPath(const RoutePoint& rp1, const RoutePoint& rp2, double maxCarWidth, 
		ed::vector<NM_ROAD::Segment>& segs, float &pathParam);
public:
	wcRoute();
	wcRoute(RoutePoint *points, int n);
	wcRoute(RoutePoint **points, int n);
	virtual ~wcRoute();

	static float InitPar(int i=0);

	// синхронизация эквидистантных путей
	virtual float GetSegmentParam(float path_par) const;
	virtual float GetPathParam(float segm_par) const;

	virtual void SetPath(NM_ROAD::Road *_path, float seg_start, float seg_end);
	NM_ROAD::Road *Path() const;

	bool approx3d(ApproximatedPath & approximatedPath, float offset, float back_offset = -1.0f, float tolerance = 10.0f, float angleTolerance = Pi/6) const;
	bool approx2d(ApproximatedPath & approximatedPath, float offset, float back_offset = -1.0f, float tolerance = 10.0f) const;	
	static void mapApproximatedPath(const ApproximatedPath & approximatedPath, MappedApproximatedPath & MappedApproximatedPath);

	void getPathPoint(float offset, double dtime, cVector * point, bool * onRoad = NULL) const;

	const RouteType & getWaypoints() const { return route; }
	void setWaypoints(RouteType &waypoints);

	void setSpeed(double speed);

	virtual float Length() const;
	virtual float TurningRadius(float par) const;
	virtual bool OnRoadSegment(float begin, float end) const;

	virtual RoutePoint *Next(float par);

	virtual void ClearRoute();

	// очистить путь
	virtual void ClearPath();

	// создать новый или дополнить путь
	virtual bool CreateGroupPath(float maxCarWidth, double maxSlope, bool canForceRiver, bool isHumanColumn, bool useBypassRoute = true);
	virtual bool CreateGroupPath(const NavMeshData::PathPoints& pp, 
		CarFormationType startForm, bool isHumanColumn, double speed, 
		float maxCarWidth, bool roadsPriorityPar);

	bool addBypassPath(const RoutePoint& rp1, const RoutePoint& rp2, const NavMeshData::SurfInfo& criticalSurf, double maxCarWidth, bool isHumanColumn, 
		ed::vector<NM_ROAD::Segment>& segs, float &pathParam, RouteType& newRoute);
	bool addOffRoadPath(const RoutePoint& rp1, const RoutePoint& rp2, double maxCarWidth, const NavMeshData::SurfInfo& criticalSurf, 
		bool isHumanColumn, bool useBypassRoute, 
		ed::vector<NM_ROAD::Segment>& segs, float &pathParam, RouteType& newRoute);

	void ChangeOnPathFormation(Mem::Ptr<Formation> form);
	bool CanChangeFormation(double pathPar);//можно ли менять формацию в данной точке пути

	// для построения эквидистантного пути [begin, end]
	// путь в main должен быть уже построен
	// off - смещение вправо от основного пути
	virtual bool CreateOffsetPath(const wcRoute& main, float off, float begin, float end);
	
	// вставить путь вместо [start, finish]
	virtual void AppendRoute(const wcRoute& addRoute, float startFrom);

	// обрубить путь
	virtual void CutOffPath(float end);

	// создать путь объезда (начало поиска, препятствие, с какой стороны, параметры объезда)
	static NM_ROAD::Road *CreateConnectionPath(const cVector& pFrom, const cVector& dirFrom,
		const cVector& pTo, const cVector& dirTo, float turnR);

	const NM_ROAD::Segment *getSegmentByParam(double pathParam) const;
	double getPathParam(double startFrom, double endPar, const osg::Vec2d& pos) const;

	virtual void	serialize(Serializer &);
	void printPath() const;

	friend Mail::Stream& Mail::operator>>(Mail::Stream& stream, wcRoute*& data);
};

#endif // wcRoute_H

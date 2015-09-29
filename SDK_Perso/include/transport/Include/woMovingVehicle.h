#ifndef woMovingVehicle_H
#define woMovingVehicle_H

#include "Transport.h"

//#include "wsFlightPlan.h"

#include "wTime.h"
#include "wMovingObject.h"
#include "LandObjects/lBridge.h"
#include "wMovingObjectConst.h"
#include "Randomizer\RandomObject.h"
#include "MovingPlatform.h"

#include "../../pathfinder/include/SurfInfo.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

#include <osg\Vec3d>
#include <osg\Vec2d>
#include <hash_map>
//#include "LandObjects\lBridge.h"

// базовый класс для машинок 
// умеет передвигаться по поверхности, в том числе в составе группы
class MovingVehicle;
class ObstacleGroup;
enum VehicleStateType;

namespace NavMeshData
{
	class MeshPolygon;
}

namespace Lua {class Config;}
namespace osg {class Vec3d;}

extern bool stopOnLandObstacles;

#ifndef USE_TERRAIN4
typedef landscape::lPointOnSurface PointOnSurface;
#else
typedef edterrain::PointOnSurfaceAdapter PointOnSurface;
#endif

class TRANSPORT_API woMovingVehicle : public MovingObject, public BridgeListener, public MovingPlatform
{
private:
	MovingVehicle *_impl;

	typedef stdext::hash_map<const MovingVehicle*, const woMovingVehicle*> ImplDictionary;
	static  ImplDictionary _vehiclesImplDictionary;

	const NavMeshData::MeshPolygon *_curPoly;


	enum SOType
	{
		soNone = 0,
		soBridge,
		soTunnel,
		soLandHeliport,
	};

	struct IntersectionCache
	{
		IntersectionCache()
			:_soType(soNone), _cacheY(0.)
		{

		}

		SOType _soType;
		double _cacheY;
		cVector _cacheN;
	};

	IntersectionCache _cacheFront, _cacheBack;
	dVector _lastCheckedPos;
	NavMeshData::SurfType _curSurfType;

	void updateMOPosition(wModelTime timeStep);
	void GetGearsPosition(osg::Vec3d& front, osg::Vec3d& back, cVector* nf, cVector* nb, float delta = 0.f);
	void PositionByGears(const osg::Vec3d &frontGear, const osg::Vec3d &backGear, osg::Vec3d &x, osg::Vec3d &pos) const;

	bool checkRideOnObstacles(double x, double z, double& height, cVector* n, IntersectionCache *cache);
	
	//setFromSurfInfo возвращают true если мы едем по тоннелю, мосту или препятствию (типо площадки подскока)
	//false - иначе. Если true требуется check delta height
	bool setFromSurfInfoWithObstacles(osg::Vec3d& pos, cVector* n, IntersectionCache *cache, bool calcRiverDepth, 
		double lastHeight, double delta);
	bool setFromSurfInfo(osg::Vec3d& pos, cVector* n, IntersectionCache *cache, bool calcRiverDepth);
	
//	bool doubleCheckDeltaHeight(const osg::Vec3d& pos, double lastHeight);
	void stopWithRemovingFromGroup(bool fast);
	void checkSpecialObjects();
	void сheckLandscape();
	void stopOnLandCondition(bool immediatly);

	inline double maxDeltaHeight(double dist) const;

	// кэш для экстраполированной позиции
	dVector             pos_p_ex;   
	wModelTime			PosUpdateTimeEx;

protected:
	void PrintTrace();
	void loadConfig(Lua::Config&);
	void initPosition(float x, float z, float bearing, bool checkObjectsHeight = true);
    
	//  Характеристики машины из базы данных
	float			X_gear_f; // Координаты нижней кромки переднего 
	float			Y_gear_f; // правого и заднего правого колеса
	float			Z_gear_f; // в связанной системе координат.
	float			X_gear_b;
	float			Y_gear_b;
	float			Z_gear_b;

    //double          k_max_speed;

public:
	// для одиночной машинки
	woMovingVehicle();
	virtual ~woMovingVehicle();

//    void    setLandGroup(float x, float z, wcLandGroup *group);
	int		Class(void) {return woClass_Vehicle;}
	void	nextStep(wModelTime timeStep); 
	
	inline MovingVehicle* vehicleImpl(){return _impl;}
	static const woMovingVehicle* getByImpl(const MovingVehicle* impl);

	// получить точную координату объекта
    virtual dVector     GetPoint(wModelTime t);
    virtual dVector		GetPoint();

	// едем по реальной дороге
	virtual bool IsOnRoad();
	NavMeshData::SurfType surfTypeFromMesh(PointOnSurface& sp);

	// BridgeListener
	void OnRoadObjectDestroyed(const lRoadDestroyedObject *bridge);

	virtual void serialize(Serializer &);

	bool canForceRiver() const;
	double maxSlopeAngle() const;
	double vertObstacle() const;
	double length() const;
	double width() const;
	double mass() const;
	double turnR() const;
	double maxSpeed () const;
	double speed() const;
	double radius() const;
	bool isStopped() const;

	void updateAIPos(); // обновить позицию MovingVehicle, чтобы его могли объезжать другие машинки

	// последняя обновлённая позиция машинки без дополнительных вычислений
	dVector fastPos() const {return pos_p;}

    virtual void setTopSpeedHandicap(double _MaxSpeedK);
	double tacticalSpeed() const;
    bool can_tacticalSpeed() const {return true;};
	void setTacticalSpeed(double speed);
	
	void setTacticalDir(const osg::Vec2d& dir); //установить желаемую ориентацию машинки для срельбы
	const osg::Vec2d& tacticalDir() const; //если ориентация не установлена возвращает (0,0)
    bool can_TacticalDir() const {return true;};
	bool tacticalDirArrived() const; //желаемая ориентация достигнута и машинка стоит
    bool tacticalDirFixed() const; 

	void startAgony();

	VehicleStateType vehicleState() const;
	bool isStaying() const;
    bool isStayingEx() const;

	void updateNeighbours(const MovingVehicle* deathVeh);
//	void updateRideOnObstacles(const viObjectShape* obs);

	// сейчас isOnBridge и isInTunnel считается по передней оси

	bool isOnBridge() const;
	bool isInTunnel() const;

	static RandomObject randomEvenly;
};

#endif // woMovingVehicle_H
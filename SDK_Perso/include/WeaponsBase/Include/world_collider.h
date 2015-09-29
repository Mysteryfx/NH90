#ifndef __WORLD_COLLIDER_H__
#define __WORLD_COLLIDER_H__

#include "WeaponsBase.h"

#include "Math/Vector.h"
#include "wTime.h"
#include "Registry/Registered.h"

class viObject;
class viSearch;
class viObjectHandle;
class ISceneObject;

class WEAPONSBASE_API wWorldCollider
{
public:
    static const viFlag flagStatic;
    static const viFlag flagDynamic;
    static const viFlag flagAll;
public:
	wWorldCollider();
	~wWorldCollider();

	bool check(	wModelTime time, 
				ObjectID self,
				ObjectID launcher,  
				int cnt, 
				const Math::Vec3d* pos, 
				const Math::Vec3d* vel, 
				ObjectID* ids, 
				double* dts,
				Math::Vec3d* local_pts, // в локальной системе координат объекта
                Math::Vec3d* normals,   // в глобальной системе координат
				ed::vector<ed::string>& part_names,
				double dt, 
				viFlag searchFlags);

protected:
	bool processIntersection_(viObject *item, viSearch *search, void *data);
	bool processIntersection_(const ISceneObject& sceneObject);
	//bool processIntersection_(const viObjectHandle& id);
	static bool processIntersectionS_(viObject *item, viSearch *search, void *data);

protected:	

	int N_;
	const Math::Vec3d* poss_;
	const Math::Vec3d* vels_;

	double dt_;

	ObjectID* colIDs_;
	double* collisionDts_;
	Math::Vec3d* collisionPoints_;
    Math::Vec3d* normals_;
    ed::vector<ed::string>* part_names_;

	ObjectID self_, launcher_;
};


#endif

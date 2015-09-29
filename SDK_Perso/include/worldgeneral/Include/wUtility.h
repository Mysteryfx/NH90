#include "WorldGeneral.h"
#include "cLinear.h"
#include "edTerrain/PointOnSurfaceAdapter.h"

class viObjectShape;
namespace landscape{
	class lPointOnSurface;
}

// recieves segment name, point in global coords and user data
typedef bool ModelSurfaceHandler(const ed::string& segment, const dVector& p, const dVector& a, void *data);

//#if 1
#ifndef USE_TERRAIN4
typedef landscape::lPointOnSurface PointOnSurface;
#else
typedef edterrain::PointOnSurfaceAdapter PointOnSurface;
#endif
// intersect object with surface
void WORLDGENERAL_API ModelSurfaceIntersect(viObjectShape& obj, PointOnSurface& surf, ModelSurfaceHandler *handler, void *data);


// обработчик пересечения
typedef bool IntersectObjectsHandler(const ed::string& segment, const ed::string& segment2, const Vector3& p_w, const Vector3& a_w, const Vector3& normal, void *data);

// пересечение объекта с объектом (с вызовом обработчика на каждом пересечении - для обработки нескольких пересечений)
bool WORLDGENERAL_API IntersectObjects(viObjectShape &obj1, viObjectShape &obj2,
	IntersectObjectsHandler* handler, void *data);

class ISceneObject;
bool WORLDGENERAL_API IntersectObjects(const ISceneObject& obj1,const ISceneObject& obj2, IntersectObjectsHandler* handler, void *data);
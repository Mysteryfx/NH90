#ifndef Modeler_Utility_H
#define Modeler_Utility_H

#include "graphicsxp.h"

#include "ed/vector.h"

class ClipVolume;

namespace Graphics
{
class ModelInstance;
class ModelConnector;

#ifndef _FINAL_VERSION
GRAPHICSXP_API void EnableCollisionShellRendering(bool enable);
GRAPHICSXP_API bool IsCollisionShellRender();
#else
inline void EnableCollisionShellRendering(bool enable){}
inline bool IsCollisionShellRender(){return false;}
#endif

// вспомогательные функции для работы с моделями
	
// проверка столкновений

// пересечение с отрезком, возвращается параметр на отрезке и нормаль в этой точке
bool GRAPHICSXP_API __cdecl IntersectSegment(ModelInstance &obj, const Vector3& a, const Vector3& b,
	ed::string& part, float *t = 0, Vector3 *n = 0);

// пересечение с отрезком заданным в локальных координатах, возвращается параметр на отрезке и нормаль в этой точке
bool GRAPHICSXP_API __cdecl IntersectSegmentLocal(ModelInstance &obj, const Vector3& la, const Vector3& lb,
										  ed::string& part, float *t = 0, Vector3 *n = 0);

// пересечение с другой моделью
bool GRAPHICSXP_API __cdecl IntersectModel(ModelInstance &obj1, ModelInstance &obj2,
	ed::string& part1, ed::string& part2, Vector3 *p = 0, Vector3 *n = 0);

// обработчик пересечения
typedef bool IntersectModelExHandler(const ed::string& segment, const Vector3& p_w, const Vector3& a_w, const Vector3& normal, void *data);

// пересечение с другой моделью (с вызовом обработчика на каждом пересечении - для обработки нескольких пересечений)
bool GRAPHICSXP_API __cdecl IntersectModelEx(ModelInstance &obj1, ModelInstance &obj2,
	ed::string& part1, ed::string& part2, Vector3 *p, Vector3 *n, Vector3 *p2, IntersectModelExHandler* handler, void *data);

// поиск ближайшего куска к точке, возвращает расстояние до куска и его имя
// если задан объем, будут выданы только куски попавшие в этот объем
float GRAPHICSXP_API __cdecl FindClosestPart(ModelInstance &obj, const Vector3& p, ed::string& part, ClipVolume *inVolume = 0);

//Структура описывающая элемент модели
typedef struct{
	float						d;			//Расстояние до тестируемой точки
	ed::string					part;		//Имя элемента модели
	ed::vector<ed::string>	closedBy;	//Список элементов модели, закрывающих элемент
}PART_DESC;

// поиск и сортировка по расстоянию кусков, ближайших к точке в заданном радиусе, возвращается кол-во кусков
int GRAPHICSXP_API  __cdecl FindAllParts(ModelInstance &obj, const Vector3& p, float r, ed::vector<PART_DESC> &parts, bool findBarrier);

GRAPHICSXP_API void getEffectsConnectors(ModelConnector **effectConnectors, ModelInstance &instance);

} // namespace Graphics

#endif // Modeler_Utility_H

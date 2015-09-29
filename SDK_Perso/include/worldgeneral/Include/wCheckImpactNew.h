#ifndef wCheckImpactNew_H
#define wCheckImpactNew_H

#include <ed/vector.h>

#include "WorldGeneral.h"
#include "box.h"

// Новая низкоуровневая проверка столкновений

// сетка
struct GridInfo
{
	int X,Y,Z;
};

extern WORLDGENERAL_API GridInfo planeGrid;
extern WORLDGENERAL_API GridInfo shipGrid;

// Найти номер ячейки по точке столкновения
// shape - форма объекта, должна содержать информацию о размерности ячеек
// p - точка в ЛСК объекта
// возвращает номер ячейки или 0 при ошибке
//int GetCellIndex(gShapeObject *shape, const cPoint& p);
WORLDGENERAL_API int GetCellIndex(const GridInfo& grid, const Box& box, const Vector3& p);

// найти список ячеек, попавших в сферу
// shape - форма объекта, должна содержать информацию о размерности ячеек
// p,r - центр и радиус сферы поиска
// результат: cells - список ячеек, попавших в сферу
//void GetCellIndices(gShapeObject *shape, const cPoint& p, float r, ed::vector<int> &cells);
WORLDGENERAL_API void GetCellIndices(const GridInfo& grid, const Box& box, const Vector3& p, float r, ed::vector<int> &cells);

#endif // wCheckImpactNew_H

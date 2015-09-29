#ifndef viHandlers_H
#define viHandlers_H

#include "viSearch.h"
#include "cLinear.h"

// здесь собраны обработчики при поиске, реализующие часто необходимые функции

// ничего не делает, просто возвращает true (так что функция поиска вернет количество объектов, попавших в объем)
bool EDOBJECTS_API viCounterHandler(viObject *item, viSearch *search, void *data);

// Считает только если Shape не равен 0
bool EDOBJECTS_API viShapeHandler(viObject *item, viSearch *search, void *data);

// Получить максимальную высоту объекта в области
// data - указатель на float (с результатом). Проинициализировать 0.
bool EDOBJECTS_API viMaxHeightHandler(viObject *item, viSearch *search, void *data);

// Поиск ближайшего объекта
struct EDOBJECTS_API viClosestHandlerInfo
{
	cPoint p;
	float len;
	viObject *closest;

	viClosestHandlerInfo(const cPoint& _p);
};

bool EDOBJECTS_API viClosestHandler(viObject *item, viSearch *search, void *data);

#endif // viHandlers_H
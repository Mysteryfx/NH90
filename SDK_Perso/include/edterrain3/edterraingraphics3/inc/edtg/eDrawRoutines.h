#ifndef EDTG_EDRAWROUTINES_H
#define EDTG_EDRAWROUTINES_H

#include "ed/string.h"

namespace edtg
{

// Типы отрисовки
enum eDrawRoutines
{
	UNKNOWN = -1,
	MAIN=0,				// Земля
//	LANDNIGHT=1,		// Земля ночью					// УДАЛИТЬ!!!
	MFD=2,			// На МФД
//	MFD_NIGHT=3,		// На МФД						// УДАЛИТЬ!!!
//	MIRROR_DAY=4,		// На Зеркала
//	MIRROR_NIGHT=5,		// На Зеркала					// УДАЛИТЬ!!!

	// Карта
	MAP=6,				// На карту
	MAPALT=7,			// На карту (земля с картой высот)
	MAPTEX=8,			// На карту (Снимок из космоса)

	//LANDNOISE=10,		// Для шумовой текстуры
	//	LANDLIGHT=11,		// Текстура освещения

	//TTF_SEARCH = 12,	// Поиск положения камеры для рендеринга текстур
	INFRARED	= 13,	// Инфракрасный МФД

	//LANDMIRROR	= 14,	// Отражение земли

	LANDLIGHT=15,	// Текстура освещения только ночю
	//LANDLIGHT_DAY=16,	// Текстура освещения только днем		// УДАЛИТЬ!!!
	GRASSHEIGHTMAP = 16,// карта высот и цвета новой травы

	RADAR	= 17,		// Радар

	LANDHEIGHTTEX = 18,	// Рендер земли в текстуру высот
	SHELF = 19,			 //Рендер подводных объектов в текстуру

	DEFERRED        = 20, // deferred shading pass
	SHADOWS         = 21, // shadowmap pass
	//	DEFERRED_LIGTHS = 22, // deferred lights
	LANDMASK         = 22, 
	CASCADSHADOWS    = 23,
	MAX_ROUTINE     = 24
};

enum enSurfaceRenderQueue
{
	SRQ_SURFACE		= 5,
	SRQ_FLATSHADOWS = 6,
	SRQ_OPAQUE		= 0,
	SRQ_TRANSPARENT = 1,
	SRQ_LIGHTS		= 2,
	SRQ_SHELF		= 3,
	SRQ_WAVES		= 4,
	SRQ_COMPUTE		= 7,
	maxQueueCount	= 8	// число очередей
};
// Получить индекс очереди по её имени, если имя кривое то возращаем opaque очередь
int getQueueIndex(const ed::string& queueName);
// Наоборот получить по индексу очереди её имя - для дебага
const char* getQueueName(int queueIndex);


}

#endif

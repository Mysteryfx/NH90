#ifndef LSEMANTICS_H
#define LSEMANTICS_H

#include "edterrain.h"
#include <stdint.h>

// Флаги семантик, для lsa4
enum edSemanticMask
{
	smaSea		= 0x001,	// Море
	smaLake     = 0x002,	// Озеро
	smaRiver    = 0x004,	// Река, канал

	smaRoad		= 0x008,	// дорога
	smaRailway  = 0x010,	// жд
	smaRunway	= 0x020,	// впп

	smaTown     = 0x040,	// город
	smaField    = 0x080,	// поле
	smaBeach    = 0x100,	// берег

	smaOther    = 0x80000000,// остальные
};
const uint32_t smaWATERS = smaSea|smaLake|smaRiver;

// Получить маску по семантике
int EDTERRAIN_API getSemanticMask(int semantic);
// Получить семантику по маске (считаем что поднят 1 бит из всех возможных)
int EDTERRAIN_API getSemanticByMask(int semantic);

const int typeMeshLand           = 0;  // Земля
const int typeMeshSea            = 1;  // Вода
const int typeMeshRoad           = 2;  // Автодороги
const int typeMeshBeach          = 3;  // Береговые линии
const int typeMeshRiver          = 4;  // Реки
const int typeMeshTown           = 5;  // Города
const int typeMeshLake           = 7;  // Озера
const int typeMeshIsland         = 8;  // Острова
const int typeMeshRailway        = 9;  // Ж.дороги
const int typeMeshChannel        = 15; // Каналы
const int typeMeshForest         = 16; // Forest
const int typeMeshGrass          = 17; // Grass
const int typeMeshStone          = 18; // Stone
const int typeMeshLine           = 20; // Географическая сетка на карте
const int typeMeshMGRS           = 21; // MGRS сетка на карте
const int typeMeshField          = 22; // поля
const int typeMeshRunway         = 23; // аэродром (впп)
const int typeMeshPlant          = 24; // растения
const int typeMeshELT            = 25; // ЛЭП (для карты)
const int typeMeshBuilding       = 26; // Строения
const int typeMeshIsoline        = 27; // Линии высот для карты
const int typeMeshSmallShit      = 28; // 
const int typeMeshTrees          = 29; // Деревья на кварталах
const int typeMeshLamp           = 30; // Лампочки
const int typeMeshShelf          = 31; // Шельф (поверхность дна)
const int typeMeshSurfaceDetails = 32; // Детали земли

const int typeMeshUndef          = 33; // На цифре 33 с меня в момент слетает хмель
const int typeMeshMAX            = 34;

// маски типов боксов (по типам проходов)
enum enBoxTypes
{
	BT_MAIN		= 1, 
	BT_LIGHTING	= 2,
	BT_FULL		= 0xFFFF,
};

//////////////////////////////////////
// Имена семантик
struct SemanticDef
{
//#ifdef EDGE
	//const char* name;
//#else
	char* name;
//#endif
	int index;
	int draw_priority;		// Порядок отрисовки накладных объектов
	float windY;			// Дополнительный ветер
	unsigned int color;	// Цвет для отладки
};
// Описание семантики по имени
EDTERRAIN_API SemanticDef* GetSemanticDecl(const char* semantic);
EDTERRAIN_API SemanticDef* GetSemanticDecl(int semantic);
// Тип семантики по имени
EDTERRAIN_API int GetSemanticType(const char* semantic);
// Список семантик
EDTERRAIN_API SemanticDef* GetSemanticList();
// Приоритет отрисовки
EDTERRAIN_API int GetSemanticPrior(int sem);

#endif
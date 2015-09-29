/*
//	Файл:				vPublic.h
//	Подсистема:			View (interface)
//
//	Экспортируемые структуры и константы
//	подсистемы управления видами
//
*/

#pragma once


// Перечисление типов камер
typedef enum vCameraType_e {
	vCameraTypeCockpit,				// Вид из кабины
	vCameraTypeNear,				// Наружная камера
	vCameraTypeTower,				// Вид с башни
	vCameraTypeChase,				// Следящая камера
	vCameraTypeFight,				// Камера данного и ближайшего объектов
	vCameraTypeAWACS,				// Вид АВАКС
	vCameraTypeFree,				// Свободная камера
//	vCameraTypeNatural,				// Камера в кабине (естественный взгляд) 
	vCameraTypeACarrier,			// Камеры на авианосце
	vCameraTypeStatic,				// Статические объекты миссии
//	vCameraTypeAlarm,				// Объекты будильника 
	vCameraTypeJFO,					// Камера над юнитом
    vCameraTypeG,					// Камера наземного места

	//	Служебные камеры визуализатора
    vCameraTypeCurrent,             // текущая камера
	vCameraTypeMirror,				// Создание изображения на зеркале
	vCameraTypeMFD,					// Создание изображения на MFD 
	vCameraTypeEnvMap,				// Генерация текстур Environment Map
	vCameraTypeMFD_Parralel			// Создание изображения на MFD карта
};

// Подтипы камер
typedef enum vCameraSubType_e 
{
	vCameraSubType_Undefined,		// Неопределенный подтип
	vCameraSubType_Planes,			// Самолеты
	vCameraSubType_Ground,			// Наземные объекты
	vCameraSubType_Navy,			// Корабли
	vCameraSubType_Weapon,			// Самолеты, ракеты и бомбы
	vCameraSubType_Others,			// Остальные объекты (отладка)
	vCameraSubType_LndgOfficer,		// руководитель полетами на авианосце
//	vCameraSubType_CaptainBridge,	// капитанский мостик на авианосце
//	vCameraSubType_GroundAndPlane,	// наземный объект + ваш самолет};
//	vCameraSubType_PlaneAndGround,	// ваш самолет + наземный объект
	vCameraSubType_FightAll,		// Камера данного и ближайшего объектов
//	vCameraSubType_FightEnemy,		// Камера данного и ближайшего вражеского объектов
//	vCameraSubType_FightFriend,		// Камера данного и ближайшего дружественного объектов
	vCameraSubType_WeaponAndTarget,	// ракетa и цель
	vCameraSubType_Targets,			// Цели миссии
//	vCameraSubType_Parachutists		// Парашютисты
	vCameraSubType_Mirage,			// Миражные объекты
	vCameraSubType_Rear,			// Вид из-за кабины назад
	vCameraSubType_Parachute,		// Из глаз выпрыгнувшего пилота
    vCameraSubType_Arcade
};

// Умалчиваемые исходные углы
//const float hAngleOrigin = 0.f;
//const float vAngleOrigin = -9.5f;
/*
// ограничения на поворот головы пилота
const float hAngleMin = -130.f;
const float hAngleMax =  130.f;
const float vAngleMin = -65.f;
const float vAngleMax =  115.f;
const float hAngleMinSu25 = -95.f;
const float hAngleMaxSu25 =  95.f;
const float vAngleMinSu25 = -65.f;
const float vAngleMaxSu25 =  80.f;
const float hAngleMinF15 = -135.f;
const float hAngleMaxF15 =  135.f;
const float vAngleMinF15 = -65.f;
const float vAngleMaxF15 =  115.f;
*/
const int nMaxThreatMissiles = 10;	// Максимальное количество угрожающих ракет
const float rThreatMissilesZone = 10000.0f;	// Радиус зоны поиска

// управлерие маркером взгляда (padlock)
const float MaxLockDistance = 10000.f;
// max углы захвата "взглядом"
const float vMaxAngleLock = 0.3927f; // TO_RAD(22.5f)
const float hMaxAngleLock = vMaxAngleLock * 4.f / 3.f;


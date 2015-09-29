#ifndef viFlags_H
#define viFlags_H

#include "Registry\Registered.h"

// типы viObject
const viFlag viMapObject			=  0x01;	// объекты карты, с которыми можно столкнуться (не двигаются)
const viFlag viMissionObject		=  0x02;	// объекты, выставленные в миссии (не двигаются)
const viFlag viDynamicObject		=  0x04;	// динамические объекты карты (воронки и обломки) (не двигаются)
const viFlag viMapNonCollidable		=  0x08;	// объекты карты, с которыми нельзя столкнуться (не двигаются)
const viFlag viWorldLightObject		=  0x10;	// легкие объекты мира (пули, бомбы, НУРСы)
const viFlag viWorldHeavyObject		=  0x20;	// тяжелые объекты мира (самолеты, машинки, корабли, ракеты и т.д.)
const viFlag viMirageObject			=  0x40;	// миражные объекты (из мирных деятельностей)
const viFlag viVolumeEffect			=  0x80;	// видео эффект (хвост от самолета и т.д.) (двигаются)
const viFlag viPilonObject			= 0x100;	// под-объект подвески и т.д. (двигаются)
const viFlag viPointObject			= 0x200;	// непространственные точечные объекты
const viFlag viSurface				= 0x400;	// кусок поверхности (пока - только для настроек камеры)
const viFlag viCloud				= 0x800;	// облака.
const viFlag viLightPersisted		=0x1000;	// источник света в составе сцен (не убивается при выходе)
const viFlag viLightTemporary		=0x2000;	// источник света созданный в миссии
const viFlag viCamera				=0x4000;	// камера, как отдельный объект мира
const viFlag viAtmosphericEffect	=0x8000;	// атмосферное возмущение (ветер, спутный след и т.д.)
const viFlag viAerodrome			=0x10000;	// аэродром, как отдельный объект мира
const viFlag viRadioBeacon          =0x20000;   // абстрактный радиомаяк , как объект мира
const viFlag viMapMissionObject		=0x40000;   // наземные объекты - составные элементы радиомаяка
const viFlag viSpot			        =0x80000;   // радио маяк , как объект мира
const viFlag viSatellite			=0x100000;  // ИСЗ , как объект мира
const viFlag viAirfield				=0x200000;  // new airfields
const viFlag viColumn				=0x400000;  // колонна машинок
const viFlag viDistrict				=0x800000;  // квартал
const viFlag viMapObjectInDistrict	=0x1000000;  // домик квартала
const viFlag viHelperObject     	=0x2000000;  // вспомогательный обьект отрисовки


// составные типы для поиска
const viFlag viLandObject			= (viMapObject | viMapNonCollidable | viMapObjectInDistrict);
const viFlag viWorldObject			= (viWorldLightObject | viWorldHeavyObject | viMirageObject);
const viFlag viCollidable			= (viMapObject | viMissionObject | viDynamicObject | viWorldHeavyObject | viMirageObject | viMapMissionObject | viMapObjectInDistrict);
const viFlag viSacrificial			= (viMapObject | viMissionObject | viWorldHeavyObject | viMirageObject | viMapMissionObject | viMapObjectInDistrict);
const viFlag viRenderable			= (viLandObject | viMissionObject | viDynamicObject | viWorldObject | viVolumeEffect | viCloud | viPilonObject | viMapMissionObject | viAirfield | viSpot | viHelperObject);
const viFlag viAny					= 0x7FFFFFFF;
const viFlag viLightSource			= (viLightPersisted | viLightTemporary);	// источник света

// смотрим занятие площадки другими объектами
const viFlag viNonstaticCollidable = viCollidable ^ (viMapObject|viMapObjectInDistrict);

#endif // viFlags_H

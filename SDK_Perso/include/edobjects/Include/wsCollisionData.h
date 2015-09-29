#pragma once

#include "cPosition.h"

// типы столкновения объекта
const char	WITH_GROUND			= 0; // с землей
const char	WITH_MAP_OBJ		= 1; // с объектом карты 
const char	WITH_WORLD_OBJ		= 2; // с объектом мира
const char	WITH_EXPLOSIVE_WAVE	= 3; // поражение взрывной волной
const char	WITH_CRATER	        = 4; // c воронкой и обломками

// варианты передачи точки столкновения 
const char	WORLD_PNT			= 0;	// координаты точки столкновения заданы в мире 
const char	LOCAL_OWN_PNT		= 1;	// координаты точки столкновения заданы в ЛСК "своего" объекта 
const char	LOCAL_NOT_OWN_PNT	= 2;	// координаты точки столкновения заданы в ЛСК "чужого" объекта

// данные о столкновении
struct wsCollisionData //данные для обработки столкновения объекта
{ 
	char			type_collision; // тип столкновения объекта (WITH_GROUND|WITH_MAP_OBJ|WITH_WORLD_OBJ|WITH_EXPLOSIVE_WAVE)
	short			number_cell; // номер ячейки "своего" объекта, в которой находится точка столкновения с "чужим" объектом
	ed::string		part;
	char			type_pnt; // тип задания точки столкновения(WORLD_PNT|LOCAL_OWN_PNT|LOCAL_NOT_OWN_PNT)
	cVector			pnt; // точка столкновения объектов
	float			damage; // ущерб, который "несет" "чужой" объект (==0, если не известен или пока не рассчитан)
	cVector			velocity;
	friend Serializer& operator << (Serializer& serializer, wsCollisionData& data)
	{
		return serializer 
			<< data.type_collision       
			<< data.number_cell	
			<< data.part
			<< data.type_pnt			
			<< data.pnt			
			<< (float&)data.damage
			<< data.velocity;
	}	
};

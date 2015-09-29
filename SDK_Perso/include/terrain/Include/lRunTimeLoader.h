#pragma once

#include "Terrain.h"

// Менеджер загрузки наземных объектов, их графических образов и сцены в целом "на лету"
class TERRAIN_API lRunTimeLoader
{
protected:
	float around_camera, around_objects;
	viFlag around_types, preload_types;

public:
	lRunTimeLoader();

	// заполнить параметры из конфига
	void FillParams();
	
	// загружает объекты в радиусе вокруг данного
	void PrecacheAroundObject(const cPoint& obj);

	// Вызывается при старте миссии 
	// подгружает наземные объекты вокруг камеры и объектов мира
	void PrecacheScene();

	// Вызывается на каждом кадре (или несколько реже)
	// Оставляет задание для TaskManager по подгрузке объектов, к которым подлетаем
	void UpdateCache();
};

extern TERRAIN_API lRunTimeLoader scenePreloader;


#include "math_serialize.inl"

template <class S>
S& operator >>(S& s, roads3::Segment& v ) 
{
	uint32_t version = 1;
	s >> version;
	if(version>=0)
	{
		s >> *(int32_t*)&v.Type; // line(0) or arc(1)
		s >> v.length;    // накопленная длина
		s >> v.p1;		 // нач. точка
		s >> v.p2;		 // кон. точка

		// (используется только для окружности)
		s >> v.cen;	 // центр 
		s >> v.radius;    // радиус дуги (используется только для окружности)
		s >> v.st_angle;  // нач. угол (против ЧС) (используется только для окружности)
		s >> v.en_angle;  // кон. угол (используется только для окружности)
		// (используется только для B-spline)
		s >> v.p1_b;		 // точка p1_B
		s >> v.p2_b;		 // точка p2_B
		// Сегментный параметр (ГРИША)
		s >> v.segmentParam;

		s >> v.flags;// бит[0] - 1 - обратное 0 - прямое - (направление вхождения в дорогу) (используется только для окружности)
	}
	if(version>=1)
	{
		s >> v.bridgeHeight;	// Высота моста
	}
	return s;
}


template <class S>
S& operator >>(S& s, roads3::RoadSceneObject& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.objindex;
		s >> v.flags;
		s >> v.segment;			// Параметр на дороге
		s >> v.param;			// ---
	}
	return s;
}

template <class S>
S& operator >>(S& s, roads3::RoadAnchor& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.crossindex;
//		s >> v.roadindex;
		s >> v.bStart;
	}
	return s;
}

template <class S>
S& operator >>(S& s, roads3::Road& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		// символьное имя дороги
		s >> v.name;
		// ширина проезжей части дороги
        s >> v.width;
		// перекресток начала дороги
		s >> v.st_cross;
		// перекресток конца дороги
        s >> v.en_cross;
		// флаги дороги [0] бит == 1, если вдоль железной дороги нужно ставить столбы
		s >> v.flags;
		// Бокс
		s >> v.box;
		// for GRISHA
		s >> v.zeroSegmentParam;

		// Список сегментов
		s >> v.segments;
		// Список объектов на дороге
		s >> v.objects;
	}
	return s;
}
template <class S>
S& operator >>(S& s, roads3::Crossroad& v ) 
{
	uint32_t version = 1;
	s >> version;
	if(version>=0)
	{
		s >> v.typeind;

		s >> v.nTexture;	// Текстурный индекс перекрестка (archaic)
		s >> v.flag;		// Флаги перекрестка
		s >> v.pos;			// Позиция перекрестка
		s >> v.rotateangle;		// Угол поворота (тип перекрестка)

		// объекты сцены
		s >> v.objectinds;

		// Массив дорог (отсортирован по углам)
		s >> v.roadinds;
	}
	if(version>=1)
	{
		s >> v.name;		// crossroad name (for taxiways)
	}
	return s;
}

template <class S>
S& operator >>(S& s, roads3::RoadLink& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.roadindex;

		s >> v.segments;	// Текстурный индекс перекрестка (archaic)
//		s >> v.bits;		// Флаги перекрестка
		s >> v.box;			// Позиция перекрестка
	}
	return s;
}

template <class S>
S& operator >>(S& s, roads3::Square& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.squareindex;
		s >> v.roads;
		s >> v.crossinds;
	}
	return s;
}

template <class S>
S& operator >>(S& s, roads3::SceneObject& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.name;
		s >> v.pos;
		s >> v.roadindex;
		s >> v.crossindex;
	}
	return s;
}



template <class S>
S& operator >>(S& s, roads3::CrossroadType& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.name;
		s >> *(int32_t*)&v.rn_type;
		s >> v.model;
		s >> v.roads;
		s >> v.crosses;
		s >> v.crossangles;
	}
	return s;
}


// Углы привязки
template <class S>
S& operator >>(S& s, roads3::CrossroadType::CrossAngle& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.crossindex;
		s >> v.angle;				// Угол привязки (от центра)
		s >> v.roadtype;			// Тип дороги
	}
	return s;
}

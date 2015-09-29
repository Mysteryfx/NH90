
template <class S>
S& operator >>(S& s, surface3::lSurfaceFile::Level& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.level;
		s >> v.size;
		s >> v.sqmin;
		s >> v.sqmax;
		s >> v.squarepool;
		s >> v.pSquares;
	}
	return s;
}
template <class S>
S& operator >>(S& s, surface3::lSurfaceFile::Heigth& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.sqmin>>v.sqmax;
		s >> v.pHeigths;
	}
	return s;
}

template <class S>
S& operator >>(S& s, surface3::Square::SideBoundFaces& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.boundfaces;
	}
	return s;
}



template <class S>
S& operator >>(S& s, surface3::Square& v ) 
{
	s >> *(geometry3::GeometrySource*)&v;

	uint32_t version = 2;
	s >> version;
	if(version>=0)
	{
		// Баунд квадрата
		s >> v.bound;
		// индекс центрального тр-ка
		s >> v.centerFace;

		// Части
		s >> v.parts;
		// смежные тр-ки
		s >> v.faceNeighbours;
		// смещение на массив исходных нормалей вершин n3V*
		s >> v.sourcenorms;
		// Далее записи о тр-ках на границе
		s >> v.SideBounds[0];
		s >> v.SideBounds[1];
		s >> v.SideBounds[2];
		s >> v.SideBounds[3];
		// Индексы частей тр-ков
		s >> v.facetypes;
	}
	if(version>=1)
	{
		// Исходный файл (чтоб ошибки искать)
		s >> v.sourcefilename;
	}
	if(version>=2)
	{
		// Семантики треугольников
		s >> v.facesemantics;
		s >> v.semanticlist;
	}

	return s;
}


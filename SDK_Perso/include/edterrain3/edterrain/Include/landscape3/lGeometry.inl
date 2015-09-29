
template <class S>
S& operator >>(S& s, geometry3::BoundData& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.box;
		s >> v.center;
		s >> v.radius;
	}
	return s;
}
/*/
template <class S>
S& operator >>(S& s, geometry3::GeometryItem::SubItem& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.textureindex;
		// Начальное смещение
		s >> v.startFace;
		s >> v.startVert;
		// Число
		s >> v.nFaces;
		s >> v.nVerts;
	}
	return s;
}
/*/

template <class S>
S& operator >>(S& s, geometry3::GeometryItem& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.materialindex_infile;
		// Начальное смещение
		s >> v.startFace;
		s >> v.startVert;
		// Число
		s >> v.nFaces;
		s >> v.nVerts;
		// Bound
		s >> v.bound;
		// Приоритет отрисовки (слой)
		s >> v.prior;
	}
	/*/
	if(version>=1)
	{
		s >> v.subitems;
	}
	/*/
	return s;
}

template <class S> S& operator >> (S& s, geometry3::GeometrySource& v) 
{
	uint32_t version = 2;
	s >> version;

	if (version >= 0)
	{
		s >> v.faces;

		// Геометрия
		s >> v.faces;		// или faces
		s >> v.lines;		// или lines
		s >> v.verts;
		s >> v.norms;
		int uvcount = TV_MAX_CHANNEL;
		s >> uvcount;
		for(int i=0; i<uvcount; i++)
			s >> v.tverts[i];
		s >> v.colors;
	}
	if (version >= 1)
	{
		s >> v.tangents;
		s >> v.binormals;
	}
	if (version >= 2)
	{
		s >> v.textureindex;
	}
	
	return s;
}



template <class S>
S& operator >>(S& s, superficial3::lSuperficial::Item& v ) 
{
	s >> *(geometry3::GeometryItem*)&v;

	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.sdp_flags;
		s >> v.geometrysourceindex;
	}
	return s;
}

template <class S>
S& operator >>(S& s, superficial3::lSuperficial& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.geoms;
//		s >> v.test;
	}
	return s;
}

template <class S>
S& operator >>(S& s, superficial3::lSemanticSquare& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.pSquares;
		s >> v.geomsindecies;
	}
	return s;
}

template <class S>
S& operator >>(S& s, superficial3::lSuperficialFile::Level& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.level;
		s >> v.sqmin>>v.sqmax;
ed::vector<superficial3::lSuperficial> squarepool;
s >> squarepool;
		s >> v.pSquares;
	}
	return s;
}

template <class S>
S& operator >>(S& s, superficial3::lSuperficialFile::SemanticTest& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.semantic;
		s >> v.sqmin>>v.sqmax;
		s >> v.squarepool;
		s >> v.pSquares;
	}
	return s;
}


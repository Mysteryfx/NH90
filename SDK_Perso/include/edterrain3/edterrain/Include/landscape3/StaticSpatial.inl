template <class S> S& operator >> (S& s, scene3::StaticSpatial& v) 
{
	uint32_t version = 0;
	s >> version;
	
	if (version >= 0)
	{
		s >> v.nodes;
		s >> v.ChildNodes;
		s >> v.ObjectIDs;
	}

	return s;
}


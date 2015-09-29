template <class S> S& operator >> (S& s, scene3::DistrictInstance& v) 
{
	uint32_t version = 1;
	s >> version;
	
	if (version >= 0)
	{
		s >> v.districttype;
		s >> v.box;
		s >> v.object_pos;
		s >> v.object_type;
		s >> v.object_flag;
	}
	if (version >= 1)
	{
		s >> v.surfaceY_xz;
	}

	return s;
}


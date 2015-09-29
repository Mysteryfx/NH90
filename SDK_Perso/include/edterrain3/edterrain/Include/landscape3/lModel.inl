template <class S>
S& operator >>(S& s, models3::lModel& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.geomsource;
		s >> v.geoms;
		s >> v.geomsource_inds;
	}
	return s;
}

template <class S>
S& operator >>(S& s, models3::ModelKey& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.name;
		s >> v.model;
	}
	return s;
}


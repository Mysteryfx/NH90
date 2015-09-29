template <class S>
S& operator >>(S& s, E3::Vector& v ) 
{
	s >> v.x;
	s >> v.y;
	s >> v.z;
	return s;
}

template <class S>
S& operator >>(S& s, cPosition& v ) 
{
	s >> v.x;
	s >> v.y;
	s >> v.z;
	s >> v.p;
	return s;
}



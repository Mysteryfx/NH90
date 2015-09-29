
template <class S>
S& operator >>(S& s, scene3::BlockBone& v)
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> v.modelname;
		// 
		s >> v.x;
		s >> v.y;
		s >> v.uv;
		s >> v.bBox;
	}
	return s;
}

template <class S>
S& operator >>(S& s, scene3::lBlockFile::LODGeometry& g)
{
	uint32_t version = 7;
	s >> version;
	if(version>=0)
	{
		s >> g.pos;
		s >> g.normal;
		s >> g.uv1;
		s >> g.uv2;
		s >> g.bone;
		s >> g.vertNumber;
		s >> g.scale;
		s >> g.yrotate;
		s >> g.indecies;
		s >> g.startFace;
		s >> g.lastFace;
	}
	if(version>=1)
	{
		s >> g.specFactor;
		s >> g.specPower;
		s >> g.specPowerAmount;
	}
	if(version>=2)
	{
		s >> g.weights;
	}
	if(version>=3)
	{
		s >> g.typedirection;
		s >> g.direction;
	}
	if(version>=4)
	{
		s >> g.material_index;
		s >> g.isValid;
	}
	if(version >= 5)
	{
		s >> g.color;
	}
	if(version >= 6)
	{
		s >> g.height;
	}
	if(version >= 7)
	{
		s >> g.selfIlluminationAmount;
		s >> g.reflectionAmount;
	}
	return s;
}
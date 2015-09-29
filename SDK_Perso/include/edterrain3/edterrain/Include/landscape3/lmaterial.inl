#ifndef EDTERRAIN_LANDSCAPE3_LMATERIAL_INL
#define EDTERRAIN_LANDSCAPE3_LMATERIAL_INL


template <class S>
S& operator >>(S& s, material3::Material::Parameter& v ) 
{
	uint32_t version = 0;
	s >> version;
	if(version>=0)
	{
		s >> *(int32_t*)&v.type;
		s >> v.text;
	}
	return s;
}

template <class S>
S& operator >>(S& s, material3::Material& v ) 
{
	uint32_t version = 1;
	s >> version;

	if(version==0)
	{
		// псевдоним материала
		s >> v.matalias;
		s >> v.semantic;
		ed::vector<material3::Material::Parameter> params;
		s >> params;
		if(s.getMode()==io::Stream::READ)
		{
			for(int i=0; i<(int)params.size(); i++)
			{
				ed::string name;
				name += char(i+'0');
				v.params[name] = params[i];
			}
		}
	}
	if(version>=1)
	{
		s >> v.matalias;
		s >> v.semantic;
		s >> v.params;
	}
	/*/
	if(version>=2)
	{
		s >> v.texturearray;
	}
	/*/
	
	return s;
}


#endif // EDTERRAIN_LANDSCAPE3_LMATERIAL_INL

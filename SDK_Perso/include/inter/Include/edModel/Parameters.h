#ifndef ED_MODEL_PARAMETERS_H
#define ED_MODEL_PARAMETERS_H

#include <ed/vector.h>
#include <cstring>

namespace model
{

struct Parameter
{
	union
	{
		double d;
		int i;
		void* p;
	};

	Parameter()
	{
		memset(this,0x0,sizeof(*this));
	}
	Parameter(double d):d(d) {}
	Parameter(float f):d((double)f) {}
	Parameter(int i):i(i) {}
	Parameter(unsigned int ui):i((int)ui) {}
	Parameter(void* p):p(p) {}

	inline operator double()const
	{
		return d;
	}
	inline operator float()const
	{
		return (float)d;
	}
	inline operator int()const
	{
		return i;
	}
	inline operator unsigned int()const
	{
		return (unsigned int)i;
	}

	inline operator double&()
	{
		return d;
	}
	inline operator int&()
	{
		return i;
	}
	inline operator unsigned int&()
	{
		return (unsigned int&)i;
	}

	template<typename _T> inline operator _T*()const
	{
		return (_T*)p;
	}
};

struct Parameters : public ed::vector<Parameter>
{
	inline void add(size_type _Pos,const Parameter& param)
	{
		if(_Pos >= size()) resize(_Pos+1);
		ed::vector<Parameter>::operator [](_Pos)=param;
	}
	/*	Parameter& operator[](unsigned int i){return at(i);}
		const Parameter& operator[](unsigned int i)const{return at(i);}*/
};
}
#endif

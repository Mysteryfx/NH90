/*
//	Файл:			cAlgorithm.h
//  Подсистема:		Common (interface)
//
//	Файл содержит описание алгоритмов которые
//	могут быть полезны при разработке
*/
#pragma once

const float PI = 3.14159f;
const float _2PI = 2 * PI;
const float PI_2 = 0.5f * PI;
const float PI_4 = 0.25f * PI;

#define MIN(a,b)		( ((a) < (b)) ? (a) : (b) )
#define MAX(a,b)		( ((a) > (b)) ? (a) : (b) )
#define MIN_MAX(a,b,c)	( MIN( (MAX((a),(b))), (c)) )

#define ABS(a)          ( ((a) < 0.) ? (-(a)) : (a) ) 
#define SIGN(a) (a > 0 ? 1 : (a < 0 ? -1 : 0))

template<typename T>
inline T TO_RAD(const T & a)			{ return a * 0.01745329f; }
template<typename T>
inline T TO_GRAD(const T & a)			{ return a * 57.295779513f; }
template<typename T>
inline T GRAD_360_to_180(const T & a)	{ return (a <= 180.f) ? a : (a - 360.0f); }
template<typename T>
inline T GRAD_180_to_360(const T & a)	{ return (a >=  0.0f) ? a : (a + 360.0f); }

template<typename T>
inline T RAD_TO_2PI(const T & a)
{
	if(a == _2PI)
	{
		return a;
	}
	else
	{
		T result = fmod(float(a), _2PI);
		if(a < T(0.0f))
		{
			result += _2PI;
		}
		return result;
	}
}

template<typename T>
inline T RAD_TO_PI(const T & a)
{
	if(a == PI)
	{
		return a;
	}
	else
	{
		T result = fmod(float(a), _2PI);
		if(ABS(result) > PI)
		{
			result -= SIGN(result) * _2PI;
		}
		return result;
	}
}


template<typename T>
inline T GRAD_TO_180(const T & a)
{
	if(a == 180)
	{
		return a;
	}
	else
	{
		T result = fmod(float(a), 360);
		if(ABS(result) > 180)
		{
			result -= SIGN(result) * 360;
		}
		return result;
	}
}

template<typename T>
inline T GRAD_TO_360(const T & a)
{
	if(a == 360)
	{
		return a;
	}
	else
	{
		T result = fmod(float(a), 360);
		if(a < T(0.0f))
		{
			result += 360;
		}
		return result;
	}
}


template<typename T> void GetYawPitchRollFromMatrix(const T & pos,float & yaw,float & pitch,float & roll)
{
	const float threshold = 0.0001f;
	float temp = sqrt(pos.x.x*pos.x.x + pos.x.z*pos.x.z);
	if(temp < threshold)
	   temp = threshold;

	yaw   = atan2(-pos.x.z, pos.x.x);
	if(yaw < 0.0f) 
	   yaw = Pi_2 + yaw;
	pitch = atan  (pos.x.y /temp);
	roll  = atan2(-pos.z.y, pos.y.y);
}

#define NAME_CONST(c) #c, c

#define SYMBOL_STR(c) #c


template<typename T>
class range_to_range
{
public:
	typedef struct range_type 
	{
		range_type()
			: start_(T(0.0)), end_(T(1.0))
		{}
		T start_;
		T end_;
	};

	void set(const range_type& r1, const range_type& r2) 
	{
		assert(r1.start_ != r1.end_);
		assert(r2.start_ != r2.end_);

		if(r1.start_ == r1.end_)
		{
			r1_.start_ = T(.0);
			r1_.end_ = T(1.0);
		}
		else r1_ = r1;

		if(r2.start_ == r2.end_)
		{
			r2_.start_ = T(.0);
			r2_.end_ = T(1.0);
		}
		else r2_ = r2;
	}

	T to_limit_1(const T val_in) const 
	{
		const T f = __max(0.0,__min((val_in - r1_.start_)/(r1_.end_-r1_.start_),1.0));
		return r1_.start_ + f * (r1_.end_-r1_.start_);
	}

	T to_limit_2(const T val_in) const 
	{
		const T f = __max(0.0,__min((val_in - r2_.start_)/(r2_.end_-r2_.start_),1.0));
		return r2_.start_ + f * (r2_.end_-r2_.start_);
	}

	T to_range_1(const T val_in) const 
	{
		const T f = (val_in - r2_.start_)/(r2_.end_-r2_.start_);
		return r1_.start_ + f * (r1_.end_-r1_.start_);
	}

	T to_range_2(const T val_in) const 
	{
		const T f = (val_in - r1_.start_)/(r1_.end_-r1_.start_);
		return r2_.start_ + f * (r2_.end_-r2_.start_);
	}

	T to_range_1_s(const T val_in) const 
	{
		const T f = __max(0.0,__min((val_in - r2_.start_)/(r2_.end_-r2_.start_),1.0));
		return r1_.start_ + f * (r1_.end_-r1_.start_);
	}

	T to_range_2_s(const T val_in) const 
	{
		const T f = __max(0.0,__min((val_in - r1_.start_)/(r1_.end_-r1_.start_),1.0));
		return r2_.start_ + f * (r2_.end_-r2_.start_);
	}

	range_type r1_;
	range_type r2_;
};

typedef range_to_range<float> r_to_r_f;

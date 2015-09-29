//                    file wFloat.h
#ifndef __wFloat_h__
#define __wFloat_h__

#include "WorldGeneral.h"
#include "Serializer\Serializer.h"
#include "cPosition.h"
#include "quaternion.h"
#include "wTime.h"

template <class T>
class wLineInterpolation 
{
public:
    
	wLineInterpolation() 
	{
		initialize = false;
		tPrev = tNext = wTime::GetModelTime();
	}
	
	virtual ~wLineInterpolation() 
	{
		initialize = false;
		tPrev = tNext = 0;
	}
		
	virtual void Set(const T & current, wModelTime tCurrent)
	{
		initialize = true;
		tPrev = tNext = tCurrent;
		prev = next = current;
	}
    bool      Initialized() const { return initialize; }
	virtual void NextStep(const T & _next, wModelTime _tNext);
	virtual T GetValue(const wModelTime t) const;

	const T & GetPrevValue() const { return prev; }
	const T & GetNextValue() const { return next; }
	wModelTime GetPrevTime() const { return tPrev; }
	wModelTime GetNextTime() const { return tNext; }

protected:

	T			prev;		
	T			next;		
	wModelTime	tPrev;	
	wModelTime	tNext;
	bool		initialize;
};

template <class T>
void wLineInterpolation<T>::NextStep(const T & _next, wModelTime _tNext)
{
	if(!initialize)
	{
		assert(false);
	}
	tPrev = tNext;
	tNext = _tNext;
	prev = next;
	next = _next;
}

template <class T>
T wLineInterpolation<T>::GetValue(const wModelTime t) const
{
	if(!initialize)
	{
		assert(false);
	}
	if(tNext - tPrev == 0 || prev == next) return prev;
	else 
	{
		T current = (next*(t - tPrev) - prev*(t - tNext))/(tNext - tPrev);
		return current;
	}
}

class WORLDGENERAL_API wFloat : public  wLineInterpolation<float>
{
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wFloat& data);
};

class WORLDGENERAL_API wDouble : public  wLineInterpolation<double>
{
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wDouble& data);
};

class WORLDGENERAL_API wPosition : public wLineInterpolation<cPosition>
{
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wPosition& data);
};

class WORLDGENERAL_API wQuaternion : public wLineInterpolation<Quaternion>
{
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wQuaternion& data);

public:

	Quaternion GetValue(const wModelTime t);
};

class WORLDGENERAL_API wVector : public wLineInterpolation<dVector>
{
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wVector& data);
};

class WORLDGENERAL_API wVector_f : public wLineInterpolation<cVector>
{
    WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wVector_f& data);
};

#endif


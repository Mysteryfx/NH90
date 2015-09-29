#ifndef DynamicParams_H
#define DynamicParams_H

#include "Resourcer\HeapVector.h"
#include "Serializer\Serializer.h"
#include "floatDebug.h"

namespace Graphics
{

// динамические параметры - характеризуют характер отрисовки
// и наложения материалов для данного конкретного случая
struct DynamicParam
{
	union 
	{
		float f;
		void *p;
		int i;
	};

	DynamicParam():p(NULL){}

	inline DynamicParam &operator =(float v) {CheckValid(v);f = v; return *this;}
	inline DynamicParam &operator +=(float v) {CheckValid(v);f += v; return *this;}
	inline DynamicParam &operator -=(float v) {CheckValid(v);f -= v; return *this;}
	inline DynamicParam &operator *=(float v) {CheckValid(v);f *= v; return *this;}
	inline DynamicParam &operator /=(float v) {CheckValid(v);f /= v; return *this;}
	inline operator float() const {return f;}
};

inline Serializer &operator <<(Serializer& serializer, DynamicParam& p) {return serializer << p.f;}

typedef HeapVector<DynamicParam> DParamList;

}

#endif // DynamicParams_H

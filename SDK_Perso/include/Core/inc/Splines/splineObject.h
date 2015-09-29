#pragma once
#include "ed/vector.h"
#include "Splines/spline.h"
#include "ed_log.h"
#include "assert.h"

class ISplineObject
{
public:
	virtual ~ISplineObject() {}
	//получить значение сплайна
	virtual void	getValue(float t, float *out) const = 0;
	//получить длину сплайна, разбивая каждый сплайн на step интервалов
	//чем выше step, тем точнее и медленнее расчет
	virtual float	getLength(float t, unsigned segments=30) const = 0;
};

//Представляет массив сплайнов как один объект для dims изменерий, таким образом что касательные 
template <int dims> class SplineObject: public ISplineObject
{
public:
	enum Type
	{
		CatmullRom,
	};

	typedef struct ElementType_
	{ 
		float component[dims]; 
		inline float length()
		{
			float len = 0;
			for(unsigned i=0; i<dims; ++i)
				len += component[i]*component[i];
			return sqrtf(len);
		}

		inline ElementType_ operator-(const ElementType_ &r) const
		{
			ElementType_ res = *this;
			for(unsigned i=0; i<dims; ++i)
				res.component[i] -= r.component[i];
			return res;
		}

		inline ElementType_ operator-() const
		{
			ElementType_ res;
			for(unsigned i=0; i<dims; ++i)
				res.component[i] = -component[i];
			return res;
		}
		
	} ElementType; //заворачиваем массив в структуру, чтобы у std вектора работало копирование и создание

	typedef ed::vector< ElementType > ElementVector;

public:
			SplineObject();
			~SplineObject();
	//Создать сплайн. 
	//По умолчанию тип сплайна - Катмул-Ром, касательные задаются как соседние точки. 
	//Чтобы не париться c касательными можно юзать generateCatmullRomTangents()
	void	set(const ElementVector &knots, const ElementVector &tangents, bool separateTangents = false, Type *types=nullptr);
	//получить значение сплайна, параметр задается от 0 до 1 для всего сплайна
	void	getValue(float t, float *out) const;
	//получить значение сплайна при постоянно скорости движения, параметр задается от 0 до 1 для всего сплайна.
	//перед использованием функции нужно один раз вызвать linearize(..)
	void	getLinearValue(float t, float *out) const;
	//получить длину сплайна, разбивая каждый сплайн на step интервалов
	//чем выше step, тем точнее и медленнее расчет
	float	getLength(float t, unsigned segments=30) const;
	//формирует таблицу соответствия параметров для обычного и равномерного движения, после чего можно юзать функцию getLinearValue()
	//Таблица будет состоять из segments элементов,
	void	linearize(unsigned segments);
	//возвращает параметр сплайна для движения по нему с постоянной скоростью
	float	getLinearParam(float t);

	//Генерирует касательные для Катмул-Ром сплайна, полагая что весь сплайн непрерывный, минимум 2 точки.
	//Если close = true совмещает касательные в крайних точках чтобы сплайн был замкнутым, 
	//в этом случае первая и последняя точка в knots должны совпадать, минимум 3 точки.
	static void generateCatmullRomTangents(const ElementVector &knots, ElementVector &tangents, bool close = false);

private:
	Type	splineTypes[dims];
	ed::vector<ISpline*> splines[dims];
	ElementVector		knots;
	ElementVector		tangents;
	int					size;
	//таблица паарметров для инеаризации скорости движения по сплайну
	ed::vector<float>	param;
	ed::vector<float>	paramLinear;

	void	clear();
	
	void	getSplineParam(float t, float &splineParam, int &id) const;
	ISpline *createSpline(unsigned dim);
	void	addSegment(const ElementType &k0, const ElementType &t0, const ElementType &k1, const ElementType &t1);
};

template <int dims>
float SplineObject<dims>::getLinearParam( float t )
{
	if(t<=0 || t>1)
		return t;
	//ищем ближайшие табличные значения в массиве линеаризованного параметра
	float id0=paramLinear.size()-2;
	for(unsigned i=0; i<paramLinear.size(); ++i)
	{
		if(t<paramLinear[i])
		{
			id0=i-1;
			break;
		}
	}
	float len = paramLinear[id0+1]-paramLinear[id0];
	float t2 = (t-paramLinear[id0])/len;//относительно отрезка

	return param[id0] + t2*(param[id0+1]-param[id0]);//пересчитываем в нелинеаризованый параметр
}

template <int dims>
void SplineObject<dims>::linearize( unsigned segments )
{
	const unsigned maxSegments = 200;	
	float lengthSum = this->getLength(1.f, maxSegments);	

	param.clear();
	paramLinear.clear();
	for(unsigned i=0; i<segments; ++i)
	{
		float t = (float)i/segments;
		param.push_back(t);
		paramLinear.push_back(this->getLength(t, t*maxSegments)/lengthSum);
	}
}

template <int dims>
void SplineObject<dims>::addSegment( const ElementType &k0, const ElementType &t0, const ElementType &k1, const ElementType &t1 )
{
	for(unsigned dim=0; dim<dims; ++dim)
	{
		ISpline *s = createSpline(dim);
		s->set(k0.component[dim], t0.component[dim], k1.component[dim], t1.component[dim]);
		splines[dim].push_back(s);
	}
}

template <int dims>
void SplineObject<dims>::generateCatmullRomTangents( const ElementVector &kn, ElementVector &tn, bool close )
{
	int size = kn.size();
	if(size<2)
	{
		assert(0);
		return;
	}
	tn.clear();

	if(size>2)
	{
		int lastId = size-1;
		if(close)
			tn.push_back(kn[lastId-1]);		
		else
			tn.push_back(kn[0]);		

		tn.push_back(kn[2]);
	
		for (int i = 1; i < lastId-1; i++)
		{
			tn.push_back(kn[i-1]);
			tn.push_back(kn[i+2]);
		}		
		tn.push_back(kn[lastId-2]);
		if(close)
			tn.push_back(kn[1]);			
		else
			tn.push_back(kn[lastId]);
	}
	else
	{
		tn.push_back(kn[0]);
		tn.push_back(kn[1]);
	}	
}

template <int dims>
float SplineObject<dims>::getLength( float paramMax, unsigned segments ) const
{
	if(!size)
		return 0;
	if(segments<1)
		segments=1;
	ed::vector<ElementType> kn;
	kn.resize(segments+1);
	//считаем позиции в точках
	for(unsigned i=0; i<=segments; ++i)
	{
		getValue(((float)i/segments)*paramMax, kn[i].component);
	}
	//считаем длины отрезков и суммируем
	float length=0;
	ElementType vLen;
	for(unsigned i=0; i<segments; ++i)
	{		
		vLen = kn[i+1] - kn[i];//сегмент		
		length += vLen.length();
	}
	return length;
}

template <int dims>
SplineObject<dims>::~SplineObject()
{
	clear();
}

template <int dims>
void SplineObject<dims>::getSplineParam( float t, float &splineParam, int &id ) const
{
	t = t*size;
	id = ceil(t)-1;
	if(id>size-1)
		id = size-1;
	if(id<0)
		id = 0;
	splineParam = t - id;
}

template <int dims>
void SplineObject<dims>::getValue( float t, float *out ) const
{
	int id;
	float param;
	getSplineParam(t, param, id);

	for(unsigned i=0; i<dims; ++i)
		out[i] = splines[i][id]->getValue(param);

	//ED_INFO("id:%d\tt:%.2f\t[%.2f;%.2f;%.2f]\n", id, param, out[0], out[1], out[2]);
}


template <int dims>
void SplineObject<dims>::getLinearValue( float t, float *out ) const
{
	getValue(getLinearParam(t), out);
}


template <int dims>
ISpline *SplineObject<dims>::createSpline( unsigned dim )
{
	switch (splineTypes[dim])
	{
	case CatmullRom: return new CatmullRomSpline;
	default: 
		assert(!"unsupported spline type");		
	}
	return nullptr;
}

template <int dims>
SplineObject<dims>::SplineObject(): size(0)
{
	for(unsigned i=0; i<dims; ++i)
		splineTypes[i] = CatmullRom;
}

template <int dims>
void SplineObject<dims>::clear()
{
	for(unsigned i=0; i<dims; ++i)
	{
		for(auto s: splines[i])
			delete s;
		splines[i].clear();
	}
	size = 0;
}

template <int dims>
void SplineObject<dims>::set( const ElementVector &kn, const ElementVector &tang, bool separateTangents, Type *types )
{
	knots = kn;
	tangents = tang;
	if(types)
		memcpy(splineTypes, types, dims*sizeof(Type));

	clear();
	//по всем сегментам
	unsigned splineId = 0;
	if(separateTangents)
	{
		for(unsigned i=0; i<kn.size()-1; ++i)
		{
			const auto &k0 = knots[i];
			const auto &k1 = knots[i+1];
			const auto &t0 = tangents[2*i];
			const auto &t1 = tangents[2*i+1];
			//по всем измерениям добавляем сплайны
			addSegment(k0,t0, k1, t1);		
		}
	}
	else
	{
		for(unsigned i=0; i<kn.size()-1; ++i)
		{
			const auto &k0 = knots[i];
			const auto &k1 = knots[i+1];
			const auto &t0 = tangents[i];
			const auto &t1 = tangents[i+1];
			//по всем измерениям добавляем сплайны
			addSegment(k0,t0, k1, -t1);
		}
	}
	size = splines[0].size();
}

typedef	SplineObject<2> Spline2d;
typedef	SplineObject<3> Spline3d;
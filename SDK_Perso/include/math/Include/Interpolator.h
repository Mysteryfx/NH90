#pragma once

#include <ed/vector.h>

#include <stdarg.h>

// точка в N-мерном пространстве
// для использования в интреполяторе
template <int N>
class InterPoint
{
public:
	InterPoint()
	{
		for( int i=1; i<N; i++)
			x[i] = 0;
	}
	InterPoint(double first, ...)
	{
		va_list argList; va_start(argList, first);
		x[0] = first;
		for( int i=1; i<N; i++)
		{
			x[i] = va_arg( argList, double);
		}
		va_end(argList);
	}
	double x[N];	// точка в N-мерном пространстве
};


// Класс для интерполяции значения, заданного на множестве точек в N-мерном пространстве
template <int N>
class Interpolator
{
protected:
	// содержит дополнительную информацию о точке
	class InterPointValue : public InterPoint<N>
	{
	public:
		double value; // значение функции в данной точке
		double k;	// предрассчитанный коэффициент
	};

	// список точек, в которых значение функции задано
	typedef ed::vector< InterPointValue >  pointList;
	pointList points;

	// возвращает квадрат расстояния между двумя точками
	 double Distance2(const InterPoint<N>& p1, const InterPoint<N>& p2) const;

	// возвращает 0 во всех точках кроме n
	 double G(int n, const InterPoint<N>& p) const;

public:
	// Сбросить
	 void Reset();

	// Расстояние до ближайшей точки
	 double TestPointDistance(const InterPoint<N>& p, double& valuenear);
	
	// добавить точку со значением функции
	 void AddPoint(const InterPoint<N>& p, double value);

	// предрассчет коэффициентов (вызвать после заполнения всех точек)
	 void Prepare();

	// посчитать интерполированное значение
	 double Value(const InterPoint<N>& p) const;
};

// Interpolator class

// возвращает квадрат расстояния между двумя точками
template <int N>
 double Interpolator<N>::Distance2(const InterPoint<N>& p1, const InterPoint<N>& p2) const
{
	double res = 0.;
	for(int i=0; i<N; i++)
	{
		double d = p1.x[i] - p2.x[i];
		res += d*d;
	}
	return res;
}

// возвращает 0 во всех точках кроме n
template <int N>
 double Interpolator<N>::G(int n, const InterPoint<N>& p) const
{
	double res = 1.;
	for(int i=0; i<points.size(); i++)
	{
		if(i == n)
		{
			double d = Distance2(points[i], p);
//			if( d>0.2) return 0;
			d = 1./(1. + d);
			res *= pow(d, (int)((points.size()-1)*2));
		}
		else
		{
			res *= sqrt(Distance2(points[i], p));
		}
	}
	return res;
}

// Расстояние до ближайшей точки
template <int N>
 double Interpolator<N>::TestPointDistance(const InterPoint<N>& p, double& valuenear)
{
	double dist2 = 1e100;
	for(int i=0; i<points.size(); i++)
	{
		double cur = Distance2(p, points[i]);
		if(dist2<cur) continue;
		dist2 = cur;
		valuenear = points[i].value;
	}
	return sqrt(dist2);
}

// Сбросить
template <int N>
 void Interpolator<N>::Reset()
{
	points.clear();
}

// добавить точку со значением функции
template <int N>
 void Interpolator<N>::AddPoint(const InterPoint<N>& p, double value)
{
	int i = points.size();
	if(i >= points.capacity()) points.reserve(i*2+1);

	InterPointValue lp;
	for(int j=0; j<N; j++) lp.x[j] = p.x[j];
	lp.value = value;
	points.push_back(lp);
}

// предрассчет коэффициентов (вызвать после заполнения всех точек)
template <int N>
 void Interpolator<N>::Prepare()
{
	for(int i=0; i<points.size(); i++)
	{
		points[i].k = points[i].value / G(i, points[i]);
	}
}

// посчитать интерполированное значение
template <int N>
 double Interpolator<N>::Value(const InterPoint<N>& p) const
{
	// пусть k - количество заданных точек
	// тогда результат - сумма функций Gk, (нормированных и умноженных на pk.value)
	// функция Gi возвращает 0 во всех точках кроме i

	double sum = 0.;
	for(int i=0; i<points.size(); i++)
	{
		sum += G(i, p) * points[i].k;
	}
	return sum;
}

#ifndef __USERINTERFACE_POINT_H__
#define __USERINTERFACE_POINT_H__

template <class T>
class  TPoint
{
public:
	T					x; 
	T					y;
public:
	explicit TPoint(T in_value = 0) : x(in_value), y(in_value){}	
	TPoint(T in_x, T in_y) : x(in_x), y(in_y)		{}
	TPoint(const TPoint& p) : x(p.x), y(p.y)			{}

	TPoint&					operator=(const TPoint& p)
	{
		if (this != &p)	
		{
			x = p.x;
			y = p.y;
		}
		return *this;
	}
	TPoint&					operator+=(const TPoint& p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
	TPoint&					operator-=(const TPoint& p)
	{
		x -= p.x; 
		y -= p.y;
		return *this;
	}

	TPoint&	 operator * (T k)
	{
		x *= k; 
		y *= k;
		return *this; 
	}

	TPoint&	 operator / (T k)
	{
		x /= k; 
		y /= k;
		return *this; 
	}

	double operator * (const TPoint &p)
	{
		return x * p.x + y * p.y; 
	}
	double length()
	{
		return sqrt(x * x + y * y);
	}

};

typedef TPoint<float> Point;
typedef TPoint<int> IPoint;

template <class T>
inline TPoint<T> operator+(const TPoint<T>& p1, 
													  const TPoint<T>& p2)
{
	return TPoint<T>(p1.x + p2.x, p1.y + p2.y);
}
template <class T>
inline TPoint<T> operator-(const TPoint<T>& p1, 
													  const TPoint<T>& p2)
{
	return TPoint<T>(p1.x - p2.x, p1.y - p2.y);
}
template <class T>
inline TPoint<T> operator/(const TPoint<T>& p1, 
													  const TPoint<T>& p2)
{
	return TPoint<T>(p1.x / p2.x, p1.y / p2.y);
}
template <class T>
inline bool operator == (const TPoint<T>& inFirstPoint, 
 														   const TPoint<T>& inSecondPoint)
{
	return (inFirstPoint.x == inSecondPoint.x && inFirstPoint.y == inSecondPoint.y);
}
template <class T>
inline bool operator != (const TPoint<T>& inFirstPoint, 
 														   const TPoint<T>& inSecondPoint)
{
	return (inFirstPoint.x != inSecondPoint.x || inFirstPoint.y != inSecondPoint.y);
}
template <class T>
inline double getDistance(const TPoint<T>& inStart,
																  const TPoint<T>& inEnd)
{
	return (inStart - inEnd).length();
}


#endif // __USERINTERFACE_POINT_H__

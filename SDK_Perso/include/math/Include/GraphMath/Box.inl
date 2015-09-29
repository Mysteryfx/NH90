//	Box.inl *******************************************************************
#pragma once

inline	Box::Box()
{
	max.set(-1e20f,-1e20f,-1e20f);
	min.set( 1e20f, 1e20f, 1e20f);
}

inline	Box::Box(const Vector3& _min, const Vector3& _max)
{
	min = _min;
	max = _max;
}

inline	Box::Box(float x, float y, float z, float X, float Y, float Z)
{
	min.set(x,y,z);
	max.set(X,Y,Z);
}

inline	Box::Box(const Vector3& center, float size)	// Куб.
{
	size/= 2;
	size =(float)fabs(size);

	Vector3 sz(size, size, size);

	min = center-sz;
	max = center+sz;
}

inline bool Box::operator ==(const Box& arg) const
{
	return (min == arg.min) && (max == arg.max);
}

inline bool Box::operator !=(const Box& arg) const
{
	return (min != arg.min) || (max != arg.max);
}

// расстояние между min и max
inline float Box::diameter() const
{
	return (max-min).length();
}

inline	float Box::radius() const	// радиус описанной сферы.
{
	if(!isValid()) return 0.0f;

	float m = min.length();
	float M = max.length();

	if	(m < M) return M;
	else		return m;
}

inline	Vector3 Box::corner(int i) const	// угол по номеру (от 0 до 7).
{
	Vector3 res;
	res.x = ((i&4) == 0) ? min.x : max.x;
	res.y = ((i&2) == 0) ? min.y : max.y;
	res.z = ((i&1) == 0) ? min.z : max.z;
	return	res;
}

inline	void Box::reset()	// восстановить начальное состояние.
{
	min.set( 1e20f, 1e20f, 1e20f);
	max.set(-1e20f,-1e20f,-1e20f);
}

inline	bool Box::intersectionXZ(const Box& arg) const
{
	Box res;
	return intersectionXZ(arg,res);
}

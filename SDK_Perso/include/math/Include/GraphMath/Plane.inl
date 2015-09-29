//	Plane.inl *****************************************************************

#pragma once

inline	Plane::Plane(const float*p)
{
n.x = p[0];
n.y = p[1];
n.z = p[2];
d   = p[3]; normalize();
}

inline	Plane::Plane(float A,float B,float C,float D)
{
n.x = A;
n.y = B;
n.z = C;
d   = D; normalize();
}

inline	Plane Plane::operator - () const
{
return	Plane(-n.x,-n.y,-n.z,-d);
}

inline	Plane& Plane::normalize(void)
{
float len = 1.f/n.length();

n*= len;
d*= len; return *this;
}

inline	bool Plane::operator ==	 (const Plane&p) const
{
return	(n == n) &&	(d == d);
}

inline	bool Plane::operator !=	 (const Plane&p) const
{
return	(n != n) ||	(d != d);
}

inline	float Plane::distanceToPoint(const Vector3&v) const
{
return (n*v + d);
}


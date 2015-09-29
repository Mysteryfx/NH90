//	Vector4.inl ***************************************************************

#define M_DELTA 0.000001f	// Error tolerance

inline	Vector4::Vector4  (float X, float Y, float Z, float W)
{
x = X; y = Y; z = Z; w = W;
}

inline	Vector4::Vector4  (const float* xyzw)
{
x = xyzw[0]; y = xyzw[1]; z = xyzw[2]; w = xyzw[3];
}

inline Vector4::operator float* ()
{
return (float*) &x;
}

inline Vector4::operator const float* () const
{
return (const float*) &x;
}

inline float& Vector4::operator[] (int i) const
{
return ((float*)&x)[i];
}

inline	Vector4  & Vector4::operator () (float X,float Y,float Z, float W) const
{
((Vector4*)this)->x = X;
((Vector4*)this)->y = Y;
((Vector4*)this)->z = Z;
((Vector4*)this)->w = W;

return *((Vector4*)this);
}

inline	Vector4   Vector4::operator +	() const	
{
return	Vector4  (x,y,z,w);
}

inline	Vector4   Vector4::operator -	() const	
{
return	Vector4  (-x,-y,-z,-w);
}

inline	bool Vector4::operator == (const Vector4  &v) const
{
return (x == v.x)&&(y == v.y)&&(z == v.z)&&(w == v.w);
}

inline	bool Vector4::operator != (const Vector4  &v) const
{
return (x != v.x)||(y != v.y)||(z != v.z)||(w != v.w);
}

inline	Vector4  & Vector4::operator += (const Vector4  &v)
{
x+=v.x; y+=v.y; z+=v.z; w+=v.w;	return *this;
}

inline	Vector4  & Vector4::operator -= (const Vector4  &v)
{
x-=v.x; y-=v.y; z-=v.z; w-=v.w;	return *this;
}

inline	Vector4  & Vector4::operator *= (float r)
{
x*=r; y*=r; z*=r; w*=w;	return *this;
}

inline	Vector4  & Vector4::operator /= (float r)
{
if	(r > M_DELTA) {r = 1.f/r; x*=r; y*=r; z*=r; w*=r;} return *this;
}

inline	float Vector4::operator * (const Vector4  &v) const
{
return x*v.x+y*v.y+z*v.z+w*v.w;
}

inline	Vector4   Vector4::operator +	(const Vector4  &v) const
{
return	Vector4  ((x+v.x),(y+v.y),(z+v.z),(w+v.w));
}

inline	Vector4   Vector4::operator -	(const Vector4  &v) const
{
return	Vector4  ((x-v.x),(y-v.y),(z-v.z),(w-v.w));
}

inline	Vector4   Vector4::operator * (float f) const
{
return Vector4  (f*x,f*y,f*z,f*w);
}

inline	Vector4   Vector4::operator / (float f) const
{
Vector4   res;

if	(f > M_DELTA)
	{
	f = 1.f/f;

	res.x = f*x;
	res.y = f*y;
	res.z = f*z;
	res.w = f*w; return res;
	}
else{
    return Vector4  (0.f,0.f,0.f,1.f);
    }
}

inline Vector4   operator * (float f, const class Vector4  & v)
{
return Vector4  (f*v.x,f*v.y,f*v.z,f*v.w);
}

inline	Vector4& Vector4::add(const Vector4& v1,const Vector4& v2)
{
	Vector4 tmp;
	tmp.x = v1.x + v2.x;
	tmp.y = v1.y + v2.y;
	tmp.z = v1.z + v2.z;
	tmp.w = v1.w + v2.w;
	*this = tmp;
	return *this;
}

inline	float Vector4::dot(const Vector4& v)
{
return (x * v.x + y * v.y + z * v.z + w * v.w);
}

inline	float Vector4::length(void)
{
	return sqrtf( x*x + y*y + z*z + w*w);
}

inline	float Vector4::lengthSq(void)
{
return x*x + y*y + z*z + w*w;
}

inline	Vector4& Vector4::lerp(const Vector4& v1,const Vector4& v2,float s)
{
	Vector4 tmp;
	tmp.x = v1.x + s * (v2.x - v1.x);
	tmp.y = v1.y + s * (v2.y - v1.y);
	tmp.z = v1.z + s * (v2.z - v1.z);
	tmp.w = v1.w + s * (v2.w - v1.w);
	*this = tmp;
	return *this;
}

inline	Vector4& Vector4::maximize(const Vector4& v1,const Vector4& v2)
{
	Vector4 tmp;
	tmp.x = std::max(v1.x, v2.x);
	tmp.y = std::max(v1.y, v2.y);
	tmp.z = std::max(v1.z, v2.z);
	tmp.w = std::max(v1.w, v2.w);
	*this = tmp;
	return *this;
}

inline	Vector4& Vector4::minimize(const Vector4& v1,const Vector4& v2)
{
	Vector4 tmp;
	tmp.x = std::min(v1.x, v2.x);
	tmp.y = std::min(v1.y, v2.y);
	tmp.z = std::min(v1.z, v2.z);
	tmp.w = std::min(v1.w, v2.w);
	*this = tmp;
	return *this;
}

inline	Vector4& Vector4::normalize(void)
{
	auto norm = length();
	if (norm > M_DELTA){
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	}
	return *this;
}

inline	Vector4& Vector4::scale(const Vector4& v,float s)
{
	Vector4 tmp;
	tmp.x = v.x * s;
	tmp.y = v.y * s;
	tmp.z = v.z * s;
	tmp.w = v.w * s;
	*this = tmp;
	return *this;
}

inline	Vector4& Vector4::subtract(const Vector4& v1,const Vector4& v2)
{
	Vector4 tmp;
	tmp.x = v1.x - v2.x;
	tmp.y = v1.y - v2.y;
	tmp.z = v1.z - v2.z;
	tmp.w = v1.w - v2.w;
	*this = tmp;
	return *this;
}

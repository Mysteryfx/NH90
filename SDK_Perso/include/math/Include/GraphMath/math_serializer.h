#pragma once

#include "Serializer/Serializer.h"

#include "vector.h"
#include "matrix.h"
#include "position.h"
#include "quaternion.h"
#include "plane.h"
#include "box.h"

// операторы для работы с сериализатором
inline Serializer &operator<<(Serializer &serializer, Vector3& value)
{
	return serializer << value.x << value.y << value.z;
}

inline Serializer &operator<<(Serializer &serializer, Matrix33& value)
{
	return serializer << value.x << value.y << value.z;
}

inline Serializer &operator<<(Serializer &serializer, Position3& value)
{
	return serializer << value.x << value.y << value.z << value.p;
}

inline Serializer &operator<<(Serializer &serializer, Quaternion& value)
{
	return serializer << value.v << value.w;
}

inline Serializer &operator<<(Serializer &serializer, Plane& value)
{
	return serializer << value.n << value.d;
}

inline Serializer &operator<<(Serializer &serializer, Box& value)
{
	return serializer << value.min << value.max;
}

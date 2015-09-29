#pragma once

#include "floatDebug.h"
#include "cPosition.h"
#include "position.h"
#include "Math/Vector.h"

#include <assert.h>
#include <stdint.h>

inline void CheckValid(Vector3 &p)
{
	CheckValid(p.x);
	CheckValid(p.y);
	CheckValid(p.z);
}

inline void CheckValid(E2::Vector &p)
{
	CheckValid(p.x);
	CheckValid(p.y);
}

inline void CheckValid(cPoint &p)
{
	CheckValid(p.x);
	CheckValid(p.y);
	CheckValid(p.z);
}

inline void CheckValid(dPoint &p)
{
	CheckValid(p.x);
	CheckValid(p.y);
	CheckValid(p.z);
}

inline void CheckValid(Math::Vec3d &p)
{
	CheckValid(p.x);
	CheckValid(p.y);
	CheckValid(p.z);
}

inline void CheckValid(Position3 &pos)
{
	CheckValid(pos.x);
	CheckValid(pos.y);
	CheckValid(pos.z);
	CheckValid(pos.p);
}

inline void CheckValid(cPosition &pos)
{
	CheckValid(pos.x);
	CheckValid(pos.y);
	CheckValid(pos.z);
	CheckValid(pos.p);
}

inline void CheckValidConst(const Vector3 &p)
{
	CheckValidConst(p.x);
	CheckValidConst(p.y);
	CheckValidConst(p.z);
}

inline void CheckValidConst(const E2::Vector &p)
{
	CheckValidConst(p.x);
	CheckValidConst(p.y);
}

inline void CheckValidConst(const cPoint &p)
{
	CheckValidConst(p.x);
	CheckValidConst(p.y);
	CheckValidConst(p.z);
}

inline void CheckValidConst(const dPoint &p)
{
	CheckValidConst(p.x);
	CheckValidConst(p.y);
	CheckValidConst(p.z);
}

inline void CheckValidConst(const Math::Vec3d &p)
{
	CheckValidConst(p.x);
	CheckValidConst(p.y);
	CheckValidConst(p.z);
}

inline void CheckValidConst(const Position3 &pos)
{
	CheckValidConst(pos.x);
	CheckValidConst(pos.y);
	CheckValidConst(pos.z);
	CheckValidConst(pos.p);
}

inline void CheckValidConst(const cPosition &pos)
{
	CheckValidConst(pos.x);
	CheckValidConst(pos.y);
	CheckValidConst(pos.z);
	CheckValidConst(pos.p);
}

inline bool is_finite(const cPoint& v)
{
    return is_finite(v[0]) && is_finite(v[1]) && is_finite(v[2]);
}

inline bool is_finite(const dPoint& v)
{
    return is_finite(v[0]) && is_finite(v[1]) && is_finite(v[2]);
}

inline bool is_finite(const Vector3& v)
{
    return is_finite(v.x) && is_finite(v.y) && is_finite(v.z);
}

inline bool is_finite(const Position3 &pos)
{
	return (
		is_finite(pos.x) &&
		is_finite(pos.y) &&
		is_finite(pos.z) &&
		is_finite(pos.p)
	);
}

#ifndef ED_CMATH_H
#define ED_CMATH_H

#include <cmath>

#ifdef _WINDOWS

// Vistual Studio 2010 has no std::isfinite() which is a part of C++11 standart - http://en.cppreference.com/w/cpp/numeric/math/isfinite
namespace std
{
    template <typename T> T isfinite(T x) { return _finite(x); }
}

#endif

#endif
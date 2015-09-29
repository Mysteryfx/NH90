#pragma once

#include <assert.h>
#include <stdint.h>

// float/double validation

inline bool is_finite(const float &x)
{
    uint32_t bitpattern = *(uint32_t *)&x;
    return (bitpattern & 0x7f800000) != 0x7f800000; // exponent is all 1's for INFs and NaNs
}

inline bool is_finite(const double &x)
{
    uint64_t bitpattern = *(uint64_t *)&x;
    uint32_t hibits = bitpattern >> 32;
    return (hibits & 0x7ff00000) != 0x7ff00000; // exponent is all 1's for INFs and NaNs
}

inline void CheckValid(float &v)
{
    if (!is_finite(v))
    {
        assert(!"invalid float: NAN or INF");
        v = 0.f;
    }
}

inline void CheckValid(double &v)
{
    if (!is_finite(v))
    {
        assert (!"invalid double");
        v = 0.0;
    }
}

inline void CheckValidConst(const float &v)
{
    if (!is_finite(v))
    {
        assert(!"invalid float: NAN or INF");
    }
}

inline void CheckValidConst(const double &v)
{
    if (!is_finite(v))
    {
        assert (!"invalid double");
    }
}


#ifndef OSG_UTILS_H
#define OSG_UTILS_H

/// \file Misc utility functions

#include <osg/Vec4s>
#include <osg/Matrixd>

#include <stdint.h>

namespace ed {

/**
 * powerOf2 - Returns if given integer is power of 2 or not.
 */
template<typename _T> bool powerOf2(_T x)
{
	static_assert(std::is_integral<_T>::value, "Argument must be integer.");
	return ((x != 0) && !(x & (x - 1)));
}

}

#ifndef DISABLE_OBSOLETE_OSGUTILS	// prohibit to use obsolete functions

namespace ed
{


inline void makeDXcompatibleProjection(osg::Matrixd& proj)
{
	if(proj(2,2)<0)
	{
		proj(2,2)*=-1;
		proj(2,3)*=-1;
	}
}

/// Packs 4 integers to one float.
/// \param range holds max. value of integers.
float packVec4iToFloat(const osg::Vec4s& v,unsigned int range);

/// Unpacks 4 integers from one float.
/// \param range holds max. value of integers.
osg::Vec4s unpackFloatToVec4i(float f,unsigned int range);


// to match mean of DXGI_FORMAT_R8G8B8A8_UNORM
inline uint32_t packARGB(int a, int r, int g, int b){
	return  ( ((((uint32_t)a)&0xFF)<<24 ) | 
			(  (((uint32_t)b)&0xFF)<<16 ) | 
			(  (((uint32_t)g)&0xFF)<<8 ) | 
			(  (((uint32_t)r)&0xFF)));
}

inline uint32_t packRGBA(int r, int g, int b,int a) 
{
	return packARGB(a,r,g,b);
}

inline uint32_t packColor(const osg::Vec4f & color)
{
	return packRGBA(color.r * 255,
					color.g * 255,
					color.b * 255,
					color.a * 255);
}

inline osg::Vec4f unpackColor(uint32_t rgba_)
{
		const unsigned char * bytes = (const unsigned char*)&rgba_;
		int r = bytes[0];
		int g = bytes[1];
		int b = bytes[2];
		int a = bytes[3];
		return osg::Vec4f(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
}

}

#endif //DISABLE_OBSOLETE_OSGUTILS
#endif /*UTILS_H_*/

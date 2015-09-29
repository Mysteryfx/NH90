#ifndef __USERINTERFACE_UTILITIES_H__
#define __USERINTERFACE_UTILITIES_H__	1

#include "ExpImpSpec.h"
#include <iomanip>
#include "UI_Point.h"

//--------------------------------------------------------
#define ARRAY_SIZE(array) int(sizeof(array)/sizeof(array[0]))
//--------------------------------------------------------
template<class T>
const T& clamp(const T& val, const T& low, const T& high)
{
	return val < low ? low : val < high ? val : high; 
}
//--------------------------------------------------------
template<class T>
void clampInterval(T& minVal, T& maxVal, const T& low, const T& high)
{
	minVal = clamp(minVal, low, high);
	maxVal = clamp(maxVal, minVal, high);
}
//--------------------------------------------------------
template<class T>
bool inside(const T& val, const T& low, const T& high)
{
	return (val >= low && val <= high); 
}
//--------------------------------------------------------
inline int signum(double num)
{
	if (num > 0)
	{
		return 1;
	}
	else if (num < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
//--------------------------------------------------------
struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR StringWithoutCaseLess
{
	bool operator() (const ed::string& a, const ed::string& b) const
	{
		return _stricmp(a.c_str(), b.c_str()) < 0;
	}
};
//--------------------------------------------------------
template <class T>
ed::string FromValueToString(T inValue, int inPrecision, bool inShowSign = false)
{
	ed::ostringstream out;
    if (inPrecision > 0)
    {
		out.setf(std::ios_base::fixed, std::ios_base::floatfield);
		out.precision( inPrecision );
		if (inShowSign)
			out << std::setiosflags(std::ios::showpos);
		out << inValue;
	}
	else
	{
		if (inShowSign)
			out << std::setiosflags(std::ios::showpos);
		out << int(inValue);
	}
	return out.str();
}
//--------------------------------------------------------
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR double getDistanceToSegment(const Point& inPoint, const Point& inStart, const Point& inEnd);
//--------------------------------------------------------
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR int GetNearestInt(double double_value);
//--------------------------------------------------------
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ed::string makePath(const char* inFileName, const char* inFolderName);
//--------------------------------------------------------
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR bool checkPath(const char* inPathName);
//--------------------------------------------------------
bool isControlKeyPressed(int keyCode);
//--------------------------------------------------------

#endif	// __USERINTERFACE_UTILITIES_H__
#include "ui_Point.h"
#include "MultiColumnListBoxTypes.h"

inline Serializer& operator<<(Serializer& serializer, Point& point)
{
	return serializer << point.x << point.y;
}

inline Serializer& operator<<(Serializer& serializer, ColumnsPrms& prms)
{
	return serializer << prms.width << prms.dist << prms.itemSet;
}

inline Serializer& operator<<(Serializer& serializer, RowsPrms& prms)
{
	return serializer << prms.height << prms.dist;
}

inline Serializer& operator<<(Serializer& serializer, Marker& prms)
{
	return serializer << prms.xShift << prms.yShift << prms.proportional;
}

#ifndef viObjectFlatShape_H
#define viObjectFlatShape_H

#include "viObjectShape.h"
#include "cPosition.h"

// для статических или редко-двигающихся объектов, которые могут хранить ClipVolume
class EDOBJECTS_API viObjectFlatShape : public viObjectShape
{
public:
	viObjectFlatShape(viFlag _type = viUnknown);
	~viObjectFlatShape();

	virtual ClipVolume *FlatShape() = 0;

	// тестируем на включение в объем
	virtual IntersectionType TestVolume(ClipVolume *volume);
};

#endif // viObjectFlatShape_H

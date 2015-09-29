#ifndef viEffect_H
#define viEffect_H

#include "viObject.h"

// Базовый класс для объемных эффектов
class viEffect : public viObject
{
public:
	viEffect(viFlag _type = viUnknown):viObject(_type){}
};


#endif // viEffect_H

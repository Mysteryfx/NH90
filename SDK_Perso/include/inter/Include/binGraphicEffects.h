#ifndef _BIN_GRAPHIC_EFFECTS_H_
#define _BIN_GRAPHIC_EFFECTS_H_

#include "binEffects.h"

#include <ed/unordered_map.h>

class Effects::GraphicEffect;

//ключом является ссылка на граф. представление эффекта
typedef ed::unordered_map< Effects::GraphicEffect* , binEffects > tMapGraphicEffect;

#endif	//_BIN_GRAPHIC_EFFECTS_H_
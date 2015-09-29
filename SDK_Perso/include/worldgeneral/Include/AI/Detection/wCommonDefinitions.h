#pragma once

#include "wMovingObject.h"

enum BACKGROUND_TYPE {BACKGROUND_TYPE_AIR, BACKGROUND_TYPE_LAND, BACKGROUND_TYPE_FOREST, BACKGROUND_TYPE_ROAD, BACKGROUND_TYPE_RUNWAY, BACKGROUND_TYPE_WATER, BACKGROUND_TYPE_MAX};

float get_object_size(woPointer obj_ptr);

#ifdef DEBUG_TEXT
ed::string get_background_type_name(BACKGROUND_TYPE background_type);
#endif
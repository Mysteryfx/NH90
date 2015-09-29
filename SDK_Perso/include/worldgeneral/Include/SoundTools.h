#pragma once

#include "WorldGeneral.h"
#include <ed/SounderAPI.h>

class MovingObject;

namespace Sound {

WORLDGENERAL_API ed::string createMainSounderName(MovingObject* host);
WORLDGENERAL_API void initMovObjSounder();
WORLDGENERAL_API void updateSounder(ed::SounderRef& sounder, MovingObject* host);

}

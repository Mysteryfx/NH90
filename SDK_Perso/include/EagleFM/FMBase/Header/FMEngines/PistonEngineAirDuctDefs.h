#pragma once

#include <ed/vector.h>

namespace EagleFM {

class AirDuctPart;
class JetCompressor;
class JetNozzle;

struct AirFlowState
{
	double T_;
	double P_;
};

typedef ed::vector<AirDuctPart*> AirDuct;

}
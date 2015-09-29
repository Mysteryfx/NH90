#ifndef ED_MODEL_MODELARGUMENTS_H
#define ED_MODEL_MODELARGUMENTS_H

#ifndef EDGE
	#include "Renderer/DynamicParams.h"
#else
	#include "edModel/Parameters.h"
#endif

namespace model{
#ifndef EDGE
	typedef Graphics::DParamList Arguments;
#else
	typedef Parameters Arguments;
#endif
}

#endif

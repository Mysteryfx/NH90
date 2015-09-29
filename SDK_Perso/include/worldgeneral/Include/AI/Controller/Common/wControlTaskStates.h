#ifndef __wcControlTaskStates__
#define __wcControlTaskStates__

#include "AI/Controller/Common/Tasks.h"
#include "wControl.h"

namespace AI
{

AI_TASK_STATE_BEGIN(NoTask, wControl)
	AI_TASK_STATE_FINAL(NoTask, wControl)
	virtual bool checkFinish();
AI_TASK_STATE_END

}

#endif __wcControlTaskStates__
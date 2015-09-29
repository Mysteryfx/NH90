#pragma once

#include "Common/Profiler.h"

#ifndef _FINAL_VERSION

#define UNIQ_VAR2(x,y) x##y
#define UNIQ_VAR1(x,y) UNIQ_VAR2(x,y)
#define UNIQ_VAR UNIQ_VAR1(__profId, __LINE__)

#define PROFILE_FUNC() \
	static int __profFuncId = globalProfiler.registerUnit(__FUNCTION__); \
	Common::ProfiledFunc __profiledFunc(__profFuncId);

#define PROFILE_CODE(code, name) \
	static int UNIQ_VAR = globalProfiler.registerUnit(name); \
	globalProfiler.startProfiling(UNIQ_VAR); \
	code; \
	globalProfiler.endProfiling();

#define BEGIN_PROFILE_CODE(name) \
	static int UNIQ_VAR = globalProfiler.registerUnit(name); \
	globalProfiler.startProfiling(UNIQ_VAR);

#define END_PROFILE_CODE() \
	globalProfiler.endProfiling();

#else

#define PROFILE_FUNC() 
#define PROFILE_CODE(code, name) code;
#define BEGIN_PROFILE_CODE(name) 
#define END_PROFILE_CODE()

#endif
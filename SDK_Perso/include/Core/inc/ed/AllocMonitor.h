#ifndef ED_CORE_ALLOC_MONITOR_H
#define ED_CORE_ALLOC_MONITOR_H

#ifdef ED_ALLOCATOR_DEBUG
#include "_config.h"

#ifndef ED_CORE_EXPORT
	#define ED_CORE_EXPORT EDSIM_DLL_IMPORT
#endif // ED_CORE_EXPORT
#endif

#include <stdint.h>

namespace ed {
#ifdef ED_ALLOCATOR_DEBUG
ED_CORE_EXPORT void incSTLAllocatorCounter(const char *moduleName, size_t n, intptr_t &p);
ED_CORE_EXPORT void decSTLAllocatorCounter(size_t n, intptr_t p);
ED_CORE_EXPORT void incNEWAllocatorCounter(const char *moduleName, size_t n, intptr_t &p);
ED_CORE_EXPORT void decNEWAllocatorCounter(size_t n, intptr_t p);
ED_CORE_EXPORT void resetAllocatorMonitorCounters();
void initAllocatorMonitor();

ED_CORE_EXPORT size_t getNumSTLAllocs(const char *moduleName);
ED_CORE_EXPORT size_t getNumNewAllocs(const char *moduleName);

ED_CORE_EXPORT const char ** getModulesNames();
ED_CORE_EXPORT unsigned int getNumModulesNames();

ED_CORE_EXPORT void brakeOnNew(unsigned int moduleId);
ED_CORE_EXPORT void brakeOnSTL(unsigned int moduleId);
ED_CORE_EXPORT void brakeOnDelete(unsigned int moduleId);

#else
inline void incSTLAllocatorCounter(const char *moduleName, size_t n, intptr_t &p){}
inline void decSTLAllocatorCounter(size_t n, intptr_t p){}
inline void incNEWAllocatorCounter(const char *moduleName, size_t n, intptr_t &p){}
inline void decNEWAllocatorCounter(size_t n, intptr_t p){}
inline void resetAllocatorMonitorCounters(){}
inline void initAllocatorMonitor(){}

inline size_t getNumSTLAllocs(const char *moduleName){
	return 0;
}

inline size_t getNumNewAllocs(const char *moduleName)
{
	return 0;
}

inline const char ** getModulesNames(){return nullptr;}
inline unsigned int getNumModulesNames(){return 0;}

inline void brakeOnNew(unsigned int moduleId){}
inline void brakeOnSTL(unsigned int moduleId){}
inline void brakeOnDelete(unsigned int moduleId){}

#endif

} // namespace ed

#endif

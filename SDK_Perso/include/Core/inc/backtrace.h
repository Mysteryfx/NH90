/**
 * ED crash dump handler.
 * Written by Dmitry S. Baikov <dsb@eagle.ru>
 */
#ifndef _ED_BACKTRACE_H_
#define _ED_BACKTRACE_H_

#include "./_config.h"
#include <windows.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sets up crash handler.
 * Dumps to a dumpfilename-%TIMESTAMP%.crash
 */
ED_CORE_EXTERN
void bt_init(const char *dumpfilename, LPTOP_LEVEL_EXCEPTION_FILTER prevHandler);


/**
 * Resets crash handler.
 */
ED_CORE_EXTERN
void bt_fini();


/**
 * Resolve addresses from crash-dump, using debug info.
 * @param path search path for crash dump files
 * @param mask search mask for crash dump files
 * @return number of resolved crash dump files
 *
 * @note Should be called from the same binary.
 *
 * Use case:
 * On a user side app just calls bt_init("Temp/myproduct")
 * in case of crash, it dumps info to Temp/myproduct-timestamp.crash
 * but if user's binary does not have debug info, most addrs will be bare.
 * Developer gets crash dump, runs the same binary with special option
 * which on application start calls bt_retrace("Temp/")
 * bt_retraces loads debug info and saves updated crash dump to Temp/*.info
 */
ED_CORE_EXTERN
int bt_retrace(const char* path);

ED_CORE_EXTERN
long bt_write_dump(PEXCEPTION_POINTERS pExceptionInfo);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_BACKTRACE_H_ */

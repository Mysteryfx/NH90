/**
 * Various OS-related functions.
 */
#ifndef _ED_SYSTEM_H_
#define _ED_SYSTEM_H_

#include "./_config.h"
#include "./ed_defs.h"

EXTERN_C_BEGIN

/**
 * Get process command line in UTF-8 encoding.
 */
ED_CORE_EXTERN
const char* SYS_cmdline();

/**
 * Get number of process arguments.
 */
ED_CORE_EXTERN
int SYS_argc();

/**
 * Get process arguments.
 */
ED_CORE_EXTERN
const char*const* SYS_argv();

/*
 * Get process binary directory.
 */
ED_CORE_EXTERN
const char* SYS_bindir();

/**
 * Get process environment variable (both varname and value are UTF-8 encoded).
 * @return 1 on success, 0 on fail
 */
ED_CORE_EXTERN
int SYS_getenv(const char* varname, char* buf, size_t maxlen);

/**
 * Put process environment variable (both varname and value are UTF-8 encoded).
 */
ED_CORE_EXTERN
int SYS_putenv(const char *varname, const char *value);

/* hw info */
ED_CORE_EXTERN
int SYS_getCpuCores();

ED_CORE_EXTERN
int SYS_getSystemRamMB();

EXTERN_C_END

#endif /* _ED_SYSTEM_H_ */

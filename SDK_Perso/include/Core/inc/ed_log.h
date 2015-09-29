/**
 * ED Log API.
 *
 * Written by Dmitry S. Baikov <dsb@eagle.ru>
 */
/**
 * Usage:
 *
 * #define ED_LOG_MODULE "MySubsystem"
 * #include "ed_log.h"
 *
 * Following macros are available:
 *
 * ED_ERROR("printf-fmt-string", ...);
 * ED_INFO()
 * ED_WARNING()
 * ED_ALERT() - opens MessageBox window with error message
 * ED_DEBUG() - works in debug builds ONLY
 *
 *
 * For tracing and debug dumps use special log level TRACE:
 *   ED_TRACE("fmt", ...)
 * or (for special module name)
 *   ED_TRACE_AS("MyTraceModule", "fmt", ...);
 *
 * TRACE-level output NEVER gets into common log files (unless explicitly specified).
 * To manually set output for your traces use:
 * #include "ed_log_ctl.h"
 * // start writing traces:
 * ed_log_set_output("my-trace-file", "MyTraceModule", ED_LOG_TRACE);
 * // stop writing traces:
 * ed_log_set_output("my-trace-file", 0, 0);
 *
 * But better use log cofiguration file.
 */
#ifndef __ed_log_h__
#define __ed_log_h__

#include <stdarg.h>
#include "./_config.h"


// taken from POSIX syslog interface
enum {
    //ED_LOG_EMERG  = 1<<0, /**< system is unusable */
    ED_LOG_ALERT    = 1<<1, /**< action must be taken immediately */
    //ED_LOG_CRIT   = 1<<2, /**< critical conditions */
    ED_LOG_ERROR    = 1<<3, /**< error conditions */
    ED_LOG_WARNING  = 1<<4, /**< warning conditions */
    //ED_LOG_NOTICE = 1<<5, /**< normal, but significant, condition */
    ED_LOG_INFO     = 1<<6, /**< informational message */
    ED_LOG_DEBUG    = 1<<7, /**< debug-level message */

    /* special log level for debug dumps/traces */
    ED_LOG_TRACE    = 1<<8, /**< trace message */

    /* Never lose messages: ed_log() becomes blocking. Implied for ED_LOG_TRACE */
    ED_LOG_RELIABLE = 1<<15,
};


#if defined(_MSC_VER) && !defined(__cplusplus) && !defined(inline)
#define inline __inline
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * module name SHOULD be a constant, resident string.
 */

ED_CORE_EXTERN
void ed_log(const char *module, int level, const char *msg);

ED_CORE_EXTERN
void ed_vlogf(const char *module, int level, const char *fmt, va_list args);

static inline
void ed_logf(const char *module, int level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ed_vlogf(module, level, fmt, args);
    va_end(args);
}

#ifndef ED_LOG_MODULE
#define ED_LOG_MODULE __FILE__
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1400)

#define _ED_GEN_LOG(level) \
static inline void ED_##level(const char *fmt, ...) \
{ va_list args; va_start(args, fmt); ed_vlogf(ED_LOG_MODULE, ED_LOG_##level, fmt, args); va_end(args); }

#define _ED_NO_GEN_LOG(level) 	static inline void ED_##level(const char *fmt, ...) {}

_ED_GEN_LOG(ALERT)
_ED_GEN_LOG(ERROR)
_ED_GEN_LOG(WARNING)
_ED_GEN_LOG(INFO)

_ED_GEN_LOG(TRACE)
static inline void ED_TRACE_AS(const char *modName, const char *fmt, ...) \
{ va_list args; va_start(args, fmt); ed_vlogf(modName, ED_LOG_TRACE, fmt, args); va_end(args); }

#ifdef _FINAL_VERSION
_ED_NO_GEN_LOG(DEBUG)
#define ED_DEBUG_TRACE ED_DEBUG
static inline void ED_DEBUG_TRACE_AS(const char *modName, const char *fmt, ...) {}
#else
_ED_GEN_LOG(DEBUG)
#define ED_DEBUG_TRACE ED_TRACE
#define ED_DEBUG_TRACE_AS ED_TRACE_AS
#endif

#else // __VA_ARGS__ supported

#define _ED_LOG(level, ...) ed_logf(ED_LOG_MODULE, level, __VA_ARGS__)

#define ED_ALERT(...)    _ED_LOG(ED_LOG_ALERT,   __VA_ARGS__)
#define ED_ERROR(...)    _ED_LOG(ED_LOG_ERROR,   __VA_ARGS__)
#define ED_WARNING(...)  _ED_LOG(ED_LOG_WARNING, __VA_ARGS__)
#define ED_INFO(...)     _ED_LOG(ED_LOG_INFO,    __VA_ARGS__)

#define ED_TRACE(...)    _ED_LOG(ED_LOG_TRACE,    __VA_ARGS__)
#define ED_TRACE_AS(modName, ...) ed_logf(modName, ED_LOG_TRACE, __VA_ARGS__)

#ifdef _FINAL_VERSION
#define ED_DEBUG(...)    (void)0
#define ED_DEBUG_TRACE(...) (void)0
#define ED_DEBUG_TRACE_AS(mod, ...) (void)0
#else
#define ED_DEBUG(...)    _ED_LOG(ED_LOG_DEBUG,   __VA_ARGS__)
#define ED_DEBUG_TRACE    ED_TRACE
#define ED_DEBUG_TRACE_AS ED_TRACE_AS
#endif

#endif // __VA_ARGS__


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __ed_log_h__ */

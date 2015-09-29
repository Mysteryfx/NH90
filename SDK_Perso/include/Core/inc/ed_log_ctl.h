/**
 * ED Log control interface.
 *
 * Functions in this file are NOT thread-safe
 * with the exception of ed_log_set_output().
 *
 * Written by Dmitry S. Baikov <dsb@eagle.ru>
 */
#ifndef __ed_log_ctl_h__
#define __ed_log_ctl_h__

#include "./_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Initializes logger internals.
 */
int ed_log_init(void);

/**
 * Deinitialized logger.
 * Implicitly calls ed_log_stop()
 */
ED_CORE_EXTERN
void ed_log_exit(void);


enum {
    /* skip TRACE in catch-all filters */
    ED_LOG_ALL_LEVELS = ED_LOG_TRACE - 1,
};

enum ED_LOG_OutputMode {
    // message is always written
    ED_LOG_OUTPUT_MESSAGE = 0,

    // additional properties
    ED_LOG_OUTPUT_TIME   = 1<<0,
    ED_LOG_OUTPUT_LEVEL  = 1<<1,
    ED_LOG_OUTPUT_MODULE = 1<<2,

    // full log output
    ED_LOG_OUTPUT_FULL = ED_LOG_OUTPUT_TIME | ED_LOG_OUTPUT_LEVEL | ED_LOG_OUTPUT_MODULE
};

/**
 * Starts logger thread.
 * If mainlog != NULL then implicitly makes
 * ed_log_set_output(mainlog, 0, ED_LOG_ALL_LEVELS, ED_LOG_OUTPUT_FULL);
 */
ED_CORE_EXTERN
int ed_log_start(const char *logdir, const char *mainlog);


/**
 * Stops logger thread.
 * Closes all outputs.
 */
ED_CORE_EXTERN
void ed_log_stop(void);

/**
 * Add output file with module/level pattern.
 * This is the only THREAD SAFE function here.
 * Next call with the same logname will change matching rules for the output.
 * To close output use: ed_log_set_output(logname, NULL, 0, 0);
 *
 * @param logname log-file name, file created as logdir/logname.log
 * @param module name or NULL to match all
 * @param level_mask bit mask of log levels
 */
ED_CORE_EXTERN
void ed_log_set_output(const char *logname, const char *module_regexp, int level_mask, int output_mask);

/**
 * Log hook record.
 */
struct ed_log_hook_record {
    double time;
    const char *module;
    const char *message;
    int level;
    int lost; // number of lost messages
};
typedef struct ed_log_hook_record ed_log_hook_record_t;

/**
 * Log hook function.
 * @note called from logging thread!
 */
typedef void (*ed_log_hook_func)(const ed_log_hook_record_t* rec, void* userdata);

/**
 * Install a hook function.
 * @return 1 on success, 0 on fail
 * @note hooks work ONLY between ed_log_start() and ed_log_stop() calls.
 * @note does not work inside hook callback.
 */
ED_CORE_EXTERN
int ed_log_hook_install(ed_log_hook_func, void* userdata);

/**
 * Remove a hook function.
 * @return 1 on success, 0 on fail
 * @note does not work inside hook callback.
 */
ED_CORE_EXTERN
int ed_log_hook_remove(ed_log_hook_func, void* userdata);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __ed_log_ctl_h__ */

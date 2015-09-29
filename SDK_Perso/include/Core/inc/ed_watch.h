#ifndef _ED_WATCH_H_
#define _ED_WATCH_H_

#include "./_config.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Watch verbosity level.
 */
enum ED_WatchLevel {
    ED_WATCH_BRIEF,
    ED_WATCH_FULL,
};

typedef uintptr_t ED_WatchId;

/**
 * Watch function.
 * Provides a human readable string of watch's contents.
 * Returns number of bytes needed for the full info.
 * MAY be called from another thread!
 */
typedef size_t (*ED_WatchFunc)(int level, char *buf, size_t maxlen, void *userdata);

/**
 * Callback to receive watch name.
 */
typedef void (*ED_WatchListFunc)(const char *name, void *userdata);

#if defined(ENABLE_WATCHES) || !defined(_FINAL_VERSION)

/**
 * Add watch.
 * \param name provides a name of watch (should be unique)
 * \param func watch pretty-print function
 * \param userdata data for a print callback
 * \returns 1 on success
 */
ED_CORE_EXTERN
int ED_watch_set(const char *name, ED_WatchFunc func, void *userdata, int showOnScreen);

/**
 * Get watch ID by name
 * \param name provides a name of watch
 * \returns >0 on succes
 */
ED_CORE_EXTERN
ED_WatchId ED_get_watch_id(const char *name);

/**
 * Delete watch.
 * \param section to search
 * \param name of watch
 * ED_watch_del(section, NULL) deletes the section
 * ED_watch_del(NULL, NULL) deletes all watches
 */
ED_CORE_EXTERN
void ED_watch_reset(const char *name);

/**
 * Get all watch info as a string (each watch starts from a new line and 'WatchName: ')
 */
ED_CORE_EXTERN
size_t ED_watch_get_all(int level, char *buf, size_t maxlen, int screenOnly);

/**
 * Get list of all active watches.
 */
ED_CORE_EXTERN
void ED_watch_get_list(ED_WatchListFunc cb, void *userdata);

/**
 * Get one watch as a string.
 */
ED_CORE_EXTERN
size_t ED_watch_get_one(const char *name, int level, char *buf, size_t maxlen);

/**
 * Get one watch as a string by ID.
 */
ED_CORE_EXTERN
size_t ED_watch_get_one_by_id(ED_WatchId id, int level, char *buf, size_t maxlen);



/*
 *
 */
int ED_watch_init();
void ED_watch_exit();

#else

static int ED_watch_set(const char *name, ED_WatchFunc func, void *userdata, int showOnScreen){return 1;}
static void ED_watch_reset(const char *name){}
static size_t ED_watch_get_all(int level, char *buf, size_t maxlen){if(maxlen > 0){buf[0] = '\0';} return 0;}
static void ED_watch_get_list(ED_WatchListFunc cb, void *userdata){}
static size_t ED_watch_get_one(const char *name, int level, char *buf, size_t maxlen){return 0;}
static size_t ED_watch_get_one(ED_WatchId id, int level, char *buf, size_t maxlen){return 0;}
static int ED_watch_init(){return 1;}
static void ED_watch_exit(){}
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "ed/string.h"

#include <functional>

namespace ed{
class JsonWriter;

typedef std::function<void(JsonWriter&)> DumpFunc;

#if defined(ENABLE_WATCHES) || !defined(_FINAL_VERSION)
ED_CORE_EXTERN void dump_set(const char *name, DumpFunc func);
ED_CORE_EXTERN void dump_reset(const char *name);
/// Requests to make dump on the end of current frame.
ED_CORE_EXTERN void request_dump();
/// Creates dump.
ED_CORE_EXTERN void make_dump();
/// Returns created dump or empty string if dump isn't ready.
ED_CORE_EXTERN ed::string get_dump();
#else
inline void dump_set(const char *name, DumpFunc func){}
inline void dump_reset(const char *name){}
void make_dump(){}
ed::string get_dump(){return "";}
#endif
}
#endif

#endif /* _ED_WATCH_H_ */

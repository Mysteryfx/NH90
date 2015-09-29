/**
 * Faster, smaller, thread-safe gettext library implementation.
 * Written by Dmitry S. Baikov <dsb@eagle.ru>
 *
 * Application interface.
 */
#ifndef _ED_gettext_h_
#define _ED_gettext_h_

#include "./_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Implements gettext() interface.
 * Looks up string translation. Returns key string if no translation found.
 * NOTE: forget the value ASAP, because subsequent call to ED_gettext_detach() may invalidate the pointer.
 */
ED_CORE_EXTERN
const char* ED_gettext_translate(const char* key);

/**
 * TODO: plural forms API
ED_CORE_EXTERN
const char* ED_gettext_ntranslate(const char* key, int n);
*/

/**
 * gettext-compatible macros
 */
#define gettext(str) ED_gettext_translate(str)
#define dgettext(dom, str) ED_gettext_translate(str)

/*
 * The following gettext macros are user-level and should NOT be defined in API headers:
 * #define gettext_noop(str) str
 * #define _(str) gettext(str)
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_gettext_h_ */

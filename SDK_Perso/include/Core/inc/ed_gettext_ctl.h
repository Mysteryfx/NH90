/**
 * Faster, smaller, thread-safe gettext library implementation.
 * Written by Dmitry S. Baikov <dsb@eagle.ru>
 *
 * Control interface.
 */
#ifndef _ED_gettext_ctl_h_
#define _ED_gettext_ctl_h_

#include "./_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Init library.
 * @retval 0 OK
 * @return error code
 */
ED_CORE_EXTERN
int ED_gettext_init();

/**
 * Shut the library down. Automatically detaches all MO-files.
 * @note deletes the mutex and thus is not thread-safe.
 * @retval 0 OK
 * @return error code
 */
ED_CORE_EXTERN
void ED_gettext_exit();

/**
 * Add translations from a given MO-file.
 * @retval 0 OK
 * @return error code
 */
ED_CORE_EXTERN
int ED_gettext_attach(const char* filename);

/**
 * Remove translations from a given MO-file.
 * @note may invalidate corresponding pointers returned by ED_gettext_translate()
 * @retval 0 OK
 * @return error code
 */
ED_CORE_EXTERN
int ED_gettext_detach(const char* filename);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_gettext_ctl_h_ */

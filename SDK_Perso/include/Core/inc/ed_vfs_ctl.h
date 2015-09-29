#ifndef _ED_VFS_CTL_H_
#define _ED_VFS_CTL_H_

#include "./_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Start VFS.
 * @param subdir_name subdirectory name for VFS_get_writedir() and VFS_get_tempdir()
 * @return 1 if ok
 */
ED_CORE_EXTERN
int VFS_init(const char *write_subdir_name, const char *base_dir);

/**
 * Stop VFS.
 */
ED_CORE_EXTERN
void VFS_exit();

/**
 * Add location to list.
 * @see VFS_get_locations
 */
ED_CORE_EXTERN
int VFS_add_location(const VFS_LocationInfo *info);

/**
 * Remove location by its path.
 * @see VFS_get_locations
 */
ED_CORE_EXTERN
int VFS_del_location(const char *path);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_VFS_H_ */

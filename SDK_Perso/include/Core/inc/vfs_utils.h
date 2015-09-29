#ifndef _ED_Core_VFS_UTILS_
#define _ED_Core_VFS_UTILS_

#include <stdint.h>
#include <functional>

#include "./_config.h"

#define TEXTURES_MOUNT_POINT	"/textures/"
#define MODELS_MOUNT_POINT		"/models/"
#define EFFECTS_MOUNT_POINT		"/effects/"
#define TMP_MOUNT_POINT			"/tmp/"
#define LIVERIES_MOUNT_POINT	TEXTURES_MOUNT_POINT"/liveries/"

/// Initializes virtual fs. Must be called only once.
ED_CORE_EXPORT void vfs_init();

/// Deinitializes virtual fs.
ED_CORE_EXPORT void vfs_deinit();

/// Mounts archive or directory. If srcDir doesn't have extentions first it'll be mounted as directory and then it'll
/// \param srcDir source archive or directory. If it doesn't have extention first it'll be mounted as directory and then independently on result mounted as archive with substituting supported archives extentions.
/// \param mountPoint mount point.
/// \param appendToPath nonzero to append to search path, zero to prepend.
/// Returns nonzero if added to path, zero on failure (bogus archive, dir missing, etc).
ED_CORE_EXPORT int vfs_mount(const char* srcDir,const char* mountPoint,int appendToPath);

/// Removes a directory or archive from the search path.
ED_CORE_EXPORT int vfs_unmount(const char* dir);

/// Reads the whole file to buffer and returns it or NULL if some problems occured.
/// You must delete (using vfs_free_file_buffer) returned buffer by youself.
/// \param bufSize holds size of buffer.
ED_CORE_EXPORT int8_t* vfs_read_file(const char* fileName,size_t* bufSize);

/// Reads the whole file to buffer and returns it or NULL if some problems occured.
ED_CORE_EXPORT int8_t* vfs_read_file(const char* fileName,size_t* bufSize, std::function<int8_t*(size_t)> allocFunc);

/// Frees buffer allocated by vfs_read_file.
ED_CORE_EXPORT void vfs_free_file_buffer(int8_t* buf);

#endif

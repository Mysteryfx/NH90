#ifndef _ED_VFS_H_
#define _ED_VFS_H_

#include "./_config.h"
#include "./ed_defs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct VFS_File VFS_File;


// filename buffer
typedef char VFS_Filename_t[256];


/**
 * Enumerate directory callback.
 */
typedef void (*VFS_readdir_callback)(void *userdata, const char *dir, const char *filename);

/**
 * Location details.
 */
struct VFS_LocationInfo {
    const char *name;
    const char *path;
    //int kind;
    //const char *icon_name;
};
typedef struct VFS_LocationInfo VFS_LocationInfo;

/**
 * Enumerate locations callback.
 */
typedef void (*VFS_locations_callback)(void *userdata, const VFS_LocationInfo *info);

/* === Directory I/O === */

/**
 * Get path to a base directory.
 * @return write path ending with "/".
 */
ED_CORE_EXTERN
const char* VFS_get_basedir();

/**
 * Get path to a write directory.
 * @return write path ending with "/".
 */
ED_CORE_EXTERN
const char* VFS_get_writedir();

/**
 * Get path to a temporary directory.
 * @return temp path ending with "/".
 */
ED_CORE_EXTERN
const char* VFS_get_tempdir();

/**
 * Get path to a application data directory.
 * @return temp path ending with "/".
 */
ED_CORE_EXTERN
const char* VFS_get_appdatadir();

/**
 * Enumerate locations (devices and places).
 */
ED_CORE_EXTERN
void VFS_get_locations(VFS_locations_callback cb, void *userdata);

/**
 * Search for file in writedir, then in the basedir().
 */
ED_CORE_EXTERN
int VFS_get_filepath(char *out_path, size_t maxlen, const char* filename);


/**
 * Join paths ensuring proper path separators.
 * @param out buffer to store output path
 * @param maxlen size of buffer
 * @param base first path of path (may alias with out)
 * @param append part to append (may be NULL, meaning nothing to append)
 * @return 1 on success, 0 on insufficient space.
 *
 * Examples:
 * VFS_Filename_t out;
 * VFS_path_join(out, sizeof(out), "c:\\", "test"); // out = "c:\\test"
 * VFS_path_join(out, sizeof(out), out, "dir"); // out = "c:\\test\\dir"
 * VFS_path_join(out, sizeof(out), out, ""); // out = "c:\\test\\dir\\"
 * VFS_path_join(out, sizeof(out), "a:\\", NULL); // out = "a:\\"
 *
 */
ED_CORE_EXTERN
int VFS_path_join(char *out, size_t maxlen, const char *base, const char *append);

/**
 * Split path ensuring proper path separators.
 * Combines normpath(dirname()) and basename in a single call.
 * @param path path to split
 * @param dirname buffer to store directory name (can be NULL)
 * @param dirname_max size of dir buffer
 * @param basename_ptr pointer to basename (can be NULL)
 * @return 1 on success, 0 on insufficient space.
 */
ED_CORE_EXTERN
int VFS_path_split(const char *path, char *dirname, size_t dirname_max, const char** basename_ptr);

/**
 * Normalize path. Converts and de-duplicates path separators.
 * @param out output buffer
 * @param maxlen output buffer size
 * @param path path to normalize (may be equal to out)
 * @return 1 on success, 0 on insufficient space.
 */
ED_CORE_EXTERN
int VFS_path_normalize(char *out, size_t maxlen, const char *path);

/**
 * Normalize path case (Converts characters to lower case on case-insensitive systems).
 * @param out output buffer
 * @param maxlen output buffer size
 * @param path path to normalize (may be equal to out)
 * @return 1 on success, 0 on insufficient space.
 */
ED_CORE_EXTERN
int VFS_path_normcase(char *out, size_t maxlen, const char *path);

/**
 * Get os-dependent absolute path (Converts characters to lower case on case-insensitive systems).
 * @param out output buffer
 * @param maxlen output buffer size
 * @param path path to absolutize (may be equal to out)
 * @return 1 on success, 0 on insufficient space or invalid filename.
 */
ED_CORE_EXTERN
int VFS_path_getreal(char *out, size_t maxlen, const char *path);

/**
 * Check if 'path' is within a given 'root'.
 * @param root base directory
 * @param path path that should be inside the base
 * @return 1 if path is inside root, 0 if not.
 */
ED_CORE_EXTERN
int VFS_path_contains(const char *root, const char *path);

/**
 * Check if file exists in the search path.
 * @return 0 if not exists, !0 if does exist
 */
ED_CORE_EXTERN
int VFS_exists(const char *utf8path);

/**
 * Check if entry is a directory.
 * @return !0 if file exists and is a directory
 */
ED_CORE_EXTERN
int VFS_isdir(const char *utf8path);

/**
 * Make directory (recursive).
 * @return non-zero on success, 0 on fail
 */
ED_CORE_EXTERN
int VFS_mkdir(const char *utf8path);

/**
 * Delete file.
 * @return non-zero on success, 0 on fail
 */
ED_CORE_EXTERN
int VFS_remove(const char *utf8path);

/**
 * Enumerate directory.
 * @return non-zero on success, 0 on fail
 */
ED_CORE_EXTERN
int VFS_readdir(const char *utf8path, VFS_readdir_callback, void *userdata);

/**
 * Get file's last modification time.
 * @return a number of seconds since the epoch (Jan 1, 1970).
 * @return 0 if unknown
 */
ED_CORE_EXTERN
uint64_t VFS_get_mod_time(const char *filename);

/**
 * Get file size by name.
 * @return file size or 0 on error
 */
ED_CORE_EXTERN
uint64_t VFS_get_file_size(const char *filename);

/**
 * Get MD5 hash of a given file.
 * @return 1 on success or 0 on error
 */
ED_CORE_EXTERN
int VFS_get_file_md5(const char *utf8path, uint8_t md5digest[16]);


/* === File I/O */

/**
 * Open file for reading.
 * @return file handle or NULL if failed
 */
ED_CORE_EXTERN
VFS_File* VFS_open_read(const char *utf8path);

/**
 * Open file for writing.
 * @return file handle or NULL if failed
 */
ED_CORE_EXTERN
VFS_File* VFS_open_write(const char *utf8path);

/**
 * Open file for appending.
 * @return file handle or NULL if failed
 */
ED_CORE_EXTERN
VFS_File* VFS_open_append(const char *utf8path);

/**
 * Read from file.
 * @return number of bytes read or 0 on eof or error
 */
ED_CORE_EXTERN
uint64_t VFS_read(VFS_File* f, void *buf, uint64_t len);

/**
 * Write to file.
 * @return number of bytes written or 0 on error
 */
ED_CORE_EXTERN
uint64_t VFS_write(VFS_File* f, const void *buf, uint64_t len);

/**
 * Set file pointer (absolute).
 * @return 0 on error
 */
ED_CORE_EXTERN
int VFS_seek(VFS_File* f, uint64_t pos);

/**
 * Get file pointer.
 * @return current position in file
 */
ED_CORE_EXTERN
uint64_t VFS_tell(VFS_File* f);

/**
 * Close file.
 */
ED_CORE_EXTERN
void VFS_close(VFS_File* f);

/**
 * Get file size.
 * For open_write and open_append modes may return number of bytes actually written to disk.
 * @return file size or 0 on error
 */
ED_CORE_EXTERN
uint64_t VFS_getsize(VFS_File* f);


/**
 * Check End-Of-File condition.
 * @return 0 - not EOF, !0 EOF (1 = EOF, -1 = error)
 */
//ED_CORE_EXTERN
//int VFS_iseof(VFS_File *f);

/**
 * Get file error.
 * @return errno error code (0 - no error)
 */
//ED_CORE_EXTERN
//int VFS_geterror(VFS_File *f);

/**
 * Force the write of data to disk.
 */
//ED_CORE_EXTERN
//void VFS_flush(VFS_File *f);


/* === Memory-only files === */

/**
 * Open memory buffer for reading.
 */
//VFS_File* VFS_open_mem_read(const void *buf, size_t sz);

/**
 * Open memory buffer for writing.
 */
//VFS_File* VFS_open_mem_write(void *buf, size_t sz);



/* === Typed data reading === */

/**
 * Read 8-bit unsigned int.
 */
ED_CORE_EXTERN
uint8_t VFS_get_uint8(VFS_File *f);

/**
 * Read 8-bit signed int.
 */
static inline
int8_t VFS_get_int8(VFS_File *f) { return (int8_t)VFS_get_uint8(f); }

/**
 * Read 16-bit unsigned int.
 */
ED_CORE_EXTERN
uint16_t VFS_get_uint16(VFS_File *f);

/**
 * Read 16-bit signed int.
 */
static inline
int16_t VFS_get_int16(VFS_File *f) { return (int16_t)VFS_get_uint16(f); }

/**
 * Read 32-bit unsigned int.
 */
ED_CORE_EXTERN
uint32_t VFS_get_uint32(VFS_File *f);

/**
 * Read 32-bit signed int.
 */
static inline
int32_t VFS_get_int32(VFS_File *f) { return (int32_t)VFS_get_uint32(f); }

/**
 * Read 64-bit unsigned int.
 */
ED_CORE_EXTERN
uint64_t VFS_get_uint64(VFS_File *f);

/**
 * Read 64-bit signed int.
 */
static inline
int64_t VFS_get_int64(VFS_File *f) { return (int64_t)VFS_get_uint64(f); }

/**
 * Read 32-bit IEEEE float value.
 */
static inline
float VFS_get_float(VFS_File *f) { float v; *(uint32_t*)&v = VFS_get_uint32(f); return v; }

/**
 * Read 64-bit IEEEE double float value.
 */
static inline
double VFS_get_double(VFS_File *f) { double v; *(uint64_t*)&v = VFS_get_uint64(f); return v; }

/**
 * Read multibyte encoded unsigned int (size_t).
 */
ED_CORE_EXTERN
uint64_t VFS_get_mbint(VFS_File *f);

/**
 * Reads string with embedded size into specified buffer
 * If buffer is smaller that string size, remaining chars are skipped.
 * String is expected to be in UTF-8 encoding.
 * @return original string length
 */
ED_CORE_EXTERN
size_t VFS_get_string(VFS_File *f, char *buf, size_t maxlen);


/* === Typed data writing === */

/**
 * Write 8-bit unsigned int.
 */
ED_CORE_EXTERN
void VFS_put_uint8(VFS_File *f, uint8_t v);

/**
 * Write 8-bit signed int.
 */
static inline
void VFS_put_int8(VFS_File *f, int8_t v) { VFS_put_uint8(f, (uint8_t)v); }

/**
 * Write 16-bit unsigned int.
 */
ED_CORE_EXTERN
void VFS_put_uint16(VFS_File *f, uint16_t v);

/**
 * Write 16-bit signed int.
 */
static inline
void VFS_put_int16(VFS_File *f, int16_t v) { VFS_put_uint16(f, (uint16_t)v); }

/**
 * Write 32-bit unsigned int.
 */
ED_CORE_EXTERN
void VFS_put_uint32(VFS_File *f, uint32_t v);

/**
 * Write 32-bit signed int.
 */
static inline
void VFS_put_int32(VFS_File *f, int32_t v) { VFS_put_uint32(f, (uint32_t)v); }

/**
 * Write 64-bit unsigned int.
 */
ED_CORE_EXTERN
void VFS_put_uint64(VFS_File *f, uint64_t v);

/**
 * Write 64-bit signed int.
 */
static inline
void VFS_put_int64(VFS_File *f, int64_t v) { VFS_put_uint64(f, (uint64_t)v); }

/**
 * Write 32-bit IEEEE float value.
 */
static inline 
void VFS_put_float(VFS_File *f, float v) { VFS_put_uint32(f, *(uint32_t*)&v); }

/**
 * Write 64-bit IEEEE double float value.
 */
static inline 
void VFS_put_double(VFS_File *f, double v) { VFS_put_uint64(f, *(uint64_t*)&v); }

/**
 * Write multibyte encoded unsigned int (size_t).
 */
ED_CORE_EXTERN
void VFS_put_mbint(VFS_File *f, uint64_t v);

/**
 * Writes string with embedded size.
 * String is expected to be in UTF-8 encoding.
 */
ED_CORE_EXTERN
void VFS_put_string(VFS_File *f, const char *buf, size_t len);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_VFS_H_ */

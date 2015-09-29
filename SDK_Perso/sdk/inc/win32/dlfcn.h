/*

     dlfcn.h for Win32

     Written in 2000 by Dmitry S. Baikov
     Released as Public domain.
 */
/*============================================================================

     $Id: dlfcn.h,v 1.1 2000/12/13 16:42:33 dsb Exp $

     UNIX-like shared library functions

 ============================================================================*/

#ifndef WIN32_DLFCN_H
#define WIN32_DLFCN_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Open modes for dlopen
 */
#define RTLD_LAZY	1	/* lazy function call binding */
#define RTLD_NOW	2	/* immediate function call binding */
#define RTLD_GLOBAL	4	/* symbols in this dlopen'ed obj are visible to other dlopen'ed objs */

/*
 * load library
 * NOTE: efective mode value for Win32 is always (RTLD_NOW | RTLD_GLOBAL)
 */
extern void* dlopen(const char* filename, int mode);

/*
 * free library
 * RETURN: 0 - ok, !0 - error
 */
extern int dlclose(void* handle);

/*
 * get symbol's address
 */
extern void* dlsym(void* handle, const char* name);

/*
 * get last error in human-readable format
 * NOTE: original (UNIX) declaration is char* dlerror(),
 * but we use const char* as a return type for safety
 * i think, this shouldn't break good-written programs
 */
extern const char* dlerror();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* !WIN32_DLFCN_H */

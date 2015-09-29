#ifndef _ED_DEFS_H_
#define _ED_DEFS_H_

// MSVC C have __inline
#if defined(_MSC_VER) && !defined(__cplusplus) && !defined(inline)
#define inline __inline
#endif


// extern "C"
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END } // extern "C"
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif


#if 0 // do we need it?
#ifdef _MSC_VER
#define ED_LIB_IMPORT __declspec(dllimport)
#define ED_LIB_EXPORT __declspec(dllexport)
#else
#define ED_LIB_IMPORT
#define ED_LIB_EXPORT
#endif
#endif

#endif /* _ED_DEFS_H_ */

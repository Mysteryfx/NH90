/*
 * Hash functions library.
 *
 * Warning: provided hash functions are endian- and partition-dependent.
 */
#ifndef _ED_HASH_H_
#define _ED_HASH_H_

#include "./_config.h"
#include "ed_defs.h"
#include <stdint.h>

EXTERN_C_BEGIN

/**
 * Fast hash for general data.
 */
ED_CORE_EXTERN
uint32_t ED_hash_data(const void *data, size_t len, uint32_t seed);

/**
 * Fast hash optimized for character strings.
 * About 10-30% slower than ED_hash_data() with explicit length,
 * but faster than ED_hash_data(str, strlen(str)).
 */
ED_CORE_EXTERN
uint32_t ED_hash_string(const char *str, uint32_t seed);

/**
 * Fast hash for 32-bit data.
 */
ED_CORE_EXTERN
uint32_t ED_hash_uint32(uint32_t);

/**
 * Fast hash for 64-bit data.
 */
ED_CORE_EXTERN
uint32_t ED_hash_uint64(uint64_t);

/**
 * Fast hash for pointers.
 *
 * uint32_t ED_hash_pointer(const void* ptr)
 * { return ED_hash_int??((uintptr_t)ptr); }
 */
#if UINTPTR_MAX == UINT32_MAX
// 32-bit
static inline
uint32_t ED_hash_pointer(const void* ptr)
{ return ED_hash_uint32((uintptr_t)ptr); }

#elif UINTPTR_MAX == UINT64_MAX
// 64-bit
static inline
uint32_t ED_hash_pointer(const void* ptr)
{ return ED_hash_uint64((uintptr_t)ptr); }

#else
#error Unknown pointer size!
#endif


// TODO:
// MD5?
// SHA1?
// SHA2 (256/224, 512/384)
// CRC32?

EXTERN_C_END

#endif /* _ED_HASH_H_ */

/**
 * ED string helpers.
 */
#ifndef _ED_STRING_H_
#define _ED_STRING_H_


#include <string.h>


#if defined(_MSC_VER) && !defined(__cplusplus) && !defined(inline)
#define inline __inline
#endif



/**
 * Copy string to buffer of specified size, padding out with zeroes and always end with '\0'
 * essentially, this is strncpy() + explicit zero-termination.
 */
static inline
void ED_strncpyz(char *out, const char *in, size_t maxlen)
{
    if (in && in[0] != '\0')
    {
        strncpy(out, in, maxlen);
        out[maxlen-1] = '\0';
    }
    else
        memset(out, 0, maxlen);
}

/**
 * Valid cross-platform snprintf and vsnprinf
 */
#ifdef _MSC_VER
// ARGHHHHH....
#include <stdarg.h>
#include <stdio.h>
static inline
int ED_vsnprintf(char *buf, size_t len, const char *fmt, va_list args)
{
    int res;
    res = _vsnprintf(buf, len, fmt, args);
	if(len != 0) buf[len -1] = '\0';
    return res;
}
static inline
int ED_vsnwprintf(wchar_t *buf, size_t len, const wchar_t *fmt, va_list args)
{
    int res;
    res = _vsnwprintf(buf, len, fmt, args);
	if(len != 0) buf[len -1] = '\0';
    return res;
}
static inline
int ED_snprintf(char *buf, size_t len, const char *fmt, ...)
{
    int res;
    va_list args;
    va_start(args, fmt);
    res = _vsnprintf(buf, len, fmt, args);
    if(len != 0) buf[len -1] = '\0';
    va_end(args);
    return res;
}
#define strcasecmp stricmp

#ifdef EDGE
#define _snprintf "Forget about this Visual C++ specific nonportable function. Use ED_snprintf instead."
#endif

#else
#include <stdio.h>
#define stricmp strcasecmp
#define ED_snprintf snprintf
#define ED_vsnprintf vsnprintf
#define ED_vsnwprintf vswprintf
#endif


/**
 * Calculate fast string hash
 */
/* djb2 from http://www.cse.yorku.ca/~oz/hash.html */
static inline
unsigned long
ED_strhash(const unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 + c */
    return hash;
}

/**
 * Simple hash-table helper
 * Usage:
 *
 * struct MyEntry { struct MyEntry *hash_next; xxx; char name[64]; }
 *
 * struct MyEntry myhash[HASH_SIZE];
 *
 * struct MyEntry **find_entry(const char *newname) {
 *     unsigned hash = ED_strhash(newname) % HASH_SIZE;
 *     struct MyEntry **p = &myhash[hash];
 *     ED_STRHASH_FIND(p, name, hash_next, newname); // HERE WE ARE!
 *     return p;
 * }
 *
 * ...
 * struct MyEntry **p = find_entry("test");
 * if (*p) {
 *     printf("found %s, deleting\n", (*p)->name);
 *     struct MyEntry *dead = *p;
 *     *p = dead->hash_next; // remove from hash list
 *     delete_myentry(dead);
 * } else {
 *     printf("not found, inserting\n");
 *     struct MyEntry *born = create_myentry("test");
 *     born->hash_next = *p; // essentially NULL
 *     *p = born;
 * }
 */
#define ED_STRHASH_FIND(pptr, field, next, value) \
    while (*pptr) if (strcmp((*pptr)->field, value)==0) break; else pptr = &(*pptr)->next;

#endif /* _ED_STRING_H_ */

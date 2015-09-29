/**
 * ED Base library
 *
 * UTF-8 handling.
 *
 * Author: dsb@eagle.ru
 */
#ifndef _ED_UTF8_H_
#define _ED_UTF8_H_

#include "./_config.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define ED_UTF8_SEQ_MAX 6
#define ED_UTF8_INVALID 0xFFFFFFFF
 
/**
 * Decode UTF-8 char and advance pointer
 * @param maxlen maximum number of bytes to read
 * Stops on '\0' or reading maxlen bytes
 * @return Unicode or ED_UTF8_INVALID
 */
ED_CORE_EXTERN
uint32_t UTF8_getc(const char **p, size_t maxlen);
 
/**
 * Encode UTF-8 char and advance pointer
 * @param p pointer to pointer to buffer (will be advanced)
 * @param maxlen maximum number of bytes to write
 * @return number of bytes written or 0 if insufficient buffer space
 */
ED_CORE_EXTERN
size_t UTF8_putc(char **p, size_t maxlen, uint32_t code);
 
/**
 * Get number of chars in UTF-8 string.
 * @return number of Unicode characters.
 */
ED_CORE_EXTERN
size_t UTF8_strlen(const char *);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_UTF8_H_ */

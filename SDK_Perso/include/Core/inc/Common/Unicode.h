#ifndef __Common_Unicode_h__
#define __Common_Unicode_h__

#include "_config.h"
#include <ed/string.h>
#include "ed_utf8.h"

namespace Common {

ED_CORE_EXTERN  ed::string
iconv_utf8_from_wchar(const wchar_t* text);

ED_CORE_EXTERN  ed::string
iconv_utf8_from_wchar(const ed::wstring& text);

ED_CORE_EXTERN  ed::wstring
iconv_utf8_to_wchar(const ed::string& text);

ED_CORE_EXTERN  ed::string
iconv_utf8_to_oem(const ed::string& text);

// convert UTF-8 character to Unicode value and increment buffer
inline unsigned mb2uni(const unsigned char *&str)
{ return (unsigned)UTF8_getc((const char**)&str, ED_UTF8_SEQ_MAX); }

// write Unicode value to buffer
// returns length written
inline int uni2mb(unsigned char *&str, unsigned u)
{ return (int)UTF8_putc((char**)&str, ED_UTF8_SEQ_MAX, (uint32_t)u); }

// get length of UTF-8 string
inline int mbstrlen(const unsigned char *str)
{ return (int)UTF8_strlen((const char*)str); }

// convert Unicode character to UTF-8 string
ED_CORE_EXTERN  ed::string uni2mbstr(unsigned u);

// substring UTF-8 string
ED_CORE_EXTERN  ed::string utf8_substr(const char * in,size_t pos,size_t n);

} // namespace Common

#endif // #ifndef __Common_Unicode_h__

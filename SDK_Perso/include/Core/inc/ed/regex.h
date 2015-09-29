#ifndef _ed_regex_h_
#define _ed_regex_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "ed/allocator.h"
#endif

#include <regex>

#include "ed/string.h"

namespace ed {
#ifdef USE_NEDMALLOC_STL_ALLOCATOR
typedef std::match_results<const char*, allocator<std::sub_match<char*>>> cmatch;
typedef std::match_results<const wchar_t*, allocator<std::sub_match<wchar_t*>>> wcmatch;
typedef std::match_results<string::const_iterator, allocator<std::sub_match<string::const_iterator>>> smatch;
typedef std::match_results<wstring::const_iterator, allocator<std::sub_match<wstring::const_iterator>>> wsmatch;
#else
typedef std::match_results<const char*> cmatch;
typedef std::match_results<const wchar_t*> wcmatch;
typedef std::match_results<string::const_iterator> smatch;
typedef std::match_results<wstring::const_iterator> wsmatch;
#endif
}

#endif /* _ed_regex_h_ */

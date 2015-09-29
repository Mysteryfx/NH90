#ifndef _ed_sstream_h_
#define _ed_sstream_h_

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
#include "./allocator.h"
#endif

#include <sstream>
#include <ed/string.h>

namespace ed {

#ifdef USE_NEDMALLOC_STL_ALLOCATOR
template<typename T, typename Traits = std::char_traits<T>, typename Alloc=ed::allocator<T> >
class basic_stringstream : public std::basic_stringstream<T, Traits, Alloc> {
    typedef std::basic_stringstream<T, Traits, Alloc> base;
public:
	explicit basic_stringstream(std::ios_base::openmode which = std::ios_base::out|std::ios_base::in ):base(which){}
	explicit basic_stringstream(const std::basic_string<T, Traits, Alloc> &str,
                        std::ios_base::openmode which = std::ios_base::out|std::ios_base::in):base(str, which){}
};

template<typename T, typename Traits = std::char_traits<T>, typename Alloc=ed::allocator<T> >
class basic_istringstream : public std::basic_istringstream<T, Traits, Alloc> {
    typedef std::basic_istringstream<T, Traits, Alloc> base;
public:
	explicit basic_istringstream(std::ios_base::openmode which = std::ios_base::in ):base(which){}
	explicit basic_istringstream(const std::basic_string<T, Traits, Alloc> &str,
                        std::ios_base::openmode which = std::ios_base::in):base(str, which){}
};

template<typename T, typename Traits = std::char_traits<T>, typename Alloc=ed::allocator<T> >
class basic_ostringstream : public std::basic_ostringstream<T, Traits, Alloc> {
    typedef std::basic_ostringstream<T, Traits, Alloc> base;
public:
	explicit basic_ostringstream(std::ios_base::openmode which = std::ios_base::out):base(which){}
	explicit basic_ostringstream(const std::basic_string<T, Traits, Alloc> &str,
                        std::ios_base::openmode which = std::ios_base::out):base(str, which){}
};

typedef basic_stringstream<char> stringstream;
typedef basic_stringstream<wchar_t> wstringstream;
typedef basic_istringstream<char> istringstream;
typedef basic_ostringstream<char> ostringstream;

#else

typedef std::stringstream stringstream;
typedef std::wstringstream wstringstream;
typedef std::ostringstream ostringstream;
typedef std::istringstream istringstream;

#endif

} // namespace ed

#endif /* _ed_sstream_h_ */

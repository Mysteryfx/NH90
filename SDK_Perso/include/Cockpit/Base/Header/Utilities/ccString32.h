#pragma once
#include "CockpitBase.h"
#include <ed/string.h>

namespace cockpit {  namespace sc {

    typedef std::basic_string<unsigned, std::char_traits<unsigned>, std::allocator<unsigned> >  str_32_bit;
    void        COCKPITBASE_API to32bit  (const ed::string & from,str_32_bit & to);
    void        COCKPITBASE_API from32bit(const str_32_bit  & from,ed::string & to);

    str_32_bit  COCKPITBASE_API to32bit  (const ed::string & from);
    ed::string COCKPITBASE_API from32bit(const str_32_bit  & from);
}}
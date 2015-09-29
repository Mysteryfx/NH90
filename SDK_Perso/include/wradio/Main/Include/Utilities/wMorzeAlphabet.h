
#ifndef _WMORZEALPHABET_H_
#define _WMORZEALPHABET_H_
#include "WRadio.h"
#include <ed/string.h>
#include <ed/map.h>

/*
UTF8
*/
class WRADIO_API morze
{
    morze();
   ~morze();
    morze(const morze & m);
public:
    static const ed::string    & get_char(unsigned c);
    static ed::string            convert(const char * str);
    static const ed::string    & next(const char *&str);
private:
    typedef ed::map<int,ed::string> morze_alphabet;
    static const morze_alphabet & alphabet();
};
#endif
#ifndef __wMsg__
#define __wMsg__

//wMsg - the list of wMessage event identifiers

#undef BEGIN_WMESSAGES
#define BEGIN_WMESSAGES enum wMsg {
#undef REGISTER_WMESSAGE
#define REGISTER_WMESSAGE(a) a,
#undef REGISTER_WMESSAGE_EX
#define REGISTER_WMESSAGE_EX(a, value) a = value,
#undef END_WMESSAGES
#define END_WMESSAGES };

#include "Comm/wMsg.h"

#undef BEGIN_WMESSAGES
#undef REGISTER_WMESSAGE
#undef REGISTER_WMESSAGE_EX
#undef END_WMESSAGES

#endif __wMsg__
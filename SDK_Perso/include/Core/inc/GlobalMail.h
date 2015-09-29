#ifndef GLOBAL_MAIL_H
#define GLOBAL_MAIL_H


#include "Mail/Registry.h"
#include "Mail/GenMessage.h"
#include "Mail/GenHandler.h"


namespace Mail {

enum FlagCategory {
	Reliable = 0x8000,
	Unreliable = 0,

	Immediate = 0x4000,
	Normal = 0,

	// obsolete
	System = 0,
	Urgent = 0,
	Broadcast = 0,

// the end.
	FlagsMask = 0xF000
};

} // Mail namespace

extern ED_MAIL_EXTERN Mail::Server *globalMail;


#endif /* GLOBAL_MAIL_H */

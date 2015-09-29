#ifndef __woATCMail_h__
#define __woATCMail_h__


#include "woATC.h"


struct woATCMail {
		woATCMail();
		~woATCMail();
	
	static void openDoor(woATC* atc, ObjectID client);
	static void closeDoor(woATC* atc, ObjectID client);
	static void	landing(woATC* atc, ObjectID client, unsigned char wstype);
	static void	takeOff(woATC* atc, ObjectID client);
	static void	glideLock(woATC* atc, bool occupy);
};

#endif /* __woATCMail_h__ */

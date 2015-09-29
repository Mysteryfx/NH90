#ifndef NET_SMOKE_MARKER
#define NET_SMOKE_MARKER

#include "Transport.h"
#include "cLinear.h"
#include "wActivityManager.h"

class AutoSmokeMarker;

class WORLDGENERAL_API AutoSmokeMarkerManager
{
	AutoSmokeMarkerManager();
	static AutoSmokeMarkerManager * instance();
	unsigned short    counter;
public:
	static void		register_mail();
	static unsigned createSmoke				   (float smokePower,const cVector & position, const E4::Vector & cColor, double duration);
    static void		deleteSmoke	  (unsigned id);
    static bool		checkSmoke	  (unsigned id);
    static void		netCreateSmoke(unsigned id, float smokePower,const cVector & position, const E4::Vector & cColor, double duration);
	static void		netDeleteSmoke(unsigned id);
};

#endif // !NET_SMOKE_MARKER

#ifndef __MISSION_EXCEPTION_H__
#define __MISSION_EXCEPTION_H__

#include "Exception.h"

namespace Mission
{

// Если что-то идет не так в миссии или в треке, бросается это исключение
class MissionException : public Exception
{
public:
	MissionException(const ed::string& message) : Exception(message) {}
};

}


#endif
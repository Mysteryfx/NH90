#pragma once
#include "WorldGeneral.h"
namespace Mail 
{
	class Message;
}

WORLDGENERAL_API bool isWorldMsg(const Mail::Message& msg);
WORLDGENERAL_API void writeWorldTrackMsg(const Mail::Message& msg);
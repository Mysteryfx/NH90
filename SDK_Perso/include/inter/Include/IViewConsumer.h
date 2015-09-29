#ifndef IViewConsumer_H
#define IViewConsumer_H

#include "osg/Matrixd"
#include "Lua/Config.h"
#include "binObjectInfo.h"

struct lua_State;
// IViewConsumer
// интерфейс для внешних камер
struct IViewConsumer
{
	virtual bool init(Lua::Config& c)=0;
	virtual bool exit()=0;
	virtual void frame(const binWorldInfo &mWorldInfo)=0;
};

#endif
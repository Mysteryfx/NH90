#ifndef IConsole_h
#define IConsole_h

#include "Inter.h"
#include <ed/string.h>

struct lua_State;

class INTER_API IConsole
{
public:
	enum Env { ENV_MAIN, ENV_MISSION };
	virtual ~IConsole() {;}
	virtual void setEnv(Env env) = 0;
	virtual void toggle() = 0;
	virtual void toggle(bool on) = 0;
	virtual void in(const ed::string & cmd) = 0;
	virtual void out(const ed::string & str) = 0;
	virtual void clear() = 0;
	virtual void clearHistory() = 0;
	virtual void setPos(float x, float y) = 0;
	virtual bool visible() const = 0;
};

extern INTER_API IConsole * globalConsole;

#endif
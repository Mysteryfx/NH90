#ifndef	__ZEROONEXIT_H_
#define	__ZEROONEXIT_H_

#include <ed/string.h>

class ZeroOnExitStr
{
protected:
	ed::string* _variable;

public:
	ZeroOnExitStr(ed::string& variable){_variable = &variable;}
	virtual ~ZeroOnExitStr(){_variable->erase();}
};

#endif	//	__ZEROONEXIT_H__
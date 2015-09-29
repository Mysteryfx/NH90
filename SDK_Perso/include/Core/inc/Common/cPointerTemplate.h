#ifndef __cPointerTemplate_h__
#define __cPointerTemplate_h__

#include "Common/Link.h"
#include "Common.h"

template <class T>
class cPointerTemplate : public Link<T>
{
public:
	cPointerTemplate() {}
	cPointerTemplate(const cPointerTemplate &source) : Link<T>(source) {}
    cPointerTemplate(LinkHost *host) {Set(host);}
	cPointerTemplate(T &host) : Link<T>(&host) {}

	T *	const	Obj(void) const			{ return static_cast<T *>(host); }
	T *			Obj(void) {return static_cast<T *>(host);}
};

#endif

#ifndef __Printer__
#define __Printer__

#include "_config.h"

namespace Common {

class Printer
{
public:
	ED_CORE_EXTERN virtual ~Printer();
	ED_CORE_EXTERN virtual void reg();
	ED_CORE_EXTERN virtual void unreg();
	virtual const char * getName() const = 0;
	virtual const char * print() = 0;
	class Manager
	{
	public:
		virtual void addPrinter(Printer * pPrinter) = 0;
		virtual void removePrinter(Printer * pPrinter) = 0;
	};
	ED_CORE_EXTERN static void setGlobalManager(Manager * pGlobalManager_);
private:
	static Manager * pGlobalManager;
};

#include <ed/vector.h>

template<class T, int buffSize>
class ObjectPrinter : public Printer
{
public:
	ObjectPrinter() : ptr(NULL) {;}
	ObjectPrinter(T * ptr_) : ptr(ptr_) {;}
	void setPtr(T * ptr_)
	{
		ptr = ptr_;
	}
	virtual void reg()
	{
		buff.resize(buffSize);
		Printer::reg();
	}
	virtual void unreg()
	{
		buff.clear();
		Printer::unreg();
	}
	virtual const char * getName() const
	{
		return ptr != NULL ? ptr->name.c_str() : "";
	}
	virtual const char * print()
	{
		ptr->print(&buff[0], buff.size() - 1);
		return &buff[0];
	}
private:
	T * ptr;
	ed::vector<char> buff;
};

template<class T>
Printer * getPrinter(T * t)
{
	return t->getPrinter();
}

}

#endif __Printer__

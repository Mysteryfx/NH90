#ifndef __EXCEPTION_BASE_H__
#define __EXCEPTION_BASE_H__

#include <ed/string.h>
#include <ed/sstream.h>

class Exception
{
public:
	explicit Exception(const ed::string& message):message_(message){}
	explicit Exception(const ed::string&& message):message_(message){}
	const ed::string& getMessage() const { return message_; }

private:
	ed::string message_;
};

#define SIMPLE_EXCEPTION(name)\
	class name : public Exception\
	{ \
	public: \
		explicit name(const ed::string& message) : Exception(message) {} \
	}

#define SIMPLE_SUBEXCEPTION(name, superclass)\
	class name : public superclass \
	{ \
	public: \
		explicit name(const ed::string& message) : superclass(message) {} \
	}

//#define FMT(x) (((ed::ostringstream&)((ed::ostringstream&)(ed::ostringstream()) << x)).str())
#define FMT(x) ((ed::ostringstream&)(ed::ostringstream() << x)).str()

#endif

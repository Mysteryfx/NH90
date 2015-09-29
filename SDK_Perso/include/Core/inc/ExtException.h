#ifndef __EXT_EXCEPTION_H__
#define __EXT_EXCEPTION_H__

#include "exception.h"

namespace ed {

/// Class to throw in case of emergency.
class ExtException: public Exception{
private:
	ed::string fileName; /// Name of file where shit happend.
	int line; /// Number of line where shit happend.
public:
	/// Create exception instance
	ExtException(const ed::string &message,const ed::string &fileName,int line) : Exception(message),fileName(fileName),line(line){ }

	virtual ~ExtException() throw() { }
public:
	/// Returns error message as c++ string
	virtual const ed::string& getReason() const { return getMessage(); }

	///Returns file name;
	const ed::string& getFileName()const{return fileName;}

	///Returns line number;
	int getLine()const{return line;}

	/// Returns error mesage as c string
	virtual const char *what() const throw() { return getMessage().c_str(); }
};


}


#endif

#ifndef __wMorzeString__
#define __wMorzeString__

#include "WRadio.h"
#include <ed/string.h>
#include <ed/vector.h>

typedef ed::vector<bool> WRADIO_API MorzeStringPositions;

class wMorzeString : public ed::string
{
public:
	wMorzeString(void) : ed::string()						 {}
	wMorzeString(const wMorzeString & str):ed::string(str)  {}
	wMorzeString(const char *c_str)							 {*this = c_str;}
	wMorzeString(const ed::string & str)					 {*this = str;}
   ~wMorzeString(void) {}
	wMorzeString & operator=(const wMorzeString & _Right)  {	assign(_Right);	return *this; }
	WRADIO_API wMorzeString & operator=(const char * _Right);
	wMorzeString & operator=(const ed::string & _Right)
	{
		*this = _Right.c_str();
		return *this;
	}	
	size_t get_length_in_dots() const {return signal.size();}	
	WRADIO_API bool check_signal_in_position(unsigned int position) const;
private:
	MorzeStringPositions signal;
};

#endif
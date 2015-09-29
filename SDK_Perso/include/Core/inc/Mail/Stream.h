/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_STREAM_H
#define MAIL_STREAM_H


#include "./_config.h"
#include "./Message.h"
#include <ed/string.h>


namespace Mail {

/**
 * I/O unit type
 */
typedef unsigned int unit_t;

/**
 * bit count type
 */
typedef unsigned char count_t;

/**
 * bit accumulator type
 */
typedef unsigned int accum_t;


/**
 * Base bitstream class.
 */
class Stream {
public:
	enum Mode { Read, Write };

	ED_MAIL_EXTERN virtual ~Stream() {}

	// get stream mode
	Mode mode() const { return mode_; }

	Message& message() { return data_; }
	const Message& message() const { return data_; }


/* Serializer-compatible interface */
	bool isLoading() const { return mode() == Read; }

	// special-case
	Stream& operator >> (bool& t) { stream(&t, 1); return *this; }

#define _MAIL_IO(T) Stream& operator >> (T& t) { stream(&t, 8*sizeof(T)); return *this; }
	_MAIL_IO(char)
//	_MAIL_IO(wchar_t)

	_MAIL_IO(signed char)
	_MAIL_IO(signed short)
	_MAIL_IO(signed int)
	//_MAIL_IO(signed long)

	_MAIL_IO(unsigned char)
	_MAIL_IO(unsigned short)
	_MAIL_IO(unsigned int)
	//_MAIL_IO(unsigned long)

	_MAIL_IO(float)
	_MAIL_IO(double)
	//_MAIL_IO(long double)
#undef _MAIL_IO

protected:
		Stream(Mode m, const Message& d) : mode_(m), data_(d) {}

	/**
	 * Streamify 'size'-bit value at 'data
	 * @param data pointer to value
	 * @param size size of type in bits
	 */
	ED_MAIL_EXTERN virtual void stream(void* data, int size) = 0;

private:
	Mode mode_; // stream mode
	Message data_;

}; // class Stream


class Writer;
class Reader;


/**
 * Helper struct, since MSVC generates only ONE (first used) body for all N values!!!
 * i.e.
 * if we have:
 * template<int N> int function() { return N; }
 * int a = function<0>();
 * int b = function<1>(); - returns ZERO!!!! because MSVC actually calls function<0>()
 * 
 */
template<int N>
struct IO {

/**
 * Get value with N lower bits set to 1.
 */
static inline
unit_t mask() { return (1<<N) - 1; }


/**
 * Read N bits from stream (N<=32).
 * properly reads and masks values.
 */
static
unit_t get(Reader& s);

/**
 * Mask and put to stream N-bit value (N<=32)
 */
static
void put(Writer& s, unit_t v);

}; // struct IO<N>


inline
Stream& operator >> (Stream& s, Info_t& i)
{
	if( s.mode()==Stream::Read )
		i = s.message()->info();
	return s;
}

ED_MAIL_EXTERN
Stream& operator >> (Stream& s, ed::string& str);

} // Mail namespace

#endif /* MAIL_STREAM_H */

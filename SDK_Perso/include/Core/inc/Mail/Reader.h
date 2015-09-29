/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_READER_H
#define MAIL_READER_H


#include "./_config.h"
#include "./Stream.h"
#include "./ByteReader.h"
#include "./VarArgs.h"


namespace Mail {


class Reader : public Stream, private ByteReader {
protected:
	/**
	 * Read 'size' bits to 'data'.
	 */
	ED_MAIL_EXTERN virtual void stream(void* data, int size);

public:
		ED_MAIL_EXTERN Reader();
		ED_MAIL_EXTERN Reader(const VarNil&);
		ED_MAIL_EXTERN Reader(const Message& msg);
		ED_MAIL_EXTERN ~Reader();

	/**
	 * Read count_t bits from stream (count_t <= 24)
	 */
	ED_MAIL_EXTERN unit_t get(count_t);
	size_t read(void* buf, size_t len) { flush(); return getData(buf, len); }
	size_t avail() const { return ByteReader::avail(); }

	bool end() { return fill==0 && ByteReader::end(); }
	void restart() { init(); }

	operator bool() { return !end(); }

	void getBuf(const void* &ptr, size_t &len) { flush(); ByteReader::getBuf(ptr, len); }
	void skip(size_t sz) { ByteReader::skip(sz); }

	ED_MAIL_EXTERN double readFixed(int wholeBits, int fracBits);
	ED_MAIL_EXTERN double readFixed(int fracBits);
	ED_MAIL_EXTERN float readHalf();

	using ByteReader::getRemainingData;

	void flush() { fill=0; }

private:
	unit_t buf;
	count_t fill;

	ED_MAIL_EXTERN void init();
}; // class Reader

/*
inline
Reader& operator >> (Reader& r, Reader& r2)
{
	r2 = r;
	return r;
}
*/

inline
Stream& operator >> (Stream& s, Reader& r)
{
	if( s.mode()==Stream::Read ) {
		r = static_cast<Reader&>(s);
		r.flush();
	}
	return s;
}


} // Mail namespace

#endif /* MAIL_READER_H */

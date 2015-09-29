/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_WRITER_H
#define MAIL_WRITER_H


#include "./Stream.h"
#include "./ByteWriter.h"


namespace Mail {


class Writer : public Stream, private ByteWriter {
protected:
	/**
	 * Write 'size' bits of 'data' to stream.
	 */
	ED_MAIL_EXTERN void stream(void* data, int size);

public:
		ED_MAIL_EXTERN Writer(const Message& msg);
		ED_MAIL_EXTERN ~Writer();

	/**
	 * Put count_t bit value to stream (count_t <=24)
	 */
	ED_MAIL_EXTERN void put(unit_t, count_t);
	void write(const void* buf, size_t len) { flush(); ByteWriter::putData(buf, len); }

	void getBuf(void* &ptr, size_t &len) { flush(); ByteWriter::getBuf(ptr, len); }
	void skip(size_t sz) { ByteWriter::skip(sz); }

	ED_MAIL_EXTERN void writeFixed(double v, int wholeBits, int fracBits);
	ED_MAIL_EXTERN void writeFixed(double v, int fracBits);
	ED_MAIL_EXTERN void writeHalf(float);

private:
	unit_t buf;
	count_t fill;

	ED_MAIL_EXTERN void flush();

}; // class Writer


template<class T> inline
Writer& operator << (Writer& w, const T& t)
{
	w >> const_cast<T&>(t);
	return w;
}


} // Mail namespace

#endif /* MAIL_WRITER_H */

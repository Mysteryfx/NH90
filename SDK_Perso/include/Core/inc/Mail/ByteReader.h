/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_ByteReader_h_
#define _ED_Mail_ByteReader_h_


#include "./_config.h"
#include "./Chunk.h"


namespace Mail {


class ByteReader {
public:
		ED_MAIL_EXTERN ByteReader();
		ED_MAIL_EXTERN ByteReader(const ChunkPtr& data);
		ED_MAIL_EXTERN ~ByteReader();

	ED_MAIL_EXTERN bool init(const ChunkPtr& data);
	ED_MAIL_EXTERN void add(const ChunkPtr& data);

	char getByte() { return prepare() ? *m_pos++ : 0; }
	ED_MAIL_EXTERN int getInt(size_t sz);
	ED_MAIL_EXTERN size_t getData(void*, size_t);
	ED_MAIL_EXTERN ChunkPtr getData(size_t);

	size_t avail() const { return (m_end - m_pos) + m_avail; }
	bool end() { return !prepare(); }
	operator bool() { return !end(); }

	void getBuf(const void* &ptr, size_t &len) { prepare(); ptr = m_pos; len = m_end - m_pos; }
	void skip(size_t sz) { m_pos += sz; if(m_pos > m_end) m_pos = m_end; }

	ED_MAIL_EXTERN ChunkPtr getRemainingData();

private:
	ChunkPtr m_head;
	ChunkPtr m_tail;
	BufferPtr m_buf;
	char* m_pos;
	char* m_end;
	size_t m_avail;

	ChunkPtr tryGetData(size_t len);

	bool prepare() { return (m_pos == m_end) ? init(m_head) : true; }

};

}


#endif /* _ED_Mail_ByteReader_h_ */

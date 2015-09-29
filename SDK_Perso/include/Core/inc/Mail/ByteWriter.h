/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_ByteWriter_h_
#define _ED_Mail_ByteWriter_h_


#include "./_config.h"
#include "./Chunk.h"


namespace Mail {


class ByteWriter {
public:
		ED_MAIL_EXTERN ByteWriter();
		ED_MAIL_EXTERN ~ByteWriter();

	ByteWriter& putByte(char c) { prepare(); *m_pos++ = c; return *this; }
	ED_MAIL_EXTERN ByteWriter& putInt(int i, size_t len);
	ED_MAIL_EXTERN ByteWriter& putData(const void*, size_t);

	ED_MAIL_EXTERN ChunkPtr end();

	void getBuf(void* &ptr, size_t &len) { prepare(); ptr = m_pos; len = m_end - m_pos; }
	void skip(size_t sz) { m_pos += sz; if(m_pos > m_end) m_pos = m_end; }

private:
	ChunkPtr m_head;
	ChunkPtr m_tail;
	BufferPtr m_buf;
	char* m_begin;
	char* m_pos;
	char* m_end;

	void init();
	ED_MAIL_EXTERN void prepare();

	class Cache;
	static Cache& cache();

};

}

#endif /* _ED_Mail_ByteWriter_h_ */

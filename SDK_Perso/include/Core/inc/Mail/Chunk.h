/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_Chunk_h_
#define _ED_Mail_Chunk_h_


#include "./_config.h"
#include "Mem/Shared.h"
#include "./Buffer.h"



namespace Mail {

class Chunk;
typedef Mem::Ptr<Chunk> ChunkPtr;

class Chunk : public Mem::SharedLite {
public:
	ED_MAIL_EXTERN static ChunkPtr alloc(size_t len);
	ED_MAIL_EXTERN static ChunkPtr try_alloc(size_t len);

		Chunk()
			: m_next(0), m_buf(0), m_begin(0), m_end(0) {}

		Chunk(const Chunk& c)
			: m_next(0), m_buf(c.m_buf), m_begin(c.m_begin), m_end(c.m_end) {}

		Chunk(const BufferPtr& buf)
			: m_next(0), m_buf(buf), m_begin(buf->begin()), m_end(buf->end()) {}

		Chunk(const BufferPtr& buf, char* begin, char* end)
			: m_next(0), m_buf(buf), m_begin(begin), m_end(end) {}

	ChunkPtr next() const { return m_next; }

	BufferPtr buffer() const { return m_buf; }
	char* begin() const { return m_begin; }
	char* end() const { return m_end; }
	size_t size() const { return m_end - m_begin; }

	ChunkPtr before(size_t pos) const
	{
		char* splitPos = m_begin + pos;
		return (splitPos > m_end) ? new Chunk(m_buf, m_begin, m_end) : 
			( splitPos > m_begin ) ? new Chunk(m_buf, m_begin, splitPos) : 0;
	}

	ChunkPtr after(size_t pos) const
	{
		char* splitPos = m_begin + pos;
		return ( splitPos < m_end ) ? new Chunk(m_buf, splitPos, m_end) : 0;
	}

	// chain function

	void link(const ChunkPtr& next) { m_next = next; }

	ChunkPtr chain_tail() { return rtail(); }
	size_t chain_size() const { return rsize(); }
	unsigned chain_count() const { return rcount(); }
	void chain_link(const ChunkPtr& next) { rlink(next); }

	ChunkPtr chain_before(size_t pos) { return rbefore(pos); }
	ChunkPtr chain_after(size_t pos) { return rafter(pos); }

protected:
	ChunkPtr m_next;
	BufferPtr m_buf;
	char* m_begin;
	char* m_end;

	// we have class-level lock, so we can lock once and then use this light funcs

	ChunkPtr rtail() { return ( !m_next ) ? this : m_next->rtail(); }
	size_t rsize() const { size_t sz = size(); return (!m_next) ? sz : sz + m_next->rsize(); }
	unsigned rcount() const { return (!m_next) ? 1 : 1 + m_next->rcount(); }
	void rlink(const ChunkPtr& next) { if( !m_next ) m_next = next; else m_next->rlink(next); }

	ED_MAIL_EXTERN ChunkPtr rbefore(size_t pos) const;
	ED_MAIL_EXTERN ChunkPtr rafter(size_t pos) const;

public:
    void* operator new(size_t) { return mem_alloc(ALLOC_CHUNK, sizeof(Chunk)); }
    void operator delete(void* p) { mem_free(ALLOC_CHUNK, p); }
};

} // namespace Mail

#endif /* _ED_Mail_Chunk_h_ */

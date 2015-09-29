/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_MESSAGE_H
#define MAIL_MESSAGE_H


#include "./_config.h"
#include "Mem/Shared.h"
#include "./_memory.h"
#include "./Types.h"
#include "./Chunk.h"
#include <assert.h>


namespace Mail {


class MsgImpl : public Mem::SharedLite {
public:
		MsgImpl(ID_t id, Flags_t flags) : m_id(id), m_flags(flags), m_size(0) {}

	ID_t id() const { return m_id; }
	Flags_t flags() const { return m_flags; }
	size_t size() const { return m_size; }
	ChunkPtr data() const { return m_head; };

	Info& info() { return m_info; }
	const Info& info() const { return m_info; }

	void add(const ChunkPtr& ch)
	{
		if( !ch )
			return;

        size_t c_size = ch->chain_size();
        if (m_size + c_size > 65535)
        {
            assert (0 && "Maximum message size is 64KB. Message truncated");
            return;
        };

		if( !m_head )
		{
			m_head = ch;
		}
		else
		{
			m_tail->link(ch);
		}
		m_tail = ch->chain_tail();
		m_size += ch->chain_size();
	}

private:
	ID_t m_id;
	Flags_t m_flags;
	size_t m_size;
	Info m_info;
	ChunkPtr m_head;
	ChunkPtr m_tail;
public:
    void* operator new(size_t) { return mem_alloc(ALLOC_MESSAGE, sizeof(MsgImpl)); }
    void operator delete(void* p) { mem_free(ALLOC_MESSAGE, p); }
};


class Message : public Mem::Ptr<MsgImpl> {
public:
		Message() {}
		Message(ID_t id, Flags_t flags) : Mem::Ptr<MsgImpl>(new MsgImpl(id, flags)) {}
};


} // Mail namespace

#endif /* MAIL_MESSAGE_H */

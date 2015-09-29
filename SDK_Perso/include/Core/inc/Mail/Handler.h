/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_HANDLER_H
#define MAIL_HANDLER_H

#include "./_config.h"
#include "Mem/Shared.h"
#include "./Message.h"


namespace Mail {

class ED_MAIL_INTERFACE Handler : public Mem::SharedLite {
public:
		Handler(ID_t i, Flags_t f, const char* name="")
			: m_id(i), m_flags(f), m_name(name) {}
	virtual ~Handler(){}

	ID_t id() const { return m_id; }
	Flags_t flags() const { return m_flags; }
	const char* name() const { return m_name; }

	virtual bool handle(const Message&) = 0;

private:
	ID_t m_id;
	Flags_t m_flags;
	const char* m_name;

}; // class Handler

typedef Mem::Ptr<Handler> HandlerPtr;

} // Mail namespace

#endif /* MAIL_HANDLER_H */

/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_Registry_h_
#define _ED_Mail_Registry_h_


#include "./_config.h"
#include "./Server.h"
#include <ed/vector.h>


namespace Mail {


class ED_MAIL_INTERFACE Registry {
public:
	Mail::Server* m_srv;
	ed::vector<Mail::ID_t> m_id;

public:
		Registry(Mail::Server* srv) : m_srv(srv) {}
		~Registry() { for(int i=0; i<(int)m_id.size(); ++i) m_srv->unreg(m_id[i]); }
	Registry& operator()(Mail::HandlerPtr h) { m_srv->reg(h); m_id.push_back(h->id()); return *this; }

}; // class Registry


} // Mail namespace

#endif /* _ED_Mail_Registry_h_ */

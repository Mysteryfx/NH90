/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_SERVER_IMPL_H
#define MAIL_SERVER_IMPL_H


#include "./Server.h"
#include <ed/vector.h>
#include <ed/map.h>
#include <ed/mutex.h>


namespace Mail {

#ifndef DEMO_VERSION

class ServerImpl : public Server {
    typedef ed::mutex Guard_t;
    mutable Guard_t _guard;
public:
		ServerImpl(ID_t split=256);
	virtual ~ServerImpl();

	virtual Server& reg( const HandlerPtr& );
	virtual Server& unreg( ID_t );
	virtual bool proc( const Message& ) const;
	virtual HandlerPtr find( ID_t ) const;

private:
	typedef ed::vector<HandlerPtr> HandlerVec;
	typedef ed::map<ID_t, HandlerPtr> HandlerMap;

	ID_t m_split;
	HandlerVec m_low;
	HandlerMap m_rest;

}; // class ServerImpl

#else

class ServerImpl : public Server {
public:
		ServerImpl(ID_t split=256) {}
	virtual ~ServerImpl() {}

	virtual Server& reg( const HandlerPtr& ) { return *this; }
	virtual Server& unreg( ID_t ) { return *this; }
	virtual bool proc( const Message& ) { return false; }
	virtual HandlerPtr find( ID_t ) { return 0; }

}; // class ServerImpl

#endif

} // Mail namespace

#endif /* MAIL_SERVER_IMPL_H */

/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_Server_h_
#define _ED_Mail_Server_h_


#include "./_config.h"
#include "./Types.h"
#include "./Handler.h"
#include <memory>


namespace Mail {


class ED_MAIL_INTERFACE Server {
public:
	virtual ~Server(){}

	virtual Server& reg( const HandlerPtr& ) = 0;
	virtual Server& unreg( ID_t ) = 0;
	virtual bool proc( const Message& ) const = 0;
	virtual HandlerPtr find( ID_t ) const = 0;

}; // class Server


} // Mail namespace

#endif /* _ED_Mail_Server_h_ */

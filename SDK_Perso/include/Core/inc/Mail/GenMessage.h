/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_GEN_MESSAGE_H
#define MAIL_GEN_MESSAGE_H


#include "./Message.h"
#include "./Writer.h"
#include "./VarArgs.h"


namespace Mail {

typedef VarNil Nil;

/**
 * Message type generator.
 */
template<ID_t I_, Flags_t F_, class A1=Nil, class A2=Nil, class A3=Nil, class A4=Nil, class A5=Nil, class A6=Nil, class A7=Nil, class A8=Nil, class A9=Nil>
class GenMessage : public Message {
public:
	typedef typename VarArgs<A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type Args;
	enum { ID = I_ };
	enum { Flags = F_ };

	GenMessage(const A1& a1=Nil(), const A2& a2=Nil(), const A3& a3=Nil(), const A4& a4=Nil(), const A5& a5=Nil(), const A6& a6=Nil(), const A7& a7=Nil(), const A8& a8=Nil(), const A9& a9=Nil())
		: Message(I_, F_) { Writer wr(*this); wr << a1 << a2 << a3 << a4 << a5 << a6 << a7 << a8 << a9; }
};


} // Mail namespace

#endif /* MAIL_GEN_MESSAGE_H */

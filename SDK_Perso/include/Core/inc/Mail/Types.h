/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef MAIL_TYPES_H
#define MAIL_TYPES_H


#include "./_config.h"


namespace Mail {

struct VarNil;


typedef unsigned int ID_t;
typedef unsigned int Flags_t;
typedef unsigned int Host_t;

struct Info {
	Info() : host(0) {}
	Info(const VarNil&) : host(0), time(0) {}

	Host_t host;
    double time;
};
typedef Info Info_t;


} // Mail namespace

#endif /* MAIL_TYPES_H */

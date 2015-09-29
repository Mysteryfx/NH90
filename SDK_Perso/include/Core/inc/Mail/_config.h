/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_config_h_
#define _ED_Mail_config_h_


#include "../_config.h"


#ifndef ED_MAIL_EXTERN
#define ED_MAIL_EXTERN ED_CORE_EXTERN
#endif


#ifndef ED_MAIL_INTERFACE
#define ED_MAIL_INTERFACE ED_CORE_INTERFACE
#endif


#ifndef ED_MAIL_API
#define ED_MAIL_API ED_CORE_API
#endif


namespace Var {}

namespace Mail {

	using namespace Var;

} // namespace Mail


#endif /* _ED_Mail_config_h_ */

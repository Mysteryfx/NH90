/**
 * This file is a part of
 * Mail library version 0.5
 * (C)opyright 2002,2003 Dmitry S. Baikov. All rights reserved.
 *
 * Licensed under GNU Lesser General Public License version 2.1 or later.
 * For details see file LICENSE or http://www.gnu.org/copyleft/lesser.html
 */
#ifndef _ED_Mail_Buffer_h_
#define _ED_Mail_Buffer_h_


#include "./_config.h"
#include "Mem/Shared.h"
#include "./_memory.h"

namespace Mail {

class Buffer : public Mem::SharedLite {
public:
        ED_MAIL_EXTERN Buffer() {}
        ED_MAIL_EXTERN ~Buffer() {}

	char* begin() { return &data[0]; }
	char* end() { return begin() + sizeof(data); }

    void* operator new(size_t sz) { return mem_alloc(ALLOC_BUFFER, sizeof(Buffer)); }
    void operator delete(void* p) { mem_free(ALLOC_BUFFER, p); }

private:
    char data[64];
};

typedef Mem::Ptr<Buffer> BufferPtr;

} // Mail namespace

#endif /* _ED_Mail_Buffer_h_ */

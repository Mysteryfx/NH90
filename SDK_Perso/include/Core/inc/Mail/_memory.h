#ifndef _ED_Mail_memory_h_
#define _ED_Mail_memory_h_

#include "./_config.h"

namespace Mail {

ED_MAIL_EXTERN
void init();

ED_MAIL_EXTERN
void exit();

enum AllocType {
    ALLOC_BUFFER,
    ALLOC_CHUNK,
    ALLOC_MESSAGE,

    NUM_ALLOC_TYPES
};

ED_MAIL_EXTERN
void* mem_alloc(AllocType t, size_t sz);

ED_MAIL_EXTERN
void mem_free(AllocType t, void* ptr);

}

#endif /* _ED_Mail_memory_h_ */

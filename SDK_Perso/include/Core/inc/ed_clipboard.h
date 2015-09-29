#ifndef _ED_CLIPBOARD_H_
#define _ED_CLIPBOARD_H_

#include "./_config.h"
#include "ed/string.h"


ED_CORE_EXTERN
ed::string getClipboardText();

ED_CORE_EXTERN
void setClipboardText(const ed::string& text);


#endif /* _ED_CLIPBOARD_H_ */

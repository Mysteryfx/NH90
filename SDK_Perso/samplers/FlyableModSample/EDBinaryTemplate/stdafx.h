// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Windows Header Files:
#include <windows.h>
#include <xutility>
#include <ed_sdk_version.h>

#if (ED_SDK_MAJOR > 1 && _MSC_VER < 1800) || (ED_SDK_MAJOR < 2 && (_MSC_VER < 1700 || _MSC_VER > 1799))
#define  MACROPRINT(x) #x
#define  MACROPRINT2(x) MACROPRINT(x)
#pragma message("MSC_VER " MACROPRINT2(_MSC_VER))
#pragma message("ED_SDK_MAJOR " MACROPRINT2(ED_SDK_MAJOR))
#undef   MACROPRINT
#undef   MACROPRINT2
#error unsupported compiler for this ED SDK VERSION
#endif

// TODO: reference additional headers your program requires here
#include "debuggueur.h"
#include "Utilitaire.h"

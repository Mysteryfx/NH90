// === Copyright 2014, Eagle Dynamics, All Rights Reserved ===
/*
 * ED API common definitions.
 */
#pragma once

#define ED_CALLTYPE __cdecl

#if defined(_WIN32)
    #if defined(ED_API_EXPORTS)
    #define ED_EXTERN extern "C" __declspec(dllexport)
    #elif defined(ED_API_STATIC) // why not?
    #define ED_EXTERN extern "C"
    #else
    #define ED_EXTERN extern "C" __declspec(dllimport)
    #endif
#else
#error Only Win32 is supported for now.
#endif

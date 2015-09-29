#ifndef _ED_Core_config_h_
#define _ED_Core_config_h_

#ifdef _WINDOWS
#ifndef ED_CORE_STATIC
    #ifndef ED_CORE_EXPORT
    #define ED_CORE_EXPORT __declspec(dllexport)
    #endif
#else
    #ifndef ED_CORE_EXPORT
    #define ED_CORE_EXPORT
    #endif
#endif

    #ifndef ED_CORE_IMPORT
    #define ED_CORE_IMPORT __declspec(dllimport)
    #endif

    #ifndef ED_CORE_INTERFACE
    #define ED_CORE_INTERFACE __declspec(novtable)
    #endif

    #ifndef ED_CORE_API
    #define ED_CORE_API __stdcall
    #endif

#else

    #ifndef ED_CORE_EXPORT
    #define ED_CORE_EXPORT
    #endif

    #ifndef ED_CORE_IMPORT
    #define ED_CORE_IMPORT
    #endif

    #ifndef ED_CORE_INTERFACE
    #define ED_CORE_INTERFACE
    #endif

    #ifndef ED_CORE_API
    #define ED_CORE_API
    #endif

#endif

#ifndef ED_CORE_EXTERN
#   ifdef ED_CORE_INTERNALS
#       define ED_CORE_EXTERN ED_CORE_EXPORT
#   else
#       define ED_CORE_EXTERN ED_CORE_IMPORT
#   endif
#endif

#endif /* _ED_Core_config_h_ */

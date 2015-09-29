#ifndef _ed_stl_config_h_
#define _ed_stl_config_h_

#ifndef ED_VA_COPY_ENABLED
    #if defined(__MWERKS__) || (defined(__GNUC__) && (__GNUC__ >= 3) && (!defined(__i386__) || defined(__x86_64__)) && !defined(__ppc__) && !defined(__PPC__) && !defined(__PPC64__))
        #define ED_VA_COPY_ENABLED 1
    #else
        #define ED_VA_COPY_ENABLED 0
    #endif
#endif

#endif /* _ed_stl_config_h_ */

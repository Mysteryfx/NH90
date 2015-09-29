#ifndef __USERINTERFACE_VERTEX_H__
#define __USERINTERFACE_VERTEX_H__

#define COLOR_MAKE(r, g, b, a)   ((unsigned int) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))) 
typedef struct _USERINTERFACEVERTEX
{
    float    sx;
    float    sy;
    float    sz;
    float    rhw;
    unsigned int    color;
    unsigned int    specular;
    float    tu;
    float    tv;

} USERINTERFACEVERTEX, *LPUSERINTERFACEVERTEX;

#endif//__USERINTERFACE_VERTEX_H__

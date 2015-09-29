#ifndef __ParamEvoker_h__
#define __ParamEvoker_h__

#include "Evoker.h"

template <class T, class P>
class ParamEvoker: public Evoker<T> 
{
public:
    void evoke(void (T::*f)(P *), P *p) { Evoker<T>::evoke(f, p); }
};

#endif
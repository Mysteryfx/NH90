#ifndef wListener_H
#define wListener_H

#include "World.h"
#include "Common/Link.h"

class Registered;
class wEmisisonInfo;

class WORLD_API wListener : public LinkHost
{
public:
	virtual ~wListener();
	virtual void onCreate(Registered *);
	virtual void onDestroy(Registered *);
    virtual void onWorldSpawnComplete();
};


template <class T> 
class ObjCreateDestoyDelegate : public wListener
{   
    T                    * owner_;
    void (T::*onCreateF) (Registered*);
    void (T::*onDestroyF)(Registered*);
public:
    ObjCreateDestoyDelegate(T * ptr_,
                            void (T::*onCreate_) (Registered*) = NULL,
                            void (T::*onDestroy_)(Registered*) = NULL)
    {
        owner_ = ptr_;
        onCreateF      = onCreate_;
        onDestroyF     = onDestroy_;
    }
    void onCreate(Registered * obj)
    {
        if (onCreateF != NULL)
            (owner_->*onCreateF)(obj);
    }
    void onDestroy(Registered * obj)
    {
        if (onDestroyF != NULL)
            (owner_->*onDestroyF)(obj);
    }
};



template <class T> 
class WorldSpawnDelegate : public wListener
{   
    T                    * owner_;
    void (T::*onWorldSpawnCompleteF) ();
public:
    WorldSpawnDelegate(T * ptr_,
        void (T::*onWorldSpawnComplete_)())
    {
        owner_               = ptr_;
        onWorldSpawnCompleteF = onWorldSpawnComplete_;
    }
    void onWorldSpawnComplete()
    {
        if (onWorldSpawnCompleteF != NULL)
            (owner_->*onWorldSpawnCompleteF)();
    }
};

#endif // wListener_H

#pragma once
#include "woSnareMailManager.h"
#include "IwNetObject.h"
#include <ed/set.h>

class woSnareMailManagerImpl : public woSnareMailManager, public IwNetObject
{

public:
    woSnareMailManagerImpl();
    ~woSnareMailManagerImpl();

    virtual void start();
    virtual void stop();

    // добавление/удаление объектов из списка
    virtual void addObject(woSnare *obj);
    virtual void removeObject(woSnare *obj);

    // From IwNetObject
    virtual int  netGetHost() const;
    virtual bool netGetCreate(Mail::Message& msg) const;
    //virtual bool netSendUpdate(int dest, int prio, int& cookie);
    //virtual void netDiscard();

    static void processSnaresInit(Mail::Reader rd);

private:

    ed::set<woSnare*>   objects_;

};
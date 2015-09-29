#pragma once
#include "Registry\Registered.h"
#include "wsType.h"
#include "WorldGeneral.h"
#include "wTime.h"
#include "cLinear.h"
#include "woSnare.h"

namespace Mail { class Reader; class Message; }

class woSnareMailManager : public Common::Unknown<Serializable>
{
public:
    // глобальный менеджер
    static WORLDGENERAL_API woSnareMailManager *instance;

    // конструктор/деструктор
    static WORLDGENERAL_API woSnareMailManager *create();
    virtual ~woSnareMailManager() {};

    // Начать работу
    virtual void start() = 0;
    // Закончить работу
    virtual void stop() = 0;

    // добавление/удаление объектов из списка
    virtual void addObject(woSnare* obj) = 0;
    virtual void removeObject(woSnare* obj) = 0;

};

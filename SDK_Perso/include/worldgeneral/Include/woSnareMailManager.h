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
    // ���������� ��������
    static WORLDGENERAL_API woSnareMailManager *instance;

    // �����������/����������
    static WORLDGENERAL_API woSnareMailManager *create();
    virtual ~woSnareMailManager() {};

    // ������ ������
    virtual void start() = 0;
    // ��������� ������
    virtual void stop() = 0;

    // ����������/�������� �������� �� ������
    virtual void addObject(woSnare* obj) = 0;
    virtual void removeObject(woSnare* obj) = 0;

};

#ifndef Lua_Transport_Mission_Functions_h
#define Lua_Transport_Mission_Functions_h

#include "Transport.h"
#include "../../Core/inc/ed/string.h"
#include "CoalitionInfo.h"
#include "../../Math/include/cLinear.h"
#include "../../Core/inc/Mail/GenMessage.h"
#include "WorldMail.h"
#include "../../WorldGeneral/Include/wMovingObject.h"
#include "AI/Controller/Common/Tasks.h"
#include "string.h"

struct lua_State;
class IwoLA;
class wControl;

namespace AI
{
    class Embarking;
    class Mission;
}
//сетевой функционал
class TRANSPORT_API TransportMissionFunctionsMail
{
public:
    typedef Mail::GenMessage<TransportMissionFunctionMsgID+2, Mail::Reliable, ObjectID, ObjectID> MsgEmbarkingStartMoveToTransport;
    typedef Mail::GenMessage<TransportMissionFunctionMsgID+3, Mail::Reliable, ObjectID, ObjectID> MsgDisembarkFromTransport;
    typedef Mail::GenMessage<TransportMissionFunctionMsgID+4, Mail::Reliable, ObjectID, ObjectID> MsgEmergencyDisembarkingContinueMoveFromTransport;
    typedef Mail::GenMessage<TransportMissionFunctionMsgID+5, Mail::Reliable, ObjectID, ObjectID> MsgAddEmbarkingSmokeMapMarker;
    typedef Mail::GenMessage<TransportMissionFunctionMsgID+6, Mail::Reliable, ObjectID, ObjectID, cVector> MsgPlaceGroupToTransport;
    typedef Mail::GenMessage<TransportMissionFunctionMsgID+7, Mail::Reliable, ObjectID, ObjectID> MsgDisplaceGroupFromTransport;
    
    TransportMissionFunctionsMail();
    ~TransportMissionFunctionsMail();

    static void sendEmbarkingStartMoveToTransport(const ObjectID passengerGroupId, const ObjectID transportId);
    static void sendEmbarkingStartMoveToFriendlyTransport(const ObjectID passengerGroupId, const ObjectID transportId);
    static void sendDisembarkFromTransport(const ObjectID passengerGroupId, const ObjectID transportId);
    static void sendAddEmbarkingSmokeMapMarker(const ObjectID passengerGroupId, const ObjectID transportId);
    static void sendPlaceGroupToTransport(ObjectID passengerGroupId, ObjectID transportId, cVector disembarkPoObjectID);
    static void sendDisplaceGroupFromTransport(ObjectID passengerGroupId, ObjectID transportId);
};

//функционал транспортировки
class TRANSPORT_API TransportMissionFunctions
{
public:
    //проверка на необходимость обновить карту в связи с погрузкой/выгрузкой
    static bool isNeedMapUpdate();
    //запрос на обновление карты
    static void mapUpdateIsNeed();
    //взятие оьекта по его id миссии
    static Registered * getObjectByMissionId(char * missionId);
    static Registered * getObjectByMissionId(ObjectID missionId);
    //добовление групп к готовому распределению
    static ed::map<ObjectID,ed::set<ObjectID>> addGroupsToDistribution(ed::map<ObjectID,int> &transports, ed::map<ObjectID,int> &passengers, ed::map<ObjectID,ed::set<ObjectID>> distribution);
    static ed::map<ObjectID,ed::set<ObjectID>> addGroupsToDistribution(ed::vector<ObjectID> &transports, ed::set<ObjectID> &passengers, ed::map<ObjectID,ed::set<ObjectID>> &distribution);
    static ed::map<ObjectID,ed::set<ObjectID>> addGroupsToDistribution(const wControl * transportController, ed::set<ObjectID> &passengers, ed::map<ObjectID,ed::set<ObjectID>> &distribution);
    //удаление лишних групп из распределения
    static void removeExcessGroups(ed::map<ObjectID,ed::set<ObjectID>> * distribution, ed::set<ObjectID> groupsId);    
    //проверка на покрытие группами всех транспортов
    static bool isGroupsCoveraged(ed::map<ObjectID,ed::set<ObjectID>> distribution, ed::set<ObjectID> groupsId);
    //получить транспорт по группе в распределении
    static ObjectID getTransportByGroupFromDistribution(ed::map<ObjectID,ed::set<ObjectID>> distribution, ObjectID groupId);
    //получение юнита управляемого человеком в группе
    static woPointer getHumanInGroup(const wControl * group);
    //проверка на то что группа сейчас внутри транспорта
    static bool checkGroupOnTransport(IwoLA * transport, wControl * group);
	//проверка на то что группа сейчас внутри группы транспорта
	static bool checkGroupOnTransport(wControl * transportGroup, wControl * group);
    //взятие всех задач погрузки транспорта
    static ed::vector<AI::Embarking *> getAllEmbarkingTasks(MovingObject * transport);
    //взятие текущей задачи погрузки
    static AI::Embarking * getCurrentEmbarkingTask(MovingObject * transport);
    //взятие финальной миссии после текущей
    static AI::Mission * getTasksAfterCurrent(wControl * transportController);
    
    
    //фабрика задачи ожидания транспорта, генерирует со стэком
    static AI::Stack factoryEmbarkToTransportWhaitForTransportStack(AI::TaskPtr finalMission,
                                                    cVector embarkPoObjectID,
                                                    float embarkZoneRadius,
                                                    cVector disembarkPoObjectID,
                                                    float disembarkZoneRadius,
                                                    ed::string selectedType,
                                                    ObjectID selectedUnit, 
                                                    ObjectID mapSmokeMarkerId,
                                                    cVector startPoObjectID,
                                                    cVector finishPoObjectID, 
                                                    double speed);
    //фабрика задачи погрузки/перевозки в транспорте, генерирует со стеком
    static AI::Stack factoryEmbarkToTransportLoadToTransportStack(AI::TaskPtr finalMission, 
                                                  cVector embarkPoObjectID, 
                                                  float embarkZoneRadius,
                                                  cVector disembarkPoObjectID,
                                                  float disembarkZoneRadius,
                                                  ed::string selectedType,
                                                  ObjectID selectedUnit,
                                                  ObjectID transportID,
                                                  cVector embarkingTransportPosition,
                                                  ObjectID mapSmokeMarkerId,
                                                  ed::vector<cVector> wayPoObjectIDs, 
                                                  double speed);
    //фабрика создающая здачу ожидания из задачи погрузки
    static AI::Stack factoryFromLoadEmbarkToTransportWhaitForTransportStack(AI::EmbarkToTransportLoadToTransport * task, MovingObject * transport, wControl * group);
    //фабрика задачи погрузки для вертолета без оболочек
    static AI::Embarking * factoryEmbarking(cVector poObjectID,ed::set<ObjectID> groupsId,
                           ed::map<ObjectID,ed::set<ObjectID>> distribution, ObjectID human,
                           bool durationFlag,wModelTime duration,
                           void (*sendCommand)(ObjectID passengerGroupId, ObjectID transportId),
                           AI::TaskPtr finalMission);
    //фабрика задачи погрузки для вертолета окутанной в миссию
    static AI::Mission * factoryEmbarkingMission(AI::Embarking * task, cVector startPoObjectID, cVector finishPoObjectID, double speed);
    
    //добавить пункт в меню десантирования
    static void addDescentMenuPunkt(ObjectID carID);
    //удалить пункт из меню десантирования
    static void removeDescentMenuPunkt(ObjectID carID);
    
    //команда на движение к транспорту с целью посадки в него
    static void embarkingStartMoveToTransport(ObjectID passengerGroupId, ObjectID transportId);
    //команда на движение к дружественному транспорту с целью посадки в него
    static void embarkingStartMoveToFriendlyTransport(ObjectID passengerGroupId, ObjectID transportId);
    //команда на выгрузку из транспорта и продолжение движения
    static void disembarkingContinueMoveFromTransport(ObjectID passengerGroupId, ObjectID transportId);
    //команда экстренной высадки
    static void emergencyDisembarkingContinueMoveFromTransport(ObjectID passengerGroupId, ObjectID transportId);
    //команада обозначения группы маркером
    static void addEmbarkingSmokeMapMarker(ObjectID pasengerGroupId, ObjectID transportId);
    //запустить следующую задачу вертолету после высадки
    static void startTransportNextTask(MovingObject * transport);
    //продолжить движение пассажиров
    static void disembarkFromTransport(ObjectID passengerGroupId, ObjectID transportId);
    //посадка группы с точки зрения вертолета, вызываю из начала задачи LoadToTransport
    static bool placeGroupToTransport(ObjectID passengerGroupId, ObjectID transportId, cVector disembarkPoObjectID);
    //высадка группы с точки зрения вертолета, вызываю из конца задачи LoadToTransport
    static void displaceGroupFromTransport(ObjectID passengerGroupId, ObjectID transportId);
};

//вызываемые функции по сети
class TRANSPORT_API NetTransportMissionFunctions
{
public:
    static void netEmbarkingStartMoveToTransport(ObjectID pasengerGroupId, ObjectID transportId);
    static void netEmbarkingStartMoveToFriendlyTransport(ObjectID pasengerGroupId, ObjectID transportId);
    static void netDisembarkFromTransport(ObjectID pasengerGroupId, ObjectID transportId);
    static void netAddEmbarkingSmokeMapMarker(ObjectID passengerGroupId, ObjectID transportId);
    static void netCreateEmbarkingSmokeMapMarker(ObjectID passengerGroupId, float smokePower, cVector position, E4::Vector cColor);
    static void netDeleteEmbarkingSmokeMapMarker(ObjectID passengerGroupId);
	static void netPlaceGroupToTransport(ObjectID passengerGroupId, ObjectID transportId, cVector disembarkPoObjectID);
	static void netDisplaceGroupFromTransport(ObjectID passengerGroupId, ObjectID transportId);
};

//функции радиоменеджера, для создания радиоменю
class TRANSPORT_API RadioManager
{
public:
	enum command
	{
		EMBARKING_TO_TRANSPORT,
		DISEMBARKING_FROM_TRANSPORT,
		EMERGENCY_DISEMBARKING_FROM_TRANSPORT,
		CREATE_SMOKE_MARKER
	};
private:
	ed::map<ObjectID,void(*)(ObjectID,ObjectID)> _calls;
	ed::set<ObjectID> _descent;
	RadioManager();
	static RadioManager * instance();
	static int luaDescentComand(lua_State * L);
public:
	//инициализация, нужно выолнить 1 раз
	static void init();
	//регистрация группы десанта
	static void addDescent(ObjectID);
	//дерегистрация группы десанта
	static void removeDescent(ObjectID);
	//функция для луа взятия десанта
	static int luaRadioManagerGetDescent(lua_State * L);
};

#endif
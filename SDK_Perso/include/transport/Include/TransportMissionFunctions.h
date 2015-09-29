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
//������� ����������
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

//���������� ���������������
class TRANSPORT_API TransportMissionFunctions
{
public:
    //�������� �� ������������� �������� ����� � ����� � ���������/���������
    static bool isNeedMapUpdate();
    //������ �� ���������� �����
    static void mapUpdateIsNeed();
    //������ ������ �� ��� id ������
    static Registered * getObjectByMissionId(char * missionId);
    static Registered * getObjectByMissionId(ObjectID missionId);
    //���������� ����� � �������� �������������
    static ed::map<ObjectID,ed::set<ObjectID>> addGroupsToDistribution(ed::map<ObjectID,int> &transports, ed::map<ObjectID,int> &passengers, ed::map<ObjectID,ed::set<ObjectID>> distribution);
    static ed::map<ObjectID,ed::set<ObjectID>> addGroupsToDistribution(ed::vector<ObjectID> &transports, ed::set<ObjectID> &passengers, ed::map<ObjectID,ed::set<ObjectID>> &distribution);
    static ed::map<ObjectID,ed::set<ObjectID>> addGroupsToDistribution(const wControl * transportController, ed::set<ObjectID> &passengers, ed::map<ObjectID,ed::set<ObjectID>> &distribution);
    //�������� ������ ����� �� �������������
    static void removeExcessGroups(ed::map<ObjectID,ed::set<ObjectID>> * distribution, ed::set<ObjectID> groupsId);    
    //�������� �� �������� �������� ���� �����������
    static bool isGroupsCoveraged(ed::map<ObjectID,ed::set<ObjectID>> distribution, ed::set<ObjectID> groupsId);
    //�������� ��������� �� ������ � �������������
    static ObjectID getTransportByGroupFromDistribution(ed::map<ObjectID,ed::set<ObjectID>> distribution, ObjectID groupId);
    //��������� ����� ������������ ��������� � ������
    static woPointer getHumanInGroup(const wControl * group);
    //�������� �� �� ��� ������ ������ ������ ����������
    static bool checkGroupOnTransport(IwoLA * transport, wControl * group);
	//�������� �� �� ��� ������ ������ ������ ������ ����������
	static bool checkGroupOnTransport(wControl * transportGroup, wControl * group);
    //������ ���� ����� �������� ����������
    static ed::vector<AI::Embarking *> getAllEmbarkingTasks(MovingObject * transport);
    //������ ������� ������ ��������
    static AI::Embarking * getCurrentEmbarkingTask(MovingObject * transport);
    //������ ��������� ������ ����� �������
    static AI::Mission * getTasksAfterCurrent(wControl * transportController);
    
    
    //������� ������ �������� ����������, ���������� �� ������
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
    //������� ������ ��������/��������� � ����������, ���������� �� ������
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
    //������� ��������� ����� �������� �� ������ ��������
    static AI::Stack factoryFromLoadEmbarkToTransportWhaitForTransportStack(AI::EmbarkToTransportLoadToTransport * task, MovingObject * transport, wControl * group);
    //������� ������ �������� ��� ��������� ��� ��������
    static AI::Embarking * factoryEmbarking(cVector poObjectID,ed::set<ObjectID> groupsId,
                           ed::map<ObjectID,ed::set<ObjectID>> distribution, ObjectID human,
                           bool durationFlag,wModelTime duration,
                           void (*sendCommand)(ObjectID passengerGroupId, ObjectID transportId),
                           AI::TaskPtr finalMission);
    //������� ������ �������� ��� ��������� ��������� � ������
    static AI::Mission * factoryEmbarkingMission(AI::Embarking * task, cVector startPoObjectID, cVector finishPoObjectID, double speed);
    
    //�������� ����� � ���� ��������������
    static void addDescentMenuPunkt(ObjectID carID);
    //������� ����� �� ���� ��������������
    static void removeDescentMenuPunkt(ObjectID carID);
    
    //������� �� �������� � ���������� � ����� ������� � ����
    static void embarkingStartMoveToTransport(ObjectID passengerGroupId, ObjectID transportId);
    //������� �� �������� � �������������� ���������� � ����� ������� � ����
    static void embarkingStartMoveToFriendlyTransport(ObjectID passengerGroupId, ObjectID transportId);
    //������� �� �������� �� ���������� � ����������� ��������
    static void disembarkingContinueMoveFromTransport(ObjectID passengerGroupId, ObjectID transportId);
    //������� ���������� �������
    static void emergencyDisembarkingContinueMoveFromTransport(ObjectID passengerGroupId, ObjectID transportId);
    //�������� ����������� ������ ��������
    static void addEmbarkingSmokeMapMarker(ObjectID pasengerGroupId, ObjectID transportId);
    //��������� ��������� ������ ��������� ����� �������
    static void startTransportNextTask(MovingObject * transport);
    //���������� �������� ����������
    static void disembarkFromTransport(ObjectID passengerGroupId, ObjectID transportId);
    //������� ������ � ����� ������ ���������, ������� �� ������ ������ LoadToTransport
    static bool placeGroupToTransport(ObjectID passengerGroupId, ObjectID transportId, cVector disembarkPoObjectID);
    //������� ������ � ����� ������ ���������, ������� �� ����� ������ LoadToTransport
    static void displaceGroupFromTransport(ObjectID passengerGroupId, ObjectID transportId);
};

//���������� ������� �� ����
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

//������� ��������������, ��� �������� ���������
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
	//�������������, ����� �������� 1 ���
	static void init();
	//����������� ������ �������
	static void addDescent(ObjectID);
	//������������� ������ �������
	static void removeDescent(ObjectID);
	//������� ��� ��� ������ �������
	static int luaRadioManagerGetDescent(lua_State * L);
};

#endif
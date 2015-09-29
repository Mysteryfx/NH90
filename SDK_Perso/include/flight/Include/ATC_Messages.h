#ifndef __ATC_Messages__
#define __ATC_Messages__

//Message from the client to the ATC
enum ATCMessage
{
	DECLARE_INBOUND, //Прилёт
	ABORT_INBOUND,	 //Отмена прилета	
	REQUEST_LANDING, //Запрос посадки
	NOTIFY_BEGIN_LANDING,   //Начало захода на посадку
	NOTIFY_ON_GLIDESLOPE, //Уведомление о занятии глиссады
	ABORT_LANDING,	 //Прерывание захода на посадку
	NOTIFY_LANDING,  //Касание ВПП
	NOTIFY_BEGIN_TAXI_FROM_RUNWAY, //Руление с ВПП на стоянку
	NOTIFY_RUNWAY_FREE, //Покидание ВПП, начало движения по рулежкам
	NOTIFY_ON_PARKING_AREA, //Прибытие на стоянку - завершение прилёта
	NOTIFY_BIRTH_COLD, //Рождение на стоянке в холодном состоянии
	NOTIFY_BIRTH_ON_RAMP_HOT, //Рождение на стоянке в горячем состоянии
	NOTIFY_BIRTH_ON_RUNWAY, //Рождение на ВПП
	NOTIFY_BIRTH_ON_HELIPAD_HOT, //Рождение на верт. площадке в горячем состоянии
	REQUEST_START_UP,	//Готовность запуску двигателей		
	ABORT_START_UP, //Отмена запуска двигателей		
	REQUEST_TAXI_FOR_TO, //Готовность к рулению на полосу		
	NOTIFY_BEGIN_TAXI_FOR_TO, //Начало руления на полосу
	NOTIFY_READY_TO_TAKEOFF, //Вырулил на ВПП, сориентировался по курсу, готов к взлёту
	REQUEST_TAKEOFF, //Запрос начало разбега
	ABORT_TAKEOFF, //Отмена процедуры вылета
	NOTIFY_BEGIN_TAKEOFF,   //Уведомление о начале разбега
	NOTIFY_TAKEOFF,  //Уведомление об отрыве от ВПП
	ABORT,            //Прекращение взаимодействия
	REQUEST_AZIMUTH,  //Прибой
	REQUEST_CONTROL_HOVER //Контрольнное висение
};

enum ClientStatus;

//Client initial state (for synchronization over network)
struct ClientInitState
{
	ClientStatus	status;
	wModelTime		time;
};

typedef ed::map<ObjectID, ClientInitState> ClientsInitState;

//Terminal initial state (for synchronization over network)
struct TerminalState
{
	ObjectID		clientID;
	int				Term_Index;
	int				Term_Index_0;
	bool			TO_AC;
};

typedef ed::vector<TerminalState> TerminalsState;

typedef ed::vector<ObjectID> HelTerminalsState;

#endif __ATC_Messages__
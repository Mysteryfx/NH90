#ifndef avEkranControl_H
#define avEkranControl_H

#include "Serializer/Serializer.h"
#include "Common.h"
#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "wTime.h"

namespace cockpit
{

typedef int  avMsg_ID;
typedef int  avMsg_CatNo;
typedef int  avMsg_priority;

const wModelTime null_time		= -1.0;
const avMsg_ID nullID			= -1;
const avMsg_ID blankID			= 1;
const avMsg_ID selftestID		= 2; // SELFTEST
const avMsg_ID readyID			= 3; // EKRAN READY
const avMsg_ID failureID		= 4; // EKRAN FAILURE
const avMsg_ID flightID			= 5; // FLIGHT
const avMsg_ID documentID		= 6; // DOCUMENT
const avMsg_ID doc_msgID		= 7; // document message

class avEkranControl;

enum avEkranControl_modes
{
	OFF,				// выключен
	NULL_MODE,			// только включили, ничего не нажимали
	SELF_TEST,			// самоконтроль
	GROUND_CONTROL,		// наземный контроль
	FLIGHT_CONTROL_TO,	// полетный контроль - режим взлета
	FLIGHT_CONTROL_FL,	// полетный контроль - режим полетный контроль
	DOCUMENT			// документирование
};

class avEkranMessage
{
public:
	avEkranMessage(){id = 0; time = 0.0; print_time = 0.0;};
	avEkranMessage(avMsg_ID id_in, wModelTime time_in, bool w_time = true)
	{
		id = id_in;
		time = time_in;
		print_time = w_time ? time : null_time;
	};
	avEkranMessage& operator = (const avEkranMessage& msg_in)
	{
		if (this != &msg_in)
		{
			id = msg_in.id;
			time = msg_in.time;
			print_time = msg_in.print_time;
			document_message = msg_in.document_message;
		}

		return *this;
	}
	inline avMsg_ID ID() const {return id;};
	inline wModelTime TIME() const {return time;};
	inline wModelTime PRINT_TIME() const {return print_time;};
	inline void void_print_time() {print_time = null_time;};
	inline void reset_print_time() {print_time = time;};
	inline const ed::string& get_document_message() const {return document_message;};
	inline ed::string& get_document_message_ref() {return document_message;};
	inline void set_document_message(const ed::string& document_message_in){document_message = document_message_in;};

	friend Serializer& operator << (Serializer& serializer, avEkranMessage& msg)
	{
		return serializer << msg.id
			<< msg.time
			<< msg.print_time
			<< msg.document_message
			;
	}
	
private:
	avMsg_ID id;
	wModelTime time, print_time;
	ed::string document_message;
};

const avEkranMessage null_message = avEkranMessage(nullID, null_time);

class avEkranMsgProperties
{
public:
	avMsg_CatNo number; // номер сообщения по каталогу
	avMsg_priority priority; // приоритет сообщения
	ed::string message;
};

class time_components
{
public:
	explicit time_components(wModelTime time)
	{
		int ltime = static_cast<int>(time);
		int minutes = ltime / 60;
		int minutes_hundreds = minutes / 100;
		char minutes_under_100 = minutes - (minutes_hundreds * 100);
		m_t = minutes_under_100 / 10;
		m_u = minutes_under_100 - (m_t * 10);

		char seconds = ltime - (minutes * 60);
		s_t = seconds / 10;
		s_u = seconds - (s_t * 10);
	};

	int m_t, m_u, s_t, s_u;

private:
	time_components(){};
	time_components& operator = (const time_components& tc_in){return *this;};
};

typedef std::multimap<avMsg_priority, avEkranMessage> av_ekran_priority_RAM;
typedef ed::list<avEkranMessage> av_ekran_queue_RAM;
typedef ed::list<avEkranMessage> av_ekran_print_queue;
typedef ed::vector<avEkranMsgProperties> av_ekran_msg_properties_base;

class COCKPITBASE_API avEkranControl :  public avDevice,
										public avBreakable, 
										public avBasicElectric
{
public:
	//avEkranControl();
	virtual ~avEkranControl();
	
//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void SetCommand(int, float value = 0){}
	virtual void serialize(Serializer &serializer);
	virtual void update();
    virtual void post_initialize()
    {
        
    }
//end of interface

	virtual void repair();

	void call();
	
	virtual bool checkSelfTestConditions() {return true;}
	void start_selftest();
	
	av_ekran_print_queue& get_print_queue() {return print_queue;};
	av_ekran_msg_properties_base& get_messages_base() {return messages_base;};
	const av_ekran_priority_RAM& get_RAM_memory() const {return RAM_memory;};
	const av_ekran_priority_RAM& get_RAM_queue() const {return RAM_queue;};
	void set_last_printed_msg(const avEkranMessage & msg_in)
	{
		last_printed_msg = msg_in;

		messages_count++;
		if (messages_count >= avEkran_max_messages)
			strip_end = true;
	};

	void new_message(avMsg_ID, bool only_document = false);
	void delete_message(avMsg_ID);
	
	virtual bool power_on();
	void power_off();
	
	bool get_is_strip_end() const {return strip_end;}
	
	bool isSignalApplicable(int signal_in);
	bool isReadyToProcessSignals();

    virtual bool getLampsTest() const { return false; };
protected:
	void setMode(int ModeIn) {prev_mode = current_mode; current_mode = ModeIn;}

	void flagOn(int signal_in);
	void flagOff(int signal_in);

	virtual void zeroize_flags() = 0;
	virtual bool isWOW_SignalPresent() = 0;
	
	void reset_transition_timers();
	void check_to_flightTO_transition();
	void check_to_flightPK_transition();
	void check_to_document_transition();
	
    inline void push_RPK(avEkranMessage msg)
	{
		RAM_RPK.push_back(msg);
		if (RAM_RPK.size() > avEkran_RAM_capacity_RPK)
			RAM_RPK.pop_front();
	}

	inline void insert_queue(std::pair<avMsg_priority, avEkranMessage> msg)
	{
		av_ekran_priority_RAM::iterator it;

		RAM_queue.insert(msg);
		if (RAM_queue.size() > avEkran_RAM_capacity_queue)
		{
			it = RAM_queue.end();
			it--;
			RAM_queue.erase(it);
		}
	}

	inline void insert_memory(std::pair<avMsg_priority, avEkranMessage> msg)
	{
		av_ekran_priority_RAM::iterator it;
		
		RAM_memory.insert(msg);
		if (RAM_memory.size() > avEkran_RAM_capacity_memory)
		{
			it = RAM_memory.end();
			it--;
			RAM_memory.erase(it);
		}
	}
	
	inline avMsg_priority msg_priority(avMsg_ID msgID) const
	{
		return messages_base[msgID].priority;
	}

	void set_system_startup_time();
	wModelTime get_system_time();
	void override_print(const avEkranMessage&);
	bool priority_print(const avEkranMessage&);
	void document_print();
	void clearPrintQueue();
	virtual void checkAllSignals() {}
	
	int current_mode, prev_mode;

	int avEkran_RAM_capacity_memory,
		avEkran_RAM_capacity_queue,
		avEkran_RAM_capacity_RPK;

	wModelTime avEkran_time_selftest,
		avEkran_time_to_takeoff_mode,
		avEkran_time_to_flightPK_mode_1,
		avEkran_time_to_flightPK_mode_2,
		avEkran_time_to_doc_mode_1,
		avEkran_time_to_doc_mode_2;

	int avEkran_max_messages;

	ed::vector< std::pair<bool, bool> > signals_matrix;
	ed::vector<int> messages_index;
	ed::vector<bool> OnlyDocumentMessages_matrix;
	ed::vector<bool> OnlyGroundControlMessages_matrix;

private:
	wModelTime selftest_timer,
		takeoff_transition_timer;
	bool takeoff_transition;

	wModelTime flightPK_transition_timer_1,
		flightPK_transition_timer_2;
	bool flightPK_transition_1, flightPK_transition_2;

	wModelTime doc_transition_timer_1,
		doc_transition_timer_2;
	bool doc_transition_1, doc_transition_2;

	wModelTime system_startup_time;

	av_ekran_priority_RAM RAM_queue;
	av_ekran_priority_RAM RAM_memory;
	av_ekran_queue_RAM    RAM_RPK;
	av_ekran_msg_properties_base messages_base;

	// очередь сообщений на печать
	// очередной элемент выбрасывается после окончания вывода
	av_ekran_print_queue print_queue;

	// последнее выведенное на печать
	// устанавливается при начале печати сообщения
	avEkranMessage last_printed_msg;

	bool strip_end;
	int messages_count;

	bool document_printed;
protected:
   	virtual const char * l_metatable_name() const { return "ekran_control_meta";}
	virtual void		 l_register_functions(lua_State *Lua_s,int index);
    static  int          l_get_actual_text_frame(lua_State *Lua_s);
private:
    ed::string last_text_frame_1;    //для экспорта
    ed::string last_text_frame_2;    //для экспорта
    int         actual_text_frame_number; //для экспорта
public:
    //костыли для выставки из индикатора
    const ed::string & get_last_text_frame_1() const           { return last_text_frame_1; } 
    const ed::string & get_last_text_frame_2() const           { return last_text_frame_2; } 
    void                set_last_text_frame_1(const char * str) { last_text_frame_1 = str; } 
    void                set_last_text_frame_2(const char * str) { last_text_frame_2 = str; } 
    void                set_actual_text_frame_number(int i)     { actual_text_frame_number = i; } ;
};

}

#endif // avEkranControl_H

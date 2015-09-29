#ifndef _avVMS_h
#define _avVMS_h

#include "Avionics/avDevice.h"
#include "Avionics/avBasicElectric.h"
#include "Avionics/avBreakable.h"
#include "Comm/wMessage.h"
#include "Sound/Sounder.h"

#include "FMElectricSystems/Items/Consumer.h"

namespace cockpit
{

typedef unsigned int avsys_msg_ID;
typedef unsigned int avVMS_msg_ID;

typedef enum VMS_numbers
{
	DIGITS_NULL = 0,
	BOARD,
	DIGITS_0_BEGIN,
	DIGITS_20_BEGIN,
	DIGITS_30_BEGIN,
	DIGITS_40_BEGIN,
	DIGITS_50_BEGIN,
	DIGITS_60_BEGIN,
	DIGITS_70_BEGIN,
	DIGITS_80_BEGIN,
	DIGITS_90_BEGIN,
	DIGITS_100_BEGIN,
	DIGITS_200_BEGIN,
	DIGITS_300_BEGIN,
	DIGITS_400_BEGIN,
	DIGITS_500_BEGIN,
	DIGITS_600_BEGIN,
	DIGITS_700_BEGIN,
	DIGITS_800_BEGIN,
	DIGITS_900_BEGIN,
	DIGITS_0_END,
	DIGITS_1_END,
	DIGITS_2_END,
	DIGITS_3_END,
	DIGITS_4_END,
	DIGITS_5_END,
	DIGITS_6_END,
	DIGITS_7_END,
	DIGITS_8_END,
	DIGITS_9_END,
	DIGITS_10_END,
	DIGITS_11_END,
	DIGITS_12_END,
	DIGITS_13_END,
	DIGITS_14_END,
	DIGITS_15_END,
	DIGITS_16_END,
	DIGITS_17_END,
	DIGITS_18_END,
	DIGITS_19_END,
	DIGITS_20_END,
	DIGITS_30_END,
	DIGITS_40_END,
	DIGITS_50_END,
	DIGITS_60_END,
	DIGITS_70_END,
	DIGITS_80_END,
	DIGITS_90_END,
	DIGITS_100_END,
	DIGITS_200_END,
	DIGITS_300_END,
	DIGITS_400_END,
	DIGITS_500_END,
	DIGITS_600_END,
	DIGITS_700_END,
	DIGITS_800_END,
	DIGITS_900_END,
	MESSAGE_BEGIN,
	MESSAGE_END,
	DIGITS_MAXIMUM
};

class avVMSMsgProperties
{
public:
	avsys_msg_ID sys_id;
	bool with_callsign;
	char times_to_repeat;
	wModelTime time_to_play;
};

typedef ed::vector<avVMSMsgProperties> av_VMS_msg_properties_base;
typedef ed::vector<float> av_VMS_msg_numbers_base;

class COCKPITBASE_API avVMS: public avDevice,
							 public avBreakable, 
							 public avBasicElectric
{
public:
	avVMS();
	virtual ~avVMS();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void post_initialize();
	virtual void serialize(Serializer &serializer){};
	virtual void release();
	virtual void update();
	//end of interface

	Sounder::Element & getSoundElement() { return messagesSource; }

	virtual void selftest() {};
	virtual bool play_message(avVMS_msg_ID ID);
	virtual void stop_playing();
	bool get_is_playing() {return is_playing;}

	bool isSignalApplicable(int signal_in);
	bool isReadyToProcessSignals();
    virtual void start_message_playing(const wMessagePtr & message);

protected:
	virtual void zeroize_flags();
	virtual void init_messages_matrix(int SignalsCount);
	virtual void init_messages_indices() = 0;

	void clearQueues();
	virtual void checkAllSignals() {}
	virtual void stop_message(const wMessagePtr& message);
	
	bool isSignalOn(int Signal);
	void setSignalOn(int Signal, bool Flag);
	
	void onPwrOff(const lwl::CSignalInfo* pInfo);

	bool is_playing, is_phrase, is_pause;
	int num_repeat;
	int state;

	wModelTime avVMS_phrase_timer, avVMS_pause_timer;
	wModelTime avVMS_pause_play_time;

	av_VMS_msg_properties_base messages_base;

	wMessagePtr CurrentMessage;
	avVMS_msg_ID CurrentMessageID;
	
	ed::list<int> SignalsToCheckQueue;
	ed::list<int> MessagesToPlayQueue;
	ed::vector< std::pair<bool, bool> > signals_matrix;
	ed::vector<int> messages_index;
	wMessage::Sender sender;
	Sounder::MessagesSource messagesSource;

	EagleFM::Elec::Consumer ElecConDevice;

private:
    void start_message(int InternalID);

};

}

#endif // _avVMS_h

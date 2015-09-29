#ifndef _wDatalinkCommunicator_h_
#define _wDatalinkCommunicator_h_

#include "wRadio.h"

#include "IwDatalinkTerminal.h"
#include "Comm/wNetCommunicator.h"

struct DatalinkState : public CommNetState
{
	bool		reportPosition;
	ed::string	SADLTN;
	int			Ka50VCUNumber;
	int			Ka50VCULeader;
};

//-=wDatalinkCommunicator=-

class WRADIO_API wDatalinkCommunicator : public IwDatalinkTerminal
{
public:
	class NetSendTransiverStatus
	{
	public:
		virtual ~NetSendTransiverStatus() {;}
		virtual void send() = 0;
	};
	wDatalinkCommunicator(	const woPointer & unit, unsigned int transmitFlags, unsigned int receiveFlags,
							IwTransiver * transiver, NetSendTransiverStatus * netSendTransiverStatus = NULL);
	~wDatalinkCommunicator();
	virtual	void send_msg(const datalink_msg & msg);
	virtual void receive_msg(const datalink_msg & msg);
	virtual	datalink_msgs_queue & get_msgs_gueue();
	virtual void getNetState(DatalinkState & datalinkState) const;
	
	inline wCommunicator & getCommunicator() { return communicator_; }
	void setNetState(const DatalinkState & datalinkState, wTransiver * pTransiver);	
private:
	class Communicator : public wCommunicator
	{
	public:
		Communicator(	const woPointer & unit, unsigned int transmitFlags, unsigned int receiveFlags,
						IwDatalinkTerminal * datalinkTerminal,
						IwTransiver * transiver);
	protected:
		virtual double	getMinSNR() const;
		virtual void	processMessage(const wMessagePtr & message);
		IwDatalinkTerminal * datalinkTerminal_;
	};
	Communicator communicator_;
	class RadioDeviceCallback; 
	RadioDeviceCallback * radioDeviceCallback_;
	datalink_msgs_queue msgs_queue_;
	IwTransiver * transiver_;
};


//-=wNetDatalinkCommunicator=-

//Local receive (receive-only transiver)
class WRADIO_API wNetDatalinkCommunicator : public IwDatalinkTerminal
{
public:
	class NetSendReceivedMessage
	{
	public:
		virtual ~NetSendReceivedMessage() {;}
		virtual void send(const datalink_msg & msg) = 0;
	};
	wNetDatalinkCommunicator(const woPointer & unit, unsigned int transmitFlags, unsigned int receiveFlags, NetSendReceivedMessage * netSendReceivedMessage);
	~wNetDatalinkCommunicator();
	virtual	void send_msg(const datalink_msg & msg);
	virtual void receive_msg(const datalink_msg & msg);
	virtual	datalink_msgs_queue & get_msgs_gueue();
	virtual void getNetState(DatalinkState & datalinkState) const;
	
	inline wCommunicator & getCommunicator() { return communicator_; }
	void setOnOff(bool on);
	void setNetState(const DatalinkState & datalinkState);
	void setTransiverState(bool enable, float sensitivity, float frequency, float bandWidth, wModulation modulation);
	wTransiverOwner	transiver;
private:
	class Communicator : public wCommunicator
	{
	public:
		Communicator(	const woPointer & unit, unsigned int transmitFlags, unsigned int receiveFlags, 
						IwDatalinkTerminal * datalinkTerminal, IwTransiver * transiver);
		virtual double getMinSNR() const;
		virtual void processMessage(const wMessagePtr & message);
	private:
		IwDatalinkTerminal * datalinkTerminal_;
	};
	NetSendReceivedMessage * netSendReceivedMessage_;
	Communicator communicator_;
};

WRADIO_API Mail::Stream & operator >> (Mail::Stream& stream, DatalinkState & datalinkState);

template<class SenderType>
class TypedNetSendReceivedMessage : public wNetDatalinkCommunicator::NetSendReceivedMessage
{
public:
	typedef void(SenderType::*SendFunc)(const datalink_msg & msg);
	TypedNetSendReceivedMessage(SenderType * sender, SendFunc pSendFunc) : sender_(sender), pSendFunc_(pSendFunc) {;}
private:
	virtual void send(const datalink_msg & msg)
	{
		(sender_->*pSendFunc_)(msg);
	}
	SenderType * sender_;
	SendFunc pSendFunc_;
};

#endif _wDatalinkCommunicator_h_

#pragma  once

#include "Avionics/Radio/avIntercom.h"

namespace cockpit {

class avBaseRadio;

class COCKPITBASE_API avIntercomWWII : 	public cockpit::avIntercom
{
	enum avIntercom_mode
	{
		INTERCOM_VHF_MODE = 0,
	};

	enum avIntercom_source
	{
		INTERCOM_SOURCE_VHF,
		INTERCOM_SOURCE_MAX
	};

	enum avIntercom_communicator
	{
		INTERCOM_COMM_VOID = -1,
		INTERCOM_COMM_VHF,
		INTERCOM_COMM_MAX
	};

public:

	//avDevice interface
	virtual void initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void post_initialize();
	//end of interface

	void		start_transmission(bool enable, bool micro);

protected:
	void		setCommunicator(avIntercom_communicator communicatorNum);
	void		trySetCommunicator(unsigned comm_id);

	avIntercom_communicator m_currentTransmitterNum;

	unsigned char		communicatorDeviceID;
private:

	DECLARE_DEV_LINKPTR(cockpit::avBaseRadio, VHF_Radio);

	avCommunicator*		communicator[INTERCOM_COMM_MAX];
};

} 

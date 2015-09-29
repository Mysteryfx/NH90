#pragma once;
#include "Avionics/avDevice.h"
#include "ICommandsCockpitDevice.h"

#include "FMElectricSystems/Items/Relay.h"
#include "FMElectricSystems/Items/BackupSwitcher.h"
#include "FMElectricSystems/Items/Consumer.h"

namespace cockpit
{ 

    enum IFF_commands
    { 
        Command_IFF_Mode1_Wheel_1   = iCommandCockpitDevice_Action_1,
        Command_IFF_Mode1_Wheel_2,

        Command_IFF_Mode3A_Wheel_1,
        Command_IFF_Mode3A_Wheel_2,
        Command_IFF_Mode3A_Wheel_3,
        Command_IFF_Mode3A_Wheel_4,

		Command_IFF_Code,
		Command_IFF_Master,
		Command_IFF_Audio_Light,
		Command_IFF_Test_M_1,
		Command_IFF_Test_M_2,
		Command_IFF_Test_M_3A,
		Command_IFF_Test_M_C,
		Command_IFF_RAD_Test_Mon,
		Command_IFF_Mic_Ident,
		Command_IFF_On_Off,
		Command_IFF_Reply,
		Command_IFF_Test,
		Command_IFF_Antenna,
		Command_IFF_Reply_Light_Dimming,
		Command_IFF_Test_Light_Dimming,
    };

    class COCKPITBASE_API avIFF_APX_72 : public avDevice
    { 
	public:
		enum IFF_CODE_Position
		{
			IFF_CODE_Zero = 0,
			IFF_CODE_B,
			IFF_CODE_A,
			IFF_CODE_HOLD
		};
		enum IFF_Lamps
		{
			IFF_REPLY_LAMP = 0,
			IFF_TEST_LAMP,
			IFF_LAMPS_MAX
		};
	public:
		avIFF_APX_72(){}
		virtual ~avIFF_APX_72(){}
	
		//avDevice interface
		virtual void post_initialize();
		virtual void SetCommand(int command,float value);
		virtual bool checkAnimationConditions(int arg_num, int command, float& value);
		virtual bool getElecPower() const = 0;
		//end of interface

		double	get_light_status(unsigned int lamp_id) const;
	
	protected:
		unsigned int current_code_position_;

	protected:

		EagleFM::Elec::Switch	power_switch_;

		EagleFM::Elec::Switch	reply_switch_;
		EagleFM::Elec::Switch	reply_test_switch_;
		EagleFM::Elec::Switch	reply_press_;
		EagleFM::Elec::Switch	reply_test_press_;
		EagleFM::Elec::BackupSwitcher	reply_nodeor_;

		EagleFM::Elec::Switch	test_switch_;

		struct IFF_lamp
		{
			IFF_lamp() : lamp_consumer_(EagleFM::Elec::DC_26_VOLT), brightness_(0.0) { }
			double brightness_;
			EagleFM::Elec::Consumer	lamp_consumer_;
		};

		IFF_lamp lamps_[IFF_LAMPS_MAX];
    };
}

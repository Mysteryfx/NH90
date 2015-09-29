#pragma once
#include "Avionics/avDevice.h"
#include "Avionics/avBreakable.h"
#include "dPosition.h"
#include "Avionics/Sensors/avSensorEye.h"
#include "Utilities/ccPolarPosition.h"
#include "Utilities/avDrawCashedObj.h"

#include "FMElectricSystems/Items/Consumer.h"
#include "FMElectricSystems/Items/Switch.h"

using namespace EagleFM;

namespace cockpit
{

    class avVMS;

    class MLWS_contact : public avDrawCashedObj
    {
    public:
        MLWS_contact():updates_count(false), is_indicated_by_rwr(false) {}
       ~MLWS_contact(){}
        woPointer     missile;
        double        time;
        PolarPosition first_spike;
        unsigned      updates_count;
		bool          is_indicated_by_rwr; // true if threat is indicated on RWR
    };

    enum MLWS_messages
    {
        msg_MLWS_03_Low,
        msg_MLWS_03_High,

        msg_MLWS_06_Low,
        msg_MLWS_06_High,

        msg_MLWS_09_Low,
        msg_MLWS_09_High,

        msg_MLWS_12_Low,
        msg_MLWS_12_High,

        MLWS_MESSAGES_COUNT,
    };


    typedef ed::vector<avSensorEye>             MLWS_eye_vector;
    typedef ed::map<unsigned int,MLWS_contact>  MLWS_contacts_map;

    typedef std::pair<unsigned int,MLWS_contact> MLWS_contact_pair;


	// description of threat spotted by MLWS
	class COCKPITBASE_API MWS_Threat
	{
	public:

		enum
		{
			TypeLaunch,  // missile launch
			TypeLaser,   // laser lighting
		};

		MWS_Threat();
		void reset();

		int Type;    // type of threat
		int SubType; // It is for laser

		float Azimuth, Elevation; // position of threat
		float SignalStreight;

		wModelTime BirthTime, UpdateTime;
		woPointer pObject;    // The pointer on launched missile
		bool IsValid;
		int WarningCounter;
	};

	typedef ed::vector<MWS_Threat> ThreatsStorage;


    class COCKPITBASE_API avMLWS : public avDevice,
									public avBreakable
    {
    public:
        avMLWS();
       ~avMLWS() {}

        virtual void initialize(unsigned char id, const ed::string& name, const ed::string& script);
        virtual void SetCommand(int command, float value = 0);
        virtual void update();

		// electric interface
		virtual bool getElecPower() const {return ElecConDevice.isPowered();}
		virtual void setElecPower(bool flag_in) {ElecSwitchDevicePower.setOnOff(flag_in);}
		//end of interface

		virtual const bool	isDegraded()const;

        MLWS_contacts_map        & get_contacts()       { return contacts;};
        const  MLWS_contacts_map & get_contacts() const { return contacts;};

#ifdef _ED_HIDDEN_DEBUG 
        void external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects);
#endif

    protected:
        // !!!!!  MLWS eyes and contacts section

		// handler of spotted missile
        static bool handler          (viObject *item, viSearch *search, void* data);
        bool        handler_implement(viObject *item, viSearch *search);

		const dPosition & get_human_position() const;

		bool        check_inside_eyes_fov(const dPosition & human_pos,const dVector & point) const;

        bool exist(const woPointer & missile);
        void insert(const woPointer & missile);
        void notify(MLWS_contact & contact);

        PolarPosition get_spike(MLWS_contact & contact);
        PolarPosition get_spike_world(MLWS_contact & contact);

        void update_eye(avSensorEye & eye);
        void update_single_search();
        void update_existing_contacts();
        bool check_visible(const dPosition & pos, woPointer& ptr) const;
        void play_message(unsigned msg);

		MLWS_eye_vector   eyes;
		MLWS_contacts_map contacts;

		bool              arcade;
		bool              use_single_search_volume;

		int    message_table[MLWS_MESSAGES_COUNT];
		double notified_cash_time[MLWS_MESSAGES_COUNT];
		double last_notified_time;
		double message_legth;
		double notify_delta;

        // Methods for operation with Threats
		bool tryInsertThreat(MWS_Threat& new_threat);
		int findNewPlace();
		MWS_Threat* findByID(ObjectID ID);
		int findOldestThreat(wModelTime& Time);
		int getThreatsSize() const {return ThreatsSize;}
		void checkThreatsSize();
		void SetThreatIsValid(int ThreatPos);
		void SetThreatIsNotValid(int ThreatPos);
		void ClearThreats();
		float getSignalStreigth(woPointer pFrom, const cPosition& currPlane_pos, int Type) const;


		ThreatsStorage  Threats;
		int				ThreatsSize;
		float			MaxElevation;
		unsigned short  MaxThreats;
		wModelTime		ThreatLiveTime;

		Elec::Consumer	ElecConDevice;
		Elec::Switch	ElecSwitchDevicePower;

	private:
		//DECLARE_DEV_LINKPTR(avVMS, betty);
		void* betty; // a very primitive way to get rid of ERROR COCKPITBASE for the wrong link
    };
}

#pragma once
#include "LoadableEquipment/Weapon/eqTVGuidedWeapon.h"
#include "Avionics/Sensors/avLaserSpotDetector.h"

namespace cockpit {


    enum E_mode_enum
    {
        EMODE_NULL,
        EMODE_READY,
        EMODE_ACTIVATE,
        EMODE_SCAN,
        EMODE_SLAVE,
    };

    class  COCKPITBASE_API eqAGM_65E : public eqTVGuidedWeapon
    {
    public:
        eqAGM_65E(const wsType & type_ = wsType(0,0,0,0));
       ~eqAGM_65E(void);

        void  initialize();
        void  scan_stop();
        void  scan(double delta);
        void  on_axis_stop();
		void  StartScan(wModelTime Delay = 0.0);
        void  search() {};
		void  update(double delta);

		virtual void zoom_in() {}
		virtual void zoom_out() {}

        void  lockon();
		virtual void free_lock();
		virtual void break_lock() {}
		virtual void uncage();
		virtual void cage();
		virtual bool is_LockOn() const;

        bool  have_valid_target() const { return LSS.slave(); }
        int   get_E_mode() const;
        bool  is_PRF_entered() const { return PRF_entered; }

		void  setElecPower(bool flag_in);

		void set_code(int code);
		int  get_code() const {return LSS.get_code();}

		virtual void slew_left(double SlewCoeff = 1.0);
		virtual void slew_right(double SlewCoeff = 1.0);
		virtual void slew_up(double SlewCoeff = 1.0);
		virtual void slew_down(double SlewCoeff = 1.0);
		virtual bool on_TV_SENSOR_move_horizontal_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		virtual bool on_TV_SENSOR_move_vertical_abs(float value, float dead_zone = 0.05, double SlewCoeff = 1.0);
		 
    protected:
		void set_gyro_moving();
		bool use_gyro_base() const;
        void start_search() {};
        void stop_search() {};

		void tracking(double delta);
		
		avLaserSpotDetector LSS;
		bool  PRF_entered;
		static const unsigned int Initial_LSR_Code;
		
		float ScanDirection;

		wModelTime TargetLostTime;
		static const wModelTime NoTargetDelay;

		bool ScanCommanded;
    };
}

#ifndef _avMarkerReceiver_h
#define _avMarkerReceiver_h

#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"

class wRadioReceiver;

namespace cockpit
{

class COCKPITBASE_API avMarkerReceiver :	public avDevice,
											public avBreakable, 										
											public avBasicElectric
{
public:
	avMarkerReceiver();
	virtual ~avMarkerReceiver();
	
	//avDevice interface
	virtual void initialize(unsigned char, const ed::string&, const ed::string&);
	virtual void SetCommand(int, float value = 0) {}
	virtual void update();
	virtual void release();
	//end of interface

	//Sounder::Element & getSoundElement() { return receiver.getSoundElement(); }

	virtual void check_device_status();
	virtual bool get_device_status();

	bool is_BRPM_below() const {return BPRM_below;}
	bool is_DRPM_below() const {return DPRM_below;}

	bool is_InnerMarker_below() const {return InnerMarker_below;}
	bool is_MiddleMarker_below() const {return MiddleMarker_below;}
	bool is_OuterMarker_below() const {return OuterMarker_below;}
	bool is_Marker_below() const {return Marker_below;}

	void check_marker_beacons();
	bool is_signal_received() const;//пойман хотя бы один передатчик
	wRadioReceiver *getRadioReceiver() {return tr_receiver;}

	void setMute(bool mute_) {mute = mute_;}
	bool getMute() const {return mute;}
protected:
	wRadioReceiver *tr_receiver;
private:
	cPoint last_transmitters_check_point;
	bool BPRM_below;
	bool DPRM_below;
	bool	InnerMarker_below;
	bool	MiddleMarker_below;
	bool	OuterMarker_below;
	bool	Marker_below;
	bool mute;
};

}

#endif // _avMarkerReceiver_h

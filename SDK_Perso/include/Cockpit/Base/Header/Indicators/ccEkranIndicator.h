#ifndef ccEkranIndicator_H
#define ccEkranIndicator_H

#include "Indicators/ccIndicator.h"
#include "Avionics/avEkranControl.h"

namespace cockpit { 

const float avEkran_indicator_dz = 0.75f;
const wModelTime avEkran_indicator_print_time = 0.45;

// для смены текста
struct COCKPITBASE_API frame_info
{
     frame_info()
     {
         zeroize_time();
         change    = false;
         curr_text = "";
     };
     void zeroize_time()
     { 
        time_string[0] =
        time_string[1] =
        time_string[2] =
        time_string[3] = ' ';
     };
     bool        change;
     ed::string curr_text;
     char        time_string[4];
};

class COCKPITBASE_API ccEkranIndicator : public ccIndicator
{
public:
	ccEkranIndicator();
	virtual ~ccEkranIndicator();
	virtual void create_elements_controllers_map();
	virtual void initialize(avDevice*, unsigned char, const ed::string&);
	
	static void txt1_control(Element*, ccDrawable*, const DParamList&);
	static void txt2_control(Element*, ccDrawable*, const DParamList&);
	static void txt_memory_control(Element*, ccDrawable*, const DParamList&);
	static void txt_queue_control(Element*, ccDrawable*, const DParamList&);
	static void txt_failure_control(Element*, ccDrawable*, const DParamList&);

	double get_dz_1() const {return dz_1;}
	double get_dz_2() const {return dz_2;}
	
	bool get_memory_flag() const {return memory_flag;}
	bool get_queue_flag() const {return queue_flag;}
	bool get_failure_flag() const {return failure_flag;}
		
	virtual void control();

	bool isOff() {return off;}

private:
    
    void make_time_string(frame_info & f, const avEkranMessage&);
	void freeze_strip(frame_info & f);
    void default_time_str(frame_info & f)
    { 
        f.time_string[0] = 
        f.time_string[1] = 
        f.time_string[2] = 
        f.time_string[3] = ' ';
    }
    void do_string_change(avEkranMessage& msg,frame_info & f);

	bool idle;
	bool print;
	bool strip_move;
	
	char curr_message;

	// для движения
	float dz_1, dz_2;
	float dz_lim1, dz_lim2;
	wModelTime print_start_time;


  	// для смены текста
    frame_info frame_1;
    frame_info frame_2;



    // флаги индикаторов
	bool memory_flag;
	bool queue_flag;
	bool failure_flag;
	bool off;
	bool moveFreezedMessage;
};

}

#endif // ccEkranIndicator_H

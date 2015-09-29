#pragma once
#include "Inter.h"
#include "cLinear.h"

struct lua_State;

class INTER_API ItriggerMail {
public:
    virtual ~ItriggerMail() = 0 {};

    virtual void outText(const ed::string&, const int) = 0;
    virtual void outSound(const ed::string&) = 0;

    // side-specific output
    virtual void outText_s(const ed::string&, const int, const char) = 0;
    virtual void outSound_s(const ed::string&, const char) = 0;

    // country-specific output
    virtual void outText_c(const ed::string&, const int, const char) = 0;
    virtual void outSound_c(const ed::string&, const char) = 0;

    virtual void explosion(const double x, const double y, const double z, const double volume) = 0;
    virtual void explosion_marker(const double x, const double y, const double z, const int color) = 0;

    virtual void illumination_bomb(const double x, const double y, const double z) = 0;
    virtual void signal_flare(const double x, const double y, const double z, const int color, const double bearing) = 0;

	virtual void radio_transmission(const ed::string& file_, const cPoint & point_, const bool am_fm_, const bool loop_, const int frequency, const int power, const ed::string & name) = 0;
	virtual void stop_radio_transmission(const ed::string & name) = 0;
}; // class triggerMail

class INTER_API Itrigger
{
    
public:
    virtual ~Itrigger() = 0 {};

    typedef ed::string flag_t;

    virtual void init(lua_State *) = 0; // инициализируемся, устанавливаем callbacks в стек
    virtual void start() = 0; // создаём деятельности

    //////////////////////////////////////////////////////////////////////////
    // callbacks 
    //////////////////////////////////////////////////////////////////////////
   
    virtual void new_mission() = 0;

    // private conditions
    virtual int c_time(lua_State *L) = 0; // OUT: number,

    // public condition
    virtual int c_unit_alive(lua_State *L) = 0; // IN: unit name, OUT: bool
    virtual int c_unit_dead(lua_State *L) = 0; // IN: unit name, OUT: bool
    virtual int c_unit_damaged(lua_State *L) = 0; // IN: unit name OUT: bool
    virtual int c_random_less(lua_State *L) = 0; // IN: int value OUT: bool

    virtual int c_group_alive(lua_State *L) = 0; // IN: group name, OUT: bool
    virtual int c_group_dead(lua_State *L) = 0; // IN: group name, OUT: bool

    virtual int c_time_after(lua_State *L) = 0; // IN: number, OUT: bool
    virtual int c_time_before(lua_State *L) = 0; // IN: number, OUT: bool
    virtual int c_unit_in_zone(lua_State *L) = 0; // IN: unit name, zone name, OUT: bool
    virtual int c_unit_out_zone(lua_State *L) = 0; // IN: unit name, zone name, OUT: bool
    virtual int c_unit_in_zone_unit(lua_State *L) = 0; // IN: unit name, zone name, zone unit, OUT: bool
    virtual int c_unit_out_zone_unit(lua_State *L) = 0; // IN: unit name, zone name, zone unit, OUT: bool
	virtual int c_bomb_in_zone(lua_State *L) = 0; // IN: unit name, zone name, OUT: bool

	virtual int c_signal_flare_in_zone(lua_State *L) = 0; // IN: unit name, zone name, OUT: bool

    virtual int c_flag_is_true(lua_State *L) = 0; // IN: flag number, OUT: bool
    virtual int c_flag_is_false(lua_State *L) = 0; // IN: flag number, OUT: bool

    // return TRUE if specific time was passed since flag was set to true
    virtual int c_time_since_flag(lua_State *L) = 0; // IN: flag number, number (seconds), OUT: bool  

    // return TRUE if player in specific coalition
    virtual int c_player_coalition(lua_State *L) = 0; // IN: string, OUT: bool  

    virtual int c_unit_altitude_higher(lua_State *L) = 0; // IN: unit name, altitude, OUT: bool  
    virtual int c_unit_altitude_lower(lua_State *L) = 0; // IN: unit name, altitude, OUT: bool  
    virtual int c_unit_speed_higher(lua_State *L) = 0; // IN: unit name, speed, OUT: bool  
    virtual int c_unit_speed_lower(lua_State *L) = 0; // IN: unit name, speed, OUT: bool  

    virtual int c_unit_heading(lua_State *L) = 0; // IN: unit name, min, max , OUT: bool  
    virtual int c_unit_pitch(lua_State *L) = 0; // IN: unit name, min, max , OUT: bool  
    virtual int c_unit_bank(lua_State *L) = 0; // IN: unit name, min, max , OUT: bool  
    virtual int c_unit_vertical_speed(lua_State *L) = 0; //IN: unit name, min, max , OUT: bool  

    virtual int c_mission_score_higher(lua_State *L) = 0; // IN: coalition ("red", "blue", "offline"), value, OUT: bool  
    virtual int c_mission_score_lower(lua_State *L) = 0; // IN: coalition ("red", "blue", "offline"), value, OUT: bool  

    virtual int c_coalition_has_airdrome(lua_State *L) = 0; // IN: coalition, airdromeID (wstype.l4), OUT: bool  
    virtual int c_coalition_has_helipad(lua_State *L) = 0; // IN: coalition, helipadID (mission name), OUT: bool  

    virtual int c_all_of_group_in_zone(lua_State *L) = 0; // IN: group name, zone name 
    virtual int c_all_of_group_out_zone(lua_State *L) = 0; // IN: group name, zone name 
    virtual int c_part_of_group_in_zone(lua_State *L) = 0; // IN: group name, zone name 
    virtual int c_part_of_group_out_zone(lua_State *L) = 0; // IN: group name, zone name 

    virtual int c_all_of_coalition_in_zone(lua_State *L) = 0; 
    virtual int c_all_of_coalition_out_zone(lua_State *L) = 0; 
    virtual int c_part_of_coalition_in_zone(lua_State *L) = 0; 
    virtual int c_part_of_coalition_out_zone(lua_State *L) = 0; 

	virtual int c_player_score_more(lua_State *L) = 0;
    virtual int c_player_score_less(lua_State *L) = 0;

    virtual int c_group_life_less(lua_State *L) = 0;
    virtual int c_unit_life_less(lua_State *L) = 0;

    virtual int c_flag_less(lua_State *L) = 0;
    virtual int c_flag_more(lua_State *L) = 0;
    virtual int c_flag_less_flag(lua_State *L) = 0;
    virtual int c_flag_equals(lua_State *L) = 0;
    virtual int c_flag_equals_flag(lua_State *L) = 0;
	virtual int c_expression(lua_State *L) = 0;

    // action
    virtual int a_end_mission(lua_State *L) = 0; // IN: result
    virtual int a_activate_group(lua_State *L) = 0; 
    virtual int a_deactivate_group(lua_State *L) = 0; 
    virtual int a_fall_in_template(lua_State *L) = 0;//IN: templateId, groupId
    virtual int a_out_text_delay(lua_State *L) = 0; // IN: text string, int seconds
    virtual int a_out_text_delay_c(lua_State *L) = 0; // IN: text string, int seconds, string side
    virtual int a_out_text_delay_s(lua_State *L) = 0; // IN: text string, int seconds, string country
    virtual int a_out_text_delay_g(lua_State *L) = 0; // IN: text string, int seconds, string group
    virtual int a_out_sound(lua_State *L) = 0; // IN: file name
	virtual int a_out_sound_stop(lua_State *L) = 0; // IN: file name
	virtual int a_out_sound_s(lua_State *L) = 0; // IN: file name string, string side
    virtual int a_out_sound_c(lua_State *L) = 0; // IN: file name string, string country
    virtual int a_out_sound_g(lua_State *L) = 0; // IN: file name string, string group
    virtual int a_set_flag(lua_State *L) = 0; // IN: flag number
    virtual int a_clear_flag(lua_State *L) = 0; // IN: flag number
    virtual int a_set_mission_result(lua_State *L) = 0; // IN: result
    virtual int a_set_failure(lua_State *L) = 0; // IN: string failure name, double probability, time start, time duration
	virtual int a_set_internal_cargo(lua_State *L) = 0; // IN: cargo mass
    virtual int a_set_command(lua_State *L) = 0;// IN: command, value 
    virtual int a_explosion(lua_State *L) = 0; // IN: string zone_name, double volume
    virtual int a_explosion_unit(lua_State *L) = 0; // IN: string unit name, double volume
    virtual int a_explosion_marker(lua_State *L) = 0; // IN: string zone name, int color
    virtual int a_explosion_marker_unit(lua_State *L) = 0; // IN: string unit name, int color

    virtual int a_illumination_bomb(lua_State *L) = 0; // IN: string zone_name, double volume
    virtual int a_signal_flare(lua_State *L) = 0; // IN: string zone_name, int color
    virtual int a_signal_flare_unit(lua_State *L) = 0; // IN: string unit name, int color

    virtual int a_play_argument(lua_State *L) = 0; // IN: string group, int arg, double start, double stop, double speed
    virtual int a_set_altitude(lua_State *L) = 0; // IN: string group, double altitude

    virtual int a_load_mission(lua_State *L) = 0; // IN: string mission_name

    virtual int a_add_radio_item(lua_State *L) = 0; // IN: string, flag, path
    virtual int a_remove_radio_item(lua_State *L) = 0; // IN: path

	virtual int a_add_radio_item_for_coalition(lua_State *L) = 0; //IN: number, string, path

	virtual int a_remove_radio_item_for_coalition(lua_State *L) = 0; //IN: number, path

	virtual int a_add_radio_item_for_group(lua_State *L) = 0; //IN: number, number, string, flag, path

	virtual int a_remove_radio_item_for_group(lua_State *L) = 0; //IN: number, number, path

	virtual int a_set_ai_task(lua_State *L) = 0;
	virtual int a_ai_task(lua_State *L) = 0;

    virtual int a_group_stop(lua_State *L) = 0; 
    virtual int a_group_resume(lua_State *L) = 0; 

	virtual int a_unit_on(lua_State *L) = 0; 
	virtual int a_unit_off(lua_State *L) = 0; 

	virtual int a_group_on(lua_State *L) = 0; 
	virtual int a_group_off(lua_State *L) = 0; 

    virtual int a_unit_emission_on(lua_State *L) = 0;
    virtual int a_unit_emission_off(lua_State *L) = 0;

    virtual int a_inc_flag(lua_State *L) = 0;
    virtual int a_dec_flag(lua_State *L) = 0;
    virtual int a_set_flag_value(lua_State *L) = 0;
	virtual int a_set_flag_random(lua_State *L) = 0;	
    virtual int a_radio_transmission(lua_State *L) = 0;
	virtual int a_stop_radio_transmission(lua_State *L) = 0;
    virtual int a_set_briefing(lua_State *L) = 0;
	virtual int a_do_script(lua_State *L) = 0;
	virtual int a_do_script_file(lua_State *L) = 0;

    //////////////////////////////////////////////////////////////////////////
    // utils
    virtual double check_mission_resultEx() = 0; // uses check_mission_result() with current human coalition
    virtual double check_mission_result(const ed::string& coalition_) = 0;

    virtual int get_flag(const flag_t &) = 0;
    virtual void set_flag(const flag_t &, const int = 1) = 0;
    virtual void clear_flag(const flag_t &) = 0;
    //virtual void dump_flags(std::ofstream &) = 0;

    //helpers
    virtual void out_text(const ed::string& string_, int delay_) = 0;
    virtual void out_sound(const ed::string& file_) = 0; // IN: file name
	virtual void out_sound_stop() = 0; // 
    virtual void out_text_s(const ed::string& string_, int delay_, char side_) = 0;
    virtual void out_sound_s(const ed::string& file_, char side_) = 0; // IN: file name
    virtual void out_text_c(const ed::string& string_, int delay_, char country_) = 0;
    virtual void out_sound_c(const ed::string& file_, char coutnry_) = 0; // IN: file name
    virtual void out_text_g(const ed::string& string_, int delay_, int group_) = 0;
    virtual void out_sound_g(const ed::string& file_, int group_) = 0; // IN: file name
    virtual void explosion(const double x, const double y, const double z, const double volume) = 0; // IN: point(x,y,z) volume
    virtual void explosion_marker(const double x, const double y, const double z, const int color) = 0; // IN: point(x,y,z)
    virtual void illumination_bomb(const double x, const double y, const double z) = 0; // IN: point(x,y,z) volume
    virtual void signal_flare(const double x, const double y, const double z, const int color, const double bearing) = 0; // IN: point(x,y,z) int color (GREEN = 0, RED = 1; WHITE = 2, YELLOW = 3)
	virtual void radio_transmission(const ed::string& file_, const cPoint & point_, const bool am_fm_, const bool loop_, const int frequency, const int power, const ed::string & name) = 0;
	virtual void stop_radio_transmission(const ed::string & name) = 0;

    virtual void service_message(const ed::string& string_, double delay_) = 0;
    virtual void service_sound(const ed::string& sound_) = 0;

    virtual void createTriggerEvent(const ed::string& event_) = 0;

    virtual ItriggerMail* getImail() const = 0;
};

extern INTER_API Itrigger *globalTrigger;

#pragma once
#include "CockpitBase.h"
#include "Indicators/ccIndicator.h"
#include "Indicators/ccIndicatorPageImpl.h"
#include "Indicators/ccIndicationTemplateBuffer.h"
#include "Avionics/Sensors/avArcadeRadar.h"

namespace cockpit 
{
    enum arcade_templates
    {
        ARCADE_HUD,
        ARCADE_HSD,
        ARCADE_SET_MAX
    };

    class avMLWS;

    template <typename T>
    class  COCKPITBASE_API trail 
    {
    public:
        trail(unsigned size)
        {
            window_start = 0;
            window_end   = 0;
            trail_size   = size;
            current_history_size = 0;
            history.resize(trail_size + 2);
        }
        void step(const T & next_value)
        {
            if (window_end >= history.size())
                window_end -= history.size();
            history[window_end] = next_value;
            ++window_end;
            if (current_history_size < trail_size)
            {
                ++current_history_size;
                return;
            }
            if(++window_start >= history.size())
                window_start -= history.size();
        }
        const T & operator[](unsigned i) const
        {
            i += window_start;
            return i < history.size() ? history[i]:
                                        history[i - history.size()];
        }
        const unsigned size() const { return current_history_size; }
    private:
        ed::vector<T> history;
        int window_start;
        int window_end;
        int trail_size;
        int current_history_size;
    };
    
    template class trail<Vector3>;

    
    class  COCKPITBASE_API  mov_ptr_mark : public avDrawCashedObj
    {
    public:
        mov_ptr_mark():mov_ptr(0){}
        mov_ptr_mark(const woPointer & obj);
        virtual ~mov_ptr_mark(){};
        bool operator ==(const mov_ptr_mark & t) const   {  return mov_ptr == t.mov_ptr;    }
        bool operator !=(const mov_ptr_mark & t) const   {  return mov_ptr != t.mov_ptr;    }
        dVector   get_point() const { return get_position().p; };
        dPosition get_position() const;
        double    get_course() const;
        void      set_local_point(const dVector & new_local_point) { local_pnt = new_local_point;}
    private:
        woPointer  mov_ptr;
        dVector    local_pnt;
    };
    typedef ed::map<unsigned,mov_ptr_mark>  marks;

    class COCKPITBASE_API ccArcadeRadar : public ccIndicator
    {
        public:
            ccArcadeRadar();
            virtual ~ccArcadeRadar();

            virtual void initialize(avDevice* dev, unsigned char id, const ed::string& script);
            virtual void create_elements_controllers_map();
            virtual void control();

            virtual void draw_temporal_set(Element *start_point,unsigned temporal_set,unsigned char level);
            static  void draw_targets(Element * element, ccDrawable* parent, const DParamList& args);
            static  void draw_target_hud(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_current_target_hud(Element * element, ccDrawable* parent, const DParamList& args);
            static  void draw_mission_target_hud(Element* element, ccDrawable* parent, const DParamList& args);

            static  void draw_target_hsd(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_current_target_hsd(Element * element, ccDrawable* parent, const DParamList& args);
            static  void draw_mission_target_hsd(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_iff_color(Element* element, ccDrawable* parent, const DParamList& args);
            
            static  void draw_mlws_contact_hsd(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_mlws_contact_hud(Element* element, ccDrawable* parent, const DParamList& args);

            static  void draw_mlws_contacts(Element* element, ccDrawable* parent, const DParamList& args);

            static  void you_are_the_target_blinking(Element* element, ccDrawable* parent, const DParamList& args);
            static  void show_radar(Element* element, ccDrawable* parent, const DParamList& args);
            static  void reset_buffer(Element* element, ccDrawable* parent, const DParamList& args);
            static  void apply_buffer(Element* element, ccDrawable* parent, const DParamList& args);
            
            static  void txt_scale(Element* element, ccDrawable* parent, const DParamList& args);
            static  void txt_course(Element* element, ccDrawable* parent, const DParamList& args);

            static  void north_direction_hsd(Element* element, ccDrawable* parent, const DParamList& args);

            static void draw_velocity_vector_hsd(Element* element, ccDrawable* parent, const DParamList& args);
            static void draw_velocity_vector(Element* element, ccDrawable* parent, const DParamList& args);

            static void pitch_scale(Element* element, ccDrawable* parent, const DParamList& args);
            static void bank_scale(Element* element, ccDrawable* parent, const DParamList& args);
            static void vy_scale(Element* element, ccDrawable* parent, const DParamList& args);

            static void baro_altitude_rotate(Element* element, ccDrawable* parent, const DParamList& args);
            static void radar_altitude_rotate(Element* element, ccDrawable* parent, const DParamList& args);
            static void surface_height_rotate(Element* element, ccDrawable* parent, const DParamList& args);
            static void switch_altitude_scale_radar(Element* element, ccDrawable* parent, const DParamList& args);

            static void draw_selector_point(Element* element, ccDrawable* parent, const DParamList& args);
            static void draw_target_distance(Element* element, ccDrawable* parent, const DParamList& args);

            static void hsd_history_pnt(Element* element, ccDrawable* parent, const DParamList& args);

            static  void draw_airfields(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_mark_hsd(Element* element, ccDrawable* parent, const DParamList& args);

            static  void draw_sight_hud(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_forestall_hud(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_distance_authorized(Element* element, ccDrawable* parent, const DParamList& args);

            static  void draw_waypoint_hud(Element* element, ccDrawable* parent, const DParamList& args);
            static  void draw_waypoint_hsd(Element* element, ccDrawable* parent, const DParamList& args);

			static void draw_selected_weapon_type (Element* element, ccDrawable* parent, const DParamList& args);
			static void draw_selected_weapon_type_simple (Element* element, ccDrawable* parent, const DParamList& args);

			static void draw_selected_weapon_count(Element* element, ccDrawable* parent, const DParamList& args);
			static void draw_selected_station_color(Element* element, ccDrawable* parent, const DParamList& args);
			static void draw_station(Element* element, ccDrawable* parent, const DParamList& args);
			static void draw_fire_permission(Element* element, ccDrawable* parent, const DParamList& args);

			static void draw_gun_is_selected(Element* element, ccDrawable* parent, const DParamList& args);
        protected:
            static void  to_hsd_space(avArcadeRadar * arcade,const dPosition & p,Element * element,const DParamList& args,bool restrict = true);
            static void  to_hsd_space(avArcadeRadar * arcade,const dVector & p,Element * element,const DParamList& args,bool restrict = true);
            static const dPosition  & hsd_position();
            
            template<class T>
            static   T* get_cashed_obj(Element * el);

            trail<Vector3>  history;
            ccIndicationTemplateBuffer*   set_templates[ARCADE_SET_MAX];
        private:
            
            marks           various_objects;
    };

    enum arcade_template_elements
    {
        TRG_AIRBORNE,
        TRG_GROUND,
        TRG_MISSION,
        TRG_CURRENT,
        TRG_MLWS_CONTACT,
		TRG_AIRDEFENSE,
        TRG_AIRFIELD,
        MAX_TRG_SYMBOLS
    };

    class COCKPITBASE_API ccArcade_template_buffer : public ccIndicationTemplateBuffer
    {
    public:
        ccArcade_template_buffer(unsigned number,unsigned c);
        virtual ~ccArcade_template_buffer();
        void rebuild			 (Element *start);
        void draw				 (Graphics::RenderParser & parser,unsigned char level);
        void initialize(ccIndicator *parent,lua_State *L,const char *name);
        void build_targets       (avArcadeRadar * arcade);
        void build_mlws_contacts (avMLWS * mlws);
        void build_airfields(const marks & objects);
        void reset() 
        {
            elements.clear();
        }
   protected:
	    virtual int  get_target_type(const arcade_target &trg) const;
        void add_target(const arcade_target &trg,unsigned type);
		void add_target(const arcade_target &trg){	add_target(trg,get_target_type(trg)); };
        void add_target(const mission_target &trg);
        void add_target_list(const arcade_target_list & lst,unsigned type);
		void add_target_list(const arcade_target_list & lst);

        void add_mission_target_list(const mission_target_list & lst);

        ccElementList   elements;
        Element**       specific_elements;
        unsigned		number_of_elements;
        unsigned        cash;
    };
}

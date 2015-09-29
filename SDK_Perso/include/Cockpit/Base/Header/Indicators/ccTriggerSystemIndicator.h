#pragma once;
#include "CockpitBase.h"
#include "Indicators/ccIndicator.h"
#include "Indicators/ccIndicationTemplateBuffer.h"
#include "Utilities/avDrawCashedObj.h"
#include "Indicators/ccIndicatorPageImpl.h"

namespace cockpit { namespace triggers { 

    class  COCKPITBASE_API trigger_highlight : public avDrawCashedObj
    { 
    public:
        trigger_highlight()      
        {
          size_x            = -1;
          size_y            = -1; 
          connector         = 0;
          visible           = true;
          outside_screen    = false;
          start_time        = 0;
          off_time          = -100;
          animation_visible = false;
          target_type       = HIGHLIGHT_POS;
        }
        enum what_is_highlighted
        {
            HIGHLIGHT_POS,
            HIGHLIGHT_CONNECTOR,
            HIGHLIGHT_INDICATION,
        };
        what_is_highlighted        target_type;
        void                       set_visible(bool val);
        float                      size_x;
        float                      size_y;
        Vector3                    custom_size;
        Graphics::ModelConnector * connector;
        Position3                  pos;
        bool                       visible;
        double                     start_time;
        double                     off_time;
//      indication element highlight
        unsigned                   indicator_id;
        ed::string                element_name;
//
        mutable bool               outside_screen;
        mutable bool               animation_visible;
    };
    typedef ed::map<unsigned,trigger_highlight*> highlights;
    class ccTriggerSystem_template_buffer;

    class COCKPITBASE_API ccTriggerSystemIndicator : public ccIndicator
    {
        public:
            ccTriggerSystemIndicator();
            virtual ~ccTriggerSystemIndicator();
            virtual void initialize(avDevice* dev, unsigned char id, const ed::string& script);
            virtual void create_elements_controllers_map();
            virtual void draw_temporal_set(Element *start_point,unsigned temporal_set,unsigned char level);

            void         highlight       (unsigned id,const Vector3 & pnt        ,const Vector3 & size);
            void         highlight       (unsigned id,const char * connector_name,const Vector3 & size);
            void         highlight_indication_element(unsigned id,
                                                      unsigned id_of_indicator,
                                                      const char * element_name,
                                                      const Vector3 & size);
            void         remove_highlight(unsigned id);   
            bool         is_highlight_visible(unsigned id) const;  

            static void   reset_buffer      (Element* element, ccDrawable* parent, const DParamList& args);
            static void   draw_highlights   (Element* element, ccDrawable* parent, const DParamList& args);
            static void   apply_buffer      (Element* element, ccDrawable* parent, const DParamList& args);
            static void   highlight_position(Element* element, ccDrawable* parent, const DParamList& args);
            static void   start_and_fade_out_animation   (Element* element, ccDrawable* parent, const DParamList& args); 
            static void   size_of_element   (Element* element, ccDrawable* parent, const DParamList& args);
            static void   direction_arrow   (Element* element, ccDrawable* parent, const DParamList& args);
        protected:
            template<class T>
            static   T* get_cashed_obj(Element * el);
        private:
            ccTriggerSystem_template_buffer*   set_template;
            highlights          highlights_map;
            trigger_highlight * check(unsigned id,bool create);
            const trigger_highlight * check(unsigned id) const;


            Position3 orient_pos;
            friend void dbg_draw();
   };

    class ccTriggerSystem_template_buffer : public ccIndicationTemplateBuffer
    {
    public:
        ccTriggerSystem_template_buffer(unsigned number,unsigned c);
        virtual ~ccTriggerSystem_template_buffer();
        void rebuild			 (Element *start);
        void draw				 (Graphics::RenderParser & parser,unsigned char level);
        void initialize(ccIndicator *parent,lua_State *L,const char *name);
        void reset() 
        {
            elements.clear();
        }
        void add_highlight(trigger_highlight & h);
   protected:
        ccElementList   elements;
        Element**       specific_elements;
        unsigned		number_of_elements;
        unsigned        cash;
    };
}}

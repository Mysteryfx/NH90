#pragma once
#include "Indicators/ccIndicator.h"
#include <thread>
#include <mutex>
#include "ed/semaphore.h"

struct IDirect3DSurface9;

class UDP_transport;

namespace cockpit {

    class COCKPITBASE_API ccVideoStreamer : public ccIndicator
    {
    public:
        ccVideoStreamer();
        virtual ~ccVideoStreamer();; 
        virtual void initialize(avDevice* dev , unsigned char id , const ed::string& script);
        virtual void post_initialize();

        virtual bool get_render_target_always() const;

        virtual const Graphics::Camera & get_camera();
        virtual dVector			  get_camera_point()  { return dVector();} 
        virtual TVScreenCondition get_screen_condition();

        virtual void              draw(unsigned char level);
    private:

        ccDrawable * get_source();
        void         read_target();

        int  source_indicator_id;
        bool is_server;
        bool collapse_to_grayscale;

        indicator_viewport viewport;
		//tango-lima-bravo
        //Graphics::DXTexture * source_render_target;
        //IDirect3DSurface9   * tmp_surface;
        //////////////////////////////////////////////////////////////////////////
        void           start_send_thread();
        //////////////////////////////////////////////////////////////////////////
        //Thread shared data
        char         * data;
        unsigned       data_size;
        //Safety//////////////////////////////////////////////////////////////////
        std::mutex     picture_guard;
        ed::semaphore  picture_ready;
        //////////////////////////////////////////////////////////////////////////
        //Send thread related data
        std::thread picture_send_thread;
        //////////////////////////////////////////////////////////////////////////
        volatile  int  picture_send;
        UDP_transport * transport;
        unsigned       next_offset;
        static void    thread_send_func(void * arg);
        void           send_impl();
        //Server//////////////////////////////////////////////////////////////////
        void           send_picture_batch();
        //Client//////////////////////////////////////////////////////////////////
        void           receive_picture_batch();
        //////////////////////////////////////////////////////////////////////////
    };
}
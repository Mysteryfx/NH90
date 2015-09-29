#pragma once
#include "CockpitBase.h"
#include "Renderer/DynamicParams.h"

namespace cockpit
{ 
    class avDevice;
    class ccDrawable;
	class ccCockpitContext;
    namespace triggers
    {
        class input_listener;
        class event_listener;
    };

    COCKPITBASE_API    void  listen_command           (avDevice                           * device   ,int command);//register to receive global command
    COCKPITBASE_API    void  listen_command           (ccDrawable                         * indicator,int command);//register to receive global command
    COCKPITBASE_API    void  listen_command           (const triggers::input_listener  & trigger_event,int command);//register to receive global command


	COCKPITBASE_API    void  listen_command_range     (avDevice    * device   ,int start,int end);//register to receive global command (start and end include)
	COCKPITBASE_API    void  listen_command_range     (ccDrawable  * indicator,int start,int end);//register to receive global command (start and end include)
    COCKPITBASE_API    void  listen_event             (avDevice    * device                           ,const ed::string & ev);//register to receive global event
	COCKPITBASE_API    void  listen_event             (ccDrawable  * indicator                        ,const ed::string & ev);//register to receive global event
    COCKPITBASE_API    void  listen_event             (const triggers::event_listener  & trigger_event,const ed::string & ev);//register to receive global event

    COCKPITBASE_API    void  break_listen_commands    (avDevice    * device);
    COCKPITBASE_API    void  break_listen_commands    (ccDrawable  * indicator);
    COCKPITBASE_API    void  break_listen_command_by_triggers(int command);
    COCKPITBASE_API    void  distribute_global_command(int command,float value);
    COCKPITBASE_API    void  distribute_global_event  (const ed::string & event,
                                                       const Graphics::DParamList & params);

    COCKPITBASE_API    void  clear_commands_handler();
	COCKPITBASE_API    void  clear_commands_handler_for_context(ccCockpitContext * context);
}
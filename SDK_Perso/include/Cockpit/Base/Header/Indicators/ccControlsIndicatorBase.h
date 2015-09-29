#pragma once
#include "Indicators/ccIndicator.h"
#include "CockpitBase.h"

namespace cockpit   {  

	class avFMProxyBase;

    class COCKPITBASE_API ccControlsIndicatorBase : public ccIndicator
    {
    public:
        ccControlsIndicatorBase(){ show_ = false;}
       ~ccControlsIndicatorBase(){}

       virtual void create_elements_controllers_map();
       void initialize(avDevice* _controller, unsigned char ID, const ed::string& script);
       void SetCommand(int command,float value);

       static void stick_roll   (Element* element, ccDrawable* parent, const DParamList& args);
       static void stick_pitch  (Element* element, ccDrawable* parent, const DParamList& args);
       static void rudder       (Element* element, ccDrawable* parent, const DParamList& args);
       static void throttle     (Element* element, ccDrawable* parent, const DParamList& args);
       static void show         (Element* element, ccDrawable* parent, const DParamList& args);
       static void parking_brake(Element* element, ccDrawable* parent, const DParamList& args);
       static void brakes_value (Element* element, ccDrawable* parent, const DParamList& args);
       static void brakes_value_actual (Element* element, ccDrawable* parent, const DParamList& args);

	    DECLARE_IND_LINKPTR(avFMProxyBase,FM_proxy);
  protected:
       bool show_;
    };
}
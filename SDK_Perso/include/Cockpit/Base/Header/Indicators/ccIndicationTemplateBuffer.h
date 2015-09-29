#pragma once
#include "CockpitBase.h"

struct lua_State;
namespace Graphics
{
	class RenderParser;
};
namespace cockpit
{
	class  ccIndicatorPage;
	class  ccIndicator;
	class  Element;
	class COCKPITBASE_API ccIndicationTemplateBuffer
	{
	public:
		ccIndicationTemplateBuffer():template_changed(false),template_page(0){};
		virtual ~ccIndicationTemplateBuffer(){};
		virtual	void rebuild			 (Element *start)=0;
		virtual void draw				 (Graphics::RenderParser & parser,unsigned char level)=0;
		void		 set_template		 (ccIndicatorPage	   	     *_template_page);
		virtual void initialize(ccIndicator *parent,lua_State *L,const char *_name);
		virtual void on_change_mode(ccIndicator *parent,lua_State *L);
	protected:
		ccIndicatorPage			   	     *template_page;
		bool                              template_changed;
		ed::string                       name;
	};
}


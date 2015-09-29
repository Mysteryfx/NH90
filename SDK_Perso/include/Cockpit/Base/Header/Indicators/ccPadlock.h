#pragma once
#include "Indicators\ccIndicator.h"

namespace cockpit
{
	//class avPadlock;
	class ccPadlock: public ccIndicator
	{
	public:
		ccPadlock(void);
		virtual ~ccPadlock(void);
		void create_elements_controllers_map();
		static void padlock_mode(Element* element, ccDrawable* parent, const DParamList& args);
		static void padlock_point(Element* element, ccDrawable* parent, const DParamList& args);
	protected:
		
	};
}



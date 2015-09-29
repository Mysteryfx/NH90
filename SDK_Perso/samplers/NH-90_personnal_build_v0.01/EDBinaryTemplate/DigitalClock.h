// EDBinaryTemplate.cpp : Defines the exported functions for the DLL application.
//
#include "dllapi.h"
#include "Indicators/ccIndicator.h"

namespace cockpit
{
	namespace Samples
	{
		class avSampleSystem;

		class DigitalClock : public ccIndicator
		{
			double time;
			unsigned next_page;

			avSampleSystem* gun_sight;

		public:
			DigitalClock();
			~DigitalClock();
			void create_elements_controllers_map();
			void control();
			void initialize(avDevice* parent, unsigned char id, const ed::string& script);
			void update();

		private:
			void select_page(unsigned page);
			//utility for mirror glass , set origin back to cockpit shape as to ease use of exported geometry from MAX
			// in this sample i use it to show glass exported from Su-27 cockpit shape
			// this glass need to us to make default clipping 
			static void set_origin_to_cockpit_shape(Element* elem, ccDrawable* parent, const DParamList& args);
			static void txt_pitch(Element* elem, ccDrawable* parent, const DParamList& args);
		};
	}
}

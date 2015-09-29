#pragma once
#include "Indicators/ccIndicator.h"
#include "Avionics/avDevice.h"


namespace cockpit    {

	class ccCargoIndicatorBase : public ccIndicator
	{
	public:
		ccCargoIndicatorBase(){}
		virtual ~ccCargoIndicatorBase(){}

		virtual void create_elements_controllers_map();
		static void  radius(Element* element, ccDrawable* parent, const DParamList& args);
		static void  circle_position(Element* element, ccDrawable* parent, const DParamList& args);
		static void  narrow_position(Element* element, ccDrawable* parent, const DParamList& args);
		static void  height_narrow_position(Element* element, ccDrawable* parent, const DParamList& args);
		static void  show (Element* element, ccDrawable* parent, const DParamList& args);
		virtual void OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
		void initialize(avDevice* _controller, unsigned char ID, const ed::string& script);
		void SetCommand(int command,float value);

		//доступ
		const bool					getNarrowShow()	const { return isNarrowShow; }
		const bool					getCircleShow()	const { return isCircleShow; }
		const bool					getNarrowHeightShow()	const { return isNarrowHeightShow; }
		const bool					getShow()	const { return isShow; }
		const bool					getIsCargoIndicator()	const { return is_cargo_indicator; }
		const double &				getCargoNarrow_x()	const { return CargoNarrow_x; }
		const double &				getCargoNarrow_y()	const { return CargoNarrow_y; }
		const double &				getCargoHeightNarrow()	const { return CargoHeightNarrow; }
		const Math::Vec3d &	        getCircle()	const { return cargo_circle;}

	protected:
		double				CargoNarrow_x;
		double				CargoNarrow_y;
		double				CargoHeightNarrow;
		bool				isNarrowShow;
		bool                isCircleShow;
		bool				isNarrowHeightShow;
		bool				isShow;
		bool				is_cargo_indicator;
		Math::Vec3d			cargo_circle;


	};
}
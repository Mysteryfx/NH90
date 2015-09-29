#pragma once
#include "Avionics\avDevice.h"
#include "Avionics\avBasicElectric.h"
#include "Avionics\avBreakable.h"

namespace cockpit
{
	enum enum_HelmetType
	{
		HELMETTYPE_MINIHUD,
		HELMETTYPE_SIGHT,
		HELMETTYPE_NVG
	};

	class COCKPITBASE_API avHelmet : public avDevice,
									 public avBreakable, 
									 public avBasicElectric
	{
	public:
		avHelmet(void);
		virtual ~avHelmet(void);

		virtual void initialize(unsigned char id, const ed::string& name, const ed::string& script);
		virtual void release();
		virtual void serialize(Serializer& serializer);
		virtual void SetCommand(int command, float value = 0);
        bool         checkAnimationConditions(int arg_num, int command, float& value);

		virtual void setElecPower(bool flag_in);

		virtual unsigned char helmet_type() const { return HELMETTYPE_MINIHUD;}
        virtual unsigned      helmet_product_type() const { return 0;}//уточнение типа,(поколениеб модификация страна производства и т.д.) 
		double      get_brightness() const { return brightness;}

	protected:
		float brightness;
	};
}


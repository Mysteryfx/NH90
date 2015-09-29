#include "Base.h"

#include <array>
#include <ed/vector.h>
#include "FMMathematics/CoreMath.h"

namespace Lua { class Config; }

namespace EagleFM { 

namespace BaseOptions {

class FMBASE_API PropellerOptions
{
public:

	enum BladeOption_Enum 
	{
		BLADE_R_AVR = 0,
		BLADE_L,
		BLADE_B,
		BLADE_PITCH,
		BLADE_OPT_MAX
	};

	PropellerOptions();

	void l_read(Lua::Config& config, const char* name);

	unsigned char blade_N_; //blades count
	float D_;				//prop diameter
	float pitch_min_;		//minimal pitch
	float pitch_max_;		//maximal pitch
	float pitch_BIAS_;		//degrees
	double moi_;			//prop moment of inertia

	unsigned char elements_N_; //blade elements count
	typedef std::array<float,BLADE_OPT_MAX> BladeOptionArray;
	ed::vector<BladeOptionArray> geometry_; // blades geometry

	Vec3		pos_l;	//prop center

};

}}
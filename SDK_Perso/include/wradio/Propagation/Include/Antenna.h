#ifndef __Antenna__
#define __Antenna__

#include "Definitions.h"

namespace Lua
{
	class Config;
}

namespace Radio
{

class Antenna
{
public:
	Antenna();
	~Antenna();
	void load(Lua::Config & config);
	inline bool empty() const { return impl_ == NULL; }
	Real getPolarization() const { return polarization_; } //Returns polarization
	Gain getSqrG(const Vec3 & direction) const; //Returns sqrt(G), where G - directive gain
	class Impl;
private:
	void clear_();
	Real polarization_; //Polarization, 0 - horizontal, 1 - vertical
	Impl * impl_;
};

}

#endif __Antenna__
#pragma once

#include "Guns/Effects/Gun_Effect.h"
#include <tuple>

namespace weapons_gun
{

class CustomArgumentsDesc : public Gun_Effect_Desc
{
	friend class CustomArguments;
public:
	virtual void l_read(Lua::Config& config) override;
	virtual void create(wAircraftGunMount* gun_mount) override;

private:
	typedef std::tuple<ed::string,int> CustomArg;
	ed::vector<CustomArg> arguments_;
};

class CustomArguments : public SimulateEffect
{
	friend class CustomArgumentsDesc;
public:
	CustomArguments(const CustomArgumentsDesc* desc, wAircraftGunMount* gun_mount);
	virtual void simulate(double dt) override;

	typedef std::tuple<int,double*> PropertyArg;
	ed::vector<PropertyArg> arguments_;
};

}

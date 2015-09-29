#pragma  once
#include <ed/map.h>
#include <ed/vector.h>
#include <ed/string.h>
#include <osg/Vec2d>
#include "Transport.h"

namespace Lua
{
	class Config;
}

struct UnitPlace
{
	UnitPlace(const osg::Vec2d &offset, double dir);
	osg::Vec2d offset;
	double dir;//По часовой стрелке
};

class CustomFormation: public ed::vector<UnitPlace> {};

class TRANSPORT_API CustomFormations
{
public:
	static CustomFormations* instance();
	void load(Lua::Config& cfg);

	const CustomFormation* getFormation(const ed::string& name);

private:
	CustomFormations();
	~CustomFormations();

	typedef ed::map<ed::string, CustomFormation> Formations;
	Formations _templates;
};
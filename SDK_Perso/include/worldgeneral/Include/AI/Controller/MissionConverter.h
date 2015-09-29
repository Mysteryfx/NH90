#ifndef __MissionConverter__
#define __MissionConverter__

//Converts old DCS mission format (route + target zones) to new DCS mission format (route + lists of actions)

#include <ed/string.h>
#include <ed/list.h>
#include <ed/vector.h>

#include "cLinear.h"

namespace Lua
{
	class Config;
}

namespace AI
{

class Task;
class ComboTask;

class MissionConverter
{
protected:
	struct OldTask
	{
	public:
		OldTask();
		void load(Lua::Config & config);
	public:
		E2::Vector	point;
		typedef ed::string Category;
		typedef ed::list<Category> Categories;
		Categories	categories;
		float		radius;
	};
	MissionConverter();
public:
	Task * convert(Lua::Config & config);
protected:
	class TaskConverter
	{
	public:
		virtual Task * convert(const OldTask & oldTask) = 0;
	};
	typedef ed::vector<TaskConverter*>	TaskConverters;
	TaskConverters 						taskConverters;
private:
	void load_(Lua::Config & config);
	void convert_(const OldTask & oldTask, ComboTask * & pComboTask);
};

class AircraftMissionConverter : public MissionConverter
{
public:
	static AircraftMissionConverter & instance();
private:
	AircraftMissionConverter();
	class EngageTargetsInZoneConverter : 	public TaskConverter
	{
	public:
		virtual Task * convert(const MissionConverter::OldTask & oldTask);
	private:
		bool isPossible_(const MissionConverter::OldTask & oldTask) const;
	};
	class BombingConverter : 				public TaskConverter
	{
	public:
		virtual Task * convert(const MissionConverter::OldTask & oldTask);
	};
	class BombingRunwayConverter : 			public TaskConverter
	{
	public:
		virtual Task * convert(const MissionConverter::OldTask & oldTask);
	};
};

class VehicleMissionConverter : public MissionConverter
{
public:
	static VehicleMissionConverter & instance();
	VehicleMissionConverter();
	class FireAtPointConverter : 			public TaskConverter
	{
	public:
		virtual Task * convert(const MissionConverter::OldTask & oldTask);
	};
};

}

#endif
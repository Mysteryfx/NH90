#pragma once
#include "Avionics/avDevice.h"
#include "Avionics/avMovingMap.h"
#include "ICommandsCockpitDevice.h"


namespace AI
{
	class Task;
}

namespace cockpit {

struct kneeboard_map_page
{
	double   volume_size;
	unsigned chart_id;
	double   rotation;
	double   latitude;
	double   longitude;
};

class avMovingMapPoint;
class SimpleOctTree;

enum kneeboard_commands
{
	KNEEBOARD_NEXT_PAGE = iCommandCockpitDevice_Action_1,
	KNEEBOARD_PREV_PAGE,
	KNEEBOARD_MAKE_MARK,
};

enum kneeboard_point_type
{
	PTYPE_WAYPOINT,
	PTYPE_SELF_MARK_POINT,
};

struct TaskRelatedData
{
	AI::Task					* task;
	ed::vector<avMovingMapPoint*> marks;

	void						  cleanup();
};

class COCKPITBASE_API avKneeboard : public avDevice,
									public avMovingMap
{
	kneeboard_map_page default_map;

	bool	 use_charts_;
	double   lat_lon_aspect_;
	unsigned map_page_id_;
	ed::vector<kneeboard_map_page> map_pages_;

	void next_page();
	void prev_page();
public:
	avKneeboard();
	virtual ~avKneeboard();

	virtual double	 get_volume_size()			    const;
	virtual bool	 get_map_rotation(double & h);

	virtual void	 initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name);
	virtual void	 update();

	virtual void	 SetCommand(int command,float value = 0);

	const kneeboard_map_page & get_map() const;

	unsigned				   get_page_id() const { return map_page_id_; }
	unsigned				   form_page_id_for_indicator () const;

	void					   set_page_id_from_indicator(unsigned);

	double get_symbol_latitude   (avMovingMapPoint * point) const;
	double get_symbol_longitude  (avMovingMapPoint * point) const;
	void   set_symbol_soordinates(avMovingMapPoint * point,double latitude,double longitude);
	void   set_symbol_heading    (avMovingMapPoint * point,double heading);


	SimpleOctTree * get_storage() { return storage; }

	SimpleOctTree * get_zones_storage() { return zones_storage; }

	const ed::vector<avMovingMapPoint*> & get_route() const { return route;} 


	unsigned	get_change_counter() const { return change_counter_; }

	void		set_user_charts_count( unsigned count) { pages_count = map_pages_.size() + count; }

	void		OnCockpitEvent(const ed::string & event, const Graphics::DParamList & params);
protected:
	virtual Vector3	 get_camera_point()    const;

	void     check_camera_scale()
	{
		//nothing to do
	};

	void	 check_camera_position()
	{
		//nothing to do
	};

	void	init_waypoints();
	void	init_zones();

	void	mark_self();

	void	update_change_counter()
	{
		if (change_counter_)
			change_counter_--;
		else 
			change_counter_ = INT_MAX;
	}
private:
	SimpleOctTree				 * storage;
	SimpleOctTree				 * zones_storage;
	ed::vector<avMovingMapPoint*> route;
	ed::vector<avMovingMapPoint*> marks;

	unsigned					   pages_count;

	unsigned					   change_counter_;

	bool						   arcade_mode_;

	TaskRelatedData				   task_data;
};

}
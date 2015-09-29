#pragma once
#include "Indicators\ccIndicator.h"
#include "IwHumanPLane.h"
#include "Indicators\ccMovingMap.h"

namespace cockpit {
class avKneeboardZoneObject;

class COCKPITBASE_API  ccKneeboardObjectsBuffer : public ccMapObjectsBuffer
{
	static const  unsigned MAX_ELEMENTS = 3;
public:
	ccKneeboardObjectsBuffer() : ccMapObjectsBuffer(MAX_ELEMENTS) {};

	void add_zone(avKneeboardZoneObject * zone);
protected:
	Element					* get_element(avMovingMapPoint *wpnt);
	ccElementList::iterator	  where_insert(Element * el);
};

class COCKPITBASE_API ccKneeboard : public ccIndicator
{
	friend class KneeboardCheckMissionFiles;				
	ccKneeboardObjectsBuffer * objects_buffer;
public:
	ccKneeboard();
	virtual ~ccKneeboard();

	void initialize(avDevice* boss, unsigned char ID, const std::string& script_name);

	void release();

	TVScreenCondition get_screen_condition();


	void create_elements_controllers_map();

	const Graphics::Camera & get_camera();
	dVector	get_camera_point();

	void update(unsigned char level);

	void draw_to_render_target();

	void draw_temporal_set(Element *start_point,unsigned temporal_set,unsigned char level);;


	void SetCommand(int command , float value);

	bool check_chunk(const std::string & name);
	bool apply_image_as_page(Graphics::Texture * tex);

protected:
	void			 l_register_functions(lua_State *Lua_s,int index);
	static int		 l_add_map_page(lua_State *Lua_s);

	static void	  chart_position(Element* element, ccDrawable* parent, const DParamList& args);
	static void	  waypoint_position(Element* element, ccDrawable* parent, const DParamList& args);
	static void	  waypoint_name(Element* element, ccDrawable* parent, const DParamList& args);

	static void	  draw_objects(Element* element, ccDrawable* parent, const DParamList& args);
	static void	  flight_plan_line(Element* element, ccDrawable* parent, const DParamList& args);
	static void	  zone_position(Element* element, ccDrawable* parent, const DParamList& args);

	ccMovingMap	* map_subset;
	std::string	  map_subset_file;

	void update_dirty_counter()
	{

	}

	void relative_map_position(Position3 & pos, const DParamList& args);
	void relative_map_position(Vector3   & pnt, const DParamList& args);
	unsigned	  picture_dirty_;
	unsigned	  last_change_counter;


};

}
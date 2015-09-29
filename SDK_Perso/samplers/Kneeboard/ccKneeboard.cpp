#include "stdafx.h"
#include "Indicators/ccKneeboard.h"
#include "WorldManager.h"
#include "Indicators/ccMovingMap.h"
#include "Avionics/avKneeboard.h"
#include "Renderer\Renderer.h"
#include "Avionics\avMovingMapPoint.h"
#include "Indicators/ccIndication.h"
#include "Avionics/avKneeboardObjects.h"
#include "Indicators\ceStringPoly.h"
#include "Mission\mission_package.h"
#include "IDispatcher.h"
#include "Resourcer/ResourceManager.h"
#include "Renderer/Texture.h"
#include "Renderer\CustomMaterial.h"
#include "IView.h"
using namespace cockpit;

REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::ccKneeboard>, ccKneeboard);

static const int DIRTY_COUNTER_ = 30;

ccKneeboard::ccKneeboard():
map_subset(0),
picture_dirty_(DIRTY_COUNTER_),
objects_buffer(NULL),
last_change_counter(0)
{
	objects_buffer = new ccKneeboardObjectsBuffer;
}

ccKneeboard::~ccKneeboard()
{

}

void ccKneeboard::release()
{
	ccIndicator::release();
	if (objects_buffer)
	{
		delete objects_buffer;
		objects_buffer = 0; 
	}
}

void ccKneeboard::l_register_functions( lua_State *Lua_s,int index )
{
	ccIndicator::l_register_functions(Lua_s,index);
	lua_pushstring(Lua_s, "Add_Map_Page");	 lua_pushcfunction(Lua_s,ccKneeboard::l_add_map_page);	lua_settable  (Lua_s,index);
}

int ccKneeboard::l_add_map_page(lua_State *Lua_s)
{
	ccKneeboard * device = l_get_device_from_meta<ccKneeboard>(Lua_s);
	if (device == NULL)
		return 0;

	unsigned char id			   = static_cast<unsigned char>(lua_tonumber(Lua_s,2));
	device->map_subset_file = lua_tostring(Lua_s,3);
	device->map_subset	    = new ccMovingMap();
	if (device->map_subset)
	{
		device->map_subset->setParent(device);
		device->map_subset->setSize(device->HalfWidth,device->HalfHeight);
		device->map_subset->setViewDistance(device->init_pos.p.x);
		device->ElementSets[id] = device->map_subset;
	}
	return  1;
}

void ccKneeboard::initialize(avDevice* boss, unsigned char ID, const std::string& script_name)
{
	ccIndicator::initialize(boss,ID,script_name);
	if(map_subset)
	{
		map_subset->initialize(map_subset_file);
		map_subset->update();
	}
	if (objects_buffer)
	{
		objects_buffer->set_cash(0);
		objects_buffer->initialize(this,L,"OBJECTS");
	}


	class KneeboardCheckMissionFiles : public Mission::MissionPackageChunkCheck
	{
		ccKneeboard * kneeboard;
	public:
		KneeboardCheckMissionFiles(ccKneeboard * kneeboard_):kneeboard(kneeboard_)
		{
			result = 0;
		}
		virtual bool check(const std::string & chunk_name)
		{
			if (kneeboard->check_chunk(chunk_name))
			{
				result++;
				true;
			}
			return false;
		}
		int result;
	};

	Lua::Config config(L);


	if (config.open("custom_images"))
	{
		int i = 0;
		std::string image;
		while(config.get(++i,&image))
		{
			Graphics::Texture * tex = 0;
			if (resourcer->IsResourceExists(rtTexture,image))
				tex = (Graphics::Texture*)resourcer->GetResource     (rtTexture,image);
			else
			{
				tex = (Graphics::Texture*)resourcer->GetResource     (rtTexture,image);
				tex->LoadToStep(1);
				tex->SetName(image);
				tex->Register();	
			}
			apply_image_as_page(tex);
		}
		config.pop();
	}


	KneeboardCheckMissionFiles checker(this);

	globalDispatcher->getMission()->forEachChunk(checker);

	int count = config.get("number_of_additional_pages",0) + checker.result;
	avKneeboard * kneeboard = dynamic_cast<avKneeboard*>(boss);
	if (kneeboard && count > 0)
		kneeboard->set_user_charts_count(count);

	listen_command(this, iCommandPlaneShowKneeboard);
}


TVScreenCondition ccKneeboard::get_screen_condition()
{	
	avKneeboard * kneeboard = static_cast<avKneeboard*>(get_controller());	
	if (kneeboard)
	{
		unsigned change_counter  = kneeboard->get_change_counter(); 
		if (last_change_counter != change_counter)
		{
			last_change_counter  = change_counter;
			picture_dirty_		 = DIRTY_COUNTER_;
		}
	}
	if (picture_dirty_)
		render_screen_type = INDICATION_ONLY;
	else
		render_screen_type = FROZEN_PICTURE;
	return render_screen_type;
}

//////////////////////////////////////////////////////////////////////////
void  ccKneeboard::create_elements_controllers_map()
{
	ccIndicator::create_elements_controllers_map();
	elements_controllers["chart_position"]		= &chart_position;
	elements_controllers["waypoint_position"]	= &waypoint_position;
	elements_controllers["waypoint_name"]		= &waypoint_name;
	elements_controllers["draw_objects"]		= &draw_objects;
	elements_controllers["flight_plan_line"]	= &flight_plan_line;
	elements_controllers["zone_position"]		= &zone_position;
}


void ccKneeboard::chart_position(Element* element, ccDrawable* parent, const DParamList& args)
{
	ccKneeboard * kneeboard = static_cast<ccKneeboard*>(parent);
	Position3 pos;
	pos.p.x  = args[0];
	pos.p.z  = args[1];
	float k  = args[2];
	float   volume_size = kneeboard->render_camera.getVolume().max.x;
	pos		*= ~kneeboard->render_camera.getPos();
	pos.p.y	*= k / volume_size;
	pos.p.z	*= k / volume_size;

	Position3 & ref  = element->get_position_ref();
	ref.p += pos.p.z * ref.z;
	ref.p += pos.p.y * ref.y;
	ref.RotateX(atan2(-pos.x.z,pos.x.y));
	//scale
	ref.y /= volume_size;
	ref.z /= volume_size;
}


void ccKneeboard::relative_map_position(Position3 & pos,const DParamList& args )
{
	pos		*= ~render_camera.getPos();
	pos.p   /=  render_camera.getVolume().max.x;
	pos.p   *=  args[0];
	pos.p.x	 =  0;
}

void ccKneeboard::relative_map_position(Vector3   & pnt, const DParamList& args)
{
	pnt		*= ~render_camera.getPos();
	pnt     /=  render_camera.getVolume().max.x;
	pnt     *=  args[0];
	pnt.x	 =  0;
}

const Graphics::Camera & ccKneeboard::get_camera()
{
	return render_camera;
}

dVector ccKneeboard::get_camera_point()
{
	return render_camera.getPos().p;
}


void ccKneeboard::draw_to_render_target()
{
	indicator_viewport viewport;

	viewport.x = 0;
	viewport.y = 0;
	viewport.w = renderer->GetTargetWidth();
	viewport.h = renderer->GetTargetHeight();

	int current_purpose = get_render_purpose(true);
	push_purpose(current_purpose);
	//////////////////////////////////////////////////////////////////////////
	draw_to_viewport(viewport);
	//////////////////////////////////////////////////////////////////////////
	pop_purpose();

	if (picture_dirty_)
		picture_dirty_--;
}

void ccKneeboard::update( unsigned char level )
{
	avKneeboard * kneeboard = static_cast<avKneeboard*>(get_controller());	
	if (kneeboard)
	{
		unsigned p					= kneeboard->form_page_id_for_indicator();
		ccLuaPageMap::iterator page = PageMap.find(p + 1);


		if (page != PageMap.end())
			current_page = &(page->second);

		if (p == 0) //map
		{
			if (level == DRAW_LEVEL_COMMON)
			{
				float aspect  = GetHalfHeight() / GetHalfWidth(); 
				render_camera = kneeboard->get_camera(true,-1,1,-aspect,aspect);
			}
			if  (map_subset)
			{
				 map_subset->set_current_level(kneeboard->get_map().chart_id);
			}
		}
		else
		{
			float aspect  = GetHalfHeight() / GetHalfWidth(); 
			

			render_camera.editPos().x =  Vector3(1,0,0);
			render_camera.editPos().y =  Vector3(0,1,0);
			render_camera.editPos().z =  Vector3(0,0,1);
			render_camera.editPos().RotateZ(Pi_05);
			render_camera.editPos().p =  Vector3();
			render_camera.editVolume().min.set(-1,-aspect,-0.1f);   
			render_camera.editVolume().max.set( 1, aspect, 2.4f);
		}
	}
	ccIndicator::update(level);
}

void ccKneeboard::draw_temporal_set( Element *start_point,unsigned temporal_set,unsigned char level )
{
	render_parser.set_reset_renderer_transform(true);
	if (objects_buffer)
		objects_buffer->draw(render_parser,level);
}

//////////////////////////////////////////////////////////////////////////
class kneeboard_draw_checker : public SimpleOctTree_check_object,
							   public std::less<avMovingMapPoint*> 
{
public:
	typedef	avMovingMapPoint*	   result_ptr;
	typedef ed::vector<result_ptr> result_list;

	kneeboard_draw_checker(){}

	virtual bool valid(SimpleOctTreeLoad *obj)
	{
		result_ptr pnt = static_cast<avMovingMapPoint*>(obj);
		if (pnt->hidden())
			return false;
		result.insert(where(pnt), pnt);
		return true;
	}

	virtual bool operator()(const result_ptr & _Left ,const result_ptr & _Right) const
	{
        return get_priority(_Left) < get_priority(_Right);
 	}

	result_list result;
protected:
	result_list::iterator where(const result_ptr & inserted)
	{
		return std::lower_bound<result_list::iterator,
			                    result_ptr,
			                    kneeboard_draw_checker>
                                (result.begin(), result.end(), inserted, *this);
	}
    int get_priority(const result_ptr & ptr) const
    {
        return 0;
    } 
};


class kneeboard_draw_checker_zone : public SimpleOctTree_check_object
{
public:
	typedef	avKneeboardZoneObject* zone_ptr;
	typedef ed::vector<zone_ptr>   zone_list;

	kneeboard_draw_checker_zone(){}

	virtual bool valid(SimpleOctTreeLoad *obj)
	{
		zone_ptr pnt = static_cast<avKneeboardZoneObject*>(obj);
		if (pnt->hidden())
			return false;
		result.push_back(pnt);
		return true;
	}
	zone_list result;
};

Element* ccKneeboardObjectsBuffer::get_element(avMovingMapPoint *wpt)
{
	if (wpt->getType() == PTYPE_SELF_MARK_POINT)
		return get_template_by_index(2);
	return get_template_by_index(0);
}

ccElementList::iterator	ccKneeboardObjectsBuffer::where_insert(Element * el)
{
	return elements.end();
}

void ccKneeboard::draw_objects(Element* element, ccDrawable* parent, const DParamList& args)
{
	ccKneeboard  *indicator = static_cast<ccKneeboard*>(parent);
	avKneeboard  *kneeboard = static_cast<avKneeboard*>(indicator->get_controller());
	if (!kneeboard || 
		!indicator->objects_buffer)
		return;

	indicator->objects_buffer->reset();
	if (!kneeboard->get_storage() && 
		!kneeboard->get_zones_storage())
		return;

	ClipVolumePlanes volume;

	Vector3 volume_min   = indicator->render_camera.getVolume().min;
	Vector3 volume_max   = indicator->render_camera.getVolume().max;

	std::swap(volume_min.x,volume_min.z);
	std::swap(volume_max.x,volume_max.z);

	volume_min *= indicator->render_camera.getPos();
	volume_max *= indicator->render_camera.getPos();

	volume.AddPlane(Plane(volume_min,  -indicator->render_camera.getPos().y));
	volume.AddPlane(Plane(volume_min,  -indicator->render_camera.getPos().z));
	volume.AddPlane(Plane(volume_max,   indicator->render_camera.getPos().y));
	volume.AddPlane(Plane(volume_max,   indicator->render_camera.getPos().z));
	volume.Prepare(0,0,0,0,0);

	kneeboard_draw_checker		checker;
	kneeboard_draw_checker_zone checker_zone;

	if (kneeboard->get_zones_storage())
		kneeboard->get_zones_storage()->Walk(checker_zone,&volume);

	if (kneeboard->get_storage())
		kneeboard->get_storage()->Walk(checker,&volume);

	for (kneeboard_draw_checker_zone::zone_list::iterator   it_z   = checker_zone.result.begin();
															it_z  != checker_zone.result.end();
														  ++it_z)
	{
		indicator->objects_buffer->add_zone(*it_z);
	}

	for (kneeboard_draw_checker::result_list::iterator it_wpt   = checker.result.begin();
													   it_wpt  != checker.result.end();
													 ++it_wpt)
	{
		indicator->objects_buffer->add_waypoint(*it_wpt);
	}
	indicator->objects_buffer->rebuild(element);
	element->set_use_temporal_set(true,0);
}


void ccKneeboard::waypoint_name(Element* element, ccDrawable* parent, const DParamList& args)
{
	ccKneeboard  *indicator = static_cast<ccKneeboard*>(parent);
	avKneeboard  *kneeboard = static_cast<avKneeboard*>(indicator->get_controller());
	const DParamList & elem_params = element->get_params();
	if  (!kneeboard || elem_params.empty())
		return;
	avMovingMapPoint* pnt = void_cast<avMovingMapPoint>(elem_params[0].p);
	if (!pnt)
	{
		element->set_isDraw(false);
		return;
	}
	if (args.size() > 0)
	{
		if (args[1] < 1)
		{
			if (pnt->getNumber() > -1)
			{
				static_cast<ceStringPoly*>(element)->set_strcat("%d",pnt->getNumber());
			}
			return;
		}
	}
	static_cast<ceStringPoly*>(element)->set_strcat_by_predefined_format(args[0],pnt->getName().c_str());
}


void ccKneeboard::waypoint_position(Element* element, ccDrawable* parent, const DParamList& args)
{
	ccKneeboard  *indicator = static_cast<ccKneeboard*>(parent);
	avKneeboard  *kneeboard = static_cast<avKneeboard*>(indicator->get_controller());
	const DParamList & elem_params = element->get_params();
	if  (!kneeboard || elem_params.empty())
		return;
	avMovingMapPoint* pnt = void_cast<avMovingMapPoint>(elem_params[0].p);
	if (pnt)
	{
		Position3	 ret		 = pnt->getPosition();
		indicator->relative_map_position(ret,args);
		Position3 &  ref		  = element->get_position_ref();
		ref.p  += ret.p.z * ref.z + 
				  ret.p.y * ref.y;
		ref.RotateX(atan2(-ret.x.z, ret.x.y));
	}
}

void ccKneeboard::flight_plan_line(Element* element, ccDrawable* parent, const DParamList& args)
{
	ccKneeboard  *indicator = static_cast<ccKneeboard*>(parent);
	avKneeboard  *kneeboard = static_cast<avKneeboard*>(indicator->get_controller());
	ceSimpleLineObject * route_line		 = static_cast<ceSimpleLineObject*>(element);
	if (!kneeboard)
		return;
	const ed::vector<avMovingMapPoint*> & route = kneeboard->get_route();
	if (route_line->get_points_number() != route.size())
		route_line->reset();
	for (int i = 0; i < route.size() ; ++i)
	{
		Vector3 pnt = toVec3(route[i]->getPosition().p);
		indicator->relative_map_position(pnt,args);
		route_line->setPointAtIndex(i,pnt,true);
	}
}

void ccKneeboard::zone_position( Element* element, ccDrawable* parent, const DParamList& args )
{
	ccKneeboard  *indicator = static_cast<ccKneeboard*>(parent);
	avKneeboard  *kneeboard = static_cast<avKneeboard*>(indicator->get_controller());
	const DParamList & elem_params = element->get_params();
	if  (!kneeboard || elem_params.empty())
		return;
	avKneeboardZoneObject* zone = void_cast<avKneeboardZoneObject>(elem_params[0].p);
	ceSimpleLineObject * obj = static_cast<ceSimpleLineObject*>(element);
	if (zone && obj)
	{
		if (obj->get_points_number() != zone->get_points().size())
			obj->reset();
		for (int i = 0; i < zone->get_points().size();++i)
		{
			Vector3 pnt = zone->get_points()[i];
			indicator->relative_map_position(pnt,args);
			obj->setPointAtIndex(i,pnt,true);
		}
	}
}

void ccKneeboardObjectsBuffer::add_zone( avKneeboardZoneObject * zone )
{
	assert(zone);
	if (!zone->get_draw_object(cash))
	{
		Element * el = get_template_by_index(1);
		if (el)
		{
			zone->set_draw_object(el,cash);
		}
	}
	Element * el_zone = zone->get_draw_object(cash);
	if (el_zone) 
	{
		elements.push_back(el_zone);
	}
}

void ccKneeboard::SetCommand( int command , float value )
{
	switch (command)
	{
	case iCommandPlaneShowKneeboard:
		{
			bool use_hud_only    = check_purpose(CURR_PURPOSE_HUD_ONLY_VIEW);
			bool use_screenspace = check_purpose(CURR_PURPOSE_SCREENSPACE_INSIDE_COCKPIT);
			
			bool current_state   = use_screenspace;
			if (globalView->GetCockpitHUDOnly())
				current_state   = use_hud_only;


			bool on = !current_state;
			if (value > 0.1)
				on = true;
			else if (value < -0.1)
				on = false;
			if (on)
			{
				if (!use_screenspace)
					 push_purpose(CURR_PURPOSE_SCREENSPACE_INSIDE_COCKPIT);
				if (!use_hud_only)
					 push_purpose(CURR_PURPOSE_HUD_ONLY_VIEW);
			}
			else
			{
				if (use_screenspace)
					remove_purpose(CURR_PURPOSE_SCREENSPACE_INSIDE_COCKPIT);
				if (use_hud_only)
					remove_purpose(CURR_PURPOSE_HUD_ONLY_VIEW);
			}
		}
		break;
	}
}

bool ccKneeboard::apply_image_as_page(Graphics::Texture * tex)
{
	if (!tex)
		return false;
	const std::string & normalized_name = tex->Name();
	bool       have_material = resourcer->IsResourceExists(rtMaterial,normalized_name);
	Material * mat  = have_material ? (Material *)resourcer->GetResource(rtMaterial,normalized_name):
									  (Material *)resourcer->GetResourceByClass(rtMaterial,"IlsMaterial");
	if (!mat)
		return false;

	CustomMaterial *cmat = mat->QueryCustomMaterial();
	cmat->SetDiffuseColor(Vector3(1,1,1));
	cmat->SetDiffuseMap(0,tex);
	cmat->SetOpacity(1);
	cmat->SetLighted(false);
	cmat->SetTwoSided(true);
	if (!have_material)
	{
		mat->SetName(normalized_name);
		mat->Register();
		ccDrawable::materials[normalized_name] = mat;
	}
	static const std::string head =  "x = 1\
									 y  =  GetAspect()\
									 SetScale(1)\
									 picture = CreateElement 'ceTexPoly'\
									 picture.name = 'picture'\
									 picture.vertices   	 = {{-x,y},{x,y},{ x,-y},{-x,-y}}\
									 picture.indices	  	 = {0,1,2;0,2,3}\
									 picture.tex_coords   = {{0,0},{1,0},{1,1},{0,1}}\
									 picture.h_clip_relation = 1\
									 picture.level			 = 4\
									 picture.material = '";
	static const std::string tail = "'\
									Add(picture)";
	ccIndicatorPage *set = new ccIndicatorPage();
	if (!set)
		return false;

	set->setSize(HalfWidth,HalfHeight);
	set->setViewDistance(1.0);
	set->setParent(this);
	set->initialize(head + normalized_name + tail,true);

	ElementSets[ElementSets.size() + 10000] = set;

	ccLuaElementSet & page = PageMap[PageMap.size() + 1];
	page.push_back(ElementSets[1]);
	page.push_back(set);
	page.push_back(ElementSets[5]);
	return true;
}


bool ccKneeboard::check_chunk(const std::string & chunk_name)
{
	static const std::string prefix = "KNEEBOARD/IMAGES/";
	if (chunk_name.find(prefix) != 0)
		return false;

	std::string texture_name = globalDispatcher->getMission()->getChunkFile(chunk_name);
	Graphics::Texture * tex = 0;

	if (resourcer->IsResourceExists(rtTexture,chunk_name))
		tex = (Graphics::Texture*)resourcer->GetResource     (rtTexture,chunk_name);
	else
	{
		tex = (Graphics::Texture*)resourcer->GetResource     (rtTexture,texture_name);
		tex->LoadToStep(1);
		tex->SetName(chunk_name);
		tex->Register();	
	}
	return apply_image_as_page(tex);
}
#include "stdafx.h"
#include "WorldManager.h"
#include "Avionics\avKneeboard.h"
#include "MapCoords.h"
#include "wAirdrome.h"
#include "Utilities\ccSimpleOctTree.h"
#include "IDispatcher.h"
#include "wsFlightPlan.h"
#include "wConst.h"
#include "wAirdrome.h"
#include "IUnitProperty.h"
#include "Avionics\avMovingMapPoint.h"
#include "Avionics/avKneeboardObjects.h"
#include "Utilities\avDate.h"
#include "AI\Controller\Common\Tasks.h"

using namespace cockpit;

REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::avKneeboard>, avKneeboard);


static const Box element_box(Vector3(0,0,0),1e-4f);

avKneeboard::avKneeboard():
storage(NULL),
zones_storage(NULL),
pages_count(0),
change_counter_(INT_MAX),
arcade_mode_(false)
{
	use_charts_		= true;
	lat_lon_aspect_ = 1.0;
	map_page_id_	= 0;
}

avKneeboard::~avKneeboard()
{
	if (storage)
	{
		storage->DeleteAll();
		delete storage;
		storage = 0;
	}
	if (zones_storage)
	{
		zones_storage->DeleteAll();
		delete zones_storage;
		zones_storage = 0;
	}
}

void avKneeboard::next_page()
{
	if (++map_page_id_ >= pages_count)
		  map_page_id_  = 0;
	update_change_counter();
}

void avKneeboard::prev_page()
{
	if (map_page_id_)
		--map_page_id_;
	else if (pages_count)
		  map_page_id_  = pages_count - 1;
	update_change_counter();
}

void avKneeboard::update()
{
	if (0)
	{
		dVector pnt = get_context()->human_ptr->GetPoint();
		double lat;
		double lon;

		MapCoordsSpace::MapCoords::LatLon(pnt.x,pnt.z,lat,lon);
#if 0
		static wModelTime time = -10;
		if (time < wTime::GetModelTime())
		{
			next_map_page();
			time = wTime::GetModelTime() + 10.0;
		}
#endif
		map_pages_[0].latitude  = lat;
		map_pages_[0].longitude = lon;
		map_pages_[0].rotation  = -get_context()->human_ptr->Course();
		update_change_counter();
	}

//	mark_self();
}


void avKneeboard::initialize(unsigned char ID, const std::string& Name, const std::string& script_name)
{
	avDevice::initialize(ID,Name,script_name);
 	make_default_activity(10.0);

	Lua::Config config(ensured_Lua());
	lat_lon_aspect_		   = config.get("lat_lon_aspect",1.0);

	dVector pnt = get_context()->human_ptr->GetPoint();
	double lat;
	double lon;
	MapCoordsSpace::MapCoords::LatLon(pnt.x,pnt.z,lat,lon);
	double rotation = -get_context()->human_ptr->Course();

	init_waypoints();

	config.call("generate_maps");

	if (config.open("map_pages"))
	{
		int i = 0;
		while(config.open(++i))
		{
			kneeboard_map_page pg;
			config.get("scale"    ,&pg.volume_size,1.0);
			config.get<unsigned>("chart"    ,&pg.chart_id,7);
			config.get("latitude" ,&pg.latitude ,lat);
			config.get("longitude",&pg.longitude,lon);
			config.get("rotation" ,&pg.rotation ,0.0);
			map_pages_.push_back(pg);
			config.pop();
		}
		config.pop();
	}

	if (map_pages_.empty())
	{
		map_pages_.resize(1);
		map_pages_[0].volume_size = config.get("default_scale",0.5);
		map_pages_[0].chart_id	  = config.get("default_chart",7);
		map_pages_[0].rotation    = -get_context()->human_ptr->Course();
		map_pages_[0].latitude    = lat;
		map_pages_[0].longitude   = lon;
	}

	pages_count = map_pages_.size();

	int count = config.get("number_of_additional_pages",0);

	if (count > 0)
		pages_count += count;
	init_zones();


	//listen events from task handler
	listen_event(this,"AI::AttackGroup");	
	listen_event(this,"AI::AttackUnit");
	listen_event(this,"AI::Bombing");
	listen_event(this,"AI::AttackMapObject");
	listen_event(this,"AI::BombingRunway");
	listen_event(this,"AI::Orbit");
	listen_event(this,"AI::Land");
	listen_event(this,"AI::Refueling");
	listen_event(this,"AI::Follow");
	listen_event(this,"AI::Escort");
	listen_event(this,"AI::Task");
	listen_event(this,"AI::resetTask");
}

double avKneeboard::get_volume_size() const
{
	return get_map().volume_size;
}

bool avKneeboard::get_map_rotation( double & h )
{
	h = get_map().rotation;
	return true;
}

Vector3 avKneeboard::get_camera_point() const
{	
	const kneeboard_map_page & map_ = get_map();
	double lat = map_.latitude;
	double lon = map_.longitude;
	if (arcade_mode_)
	{
		dVector pnt = get_context()->human_ptr->GetPoint();
		MapCoordsSpace::MapCoords::LatLon(pnt.x,pnt.z,lat,lon);
	}
	return Vector3(lat / lat_lon_aspect_,
				   use_charts_ ? 0 : 2.0f * map_.volume_size,
				   lon);
}

const kneeboard_map_page & avKneeboard::get_map() const
{
	if (map_page_id_  <  map_pages_.size())
	{
		return map_pages_[map_page_id_];
	}
	else
	{
		return map_pages_[0];
	}
}

void avKneeboard::init_waypoints()
{
	Box bounds;
	bounds.min.x  = -180.0; 
	bounds.max.x  =  180.0; 
	bounds.min.z  = -180.0;
	bounds.max.z  =  180.0;
	bounds.min.y  = -300.0f;	 
	bounds.max.y  =  30000.0f;


	storage = SimpleOctTree::create_quad_tree(bounds,1.0);

	Lua::Config config(globalDispatcher->mission()); 
	if (config.open("db.human.group.route.points"))
	{

		Lua::Config device_config(ensured_Lua());

		unsigned i = 0;
		while(config.open(++i))
		{
			Waypoint wpt;
			read_waypoint(wpt,config);
			if (wpt.Fly_over == PRT_TAKEOFF_FROMRUNWAY  ||
				wpt.Fly_over == PRT_TAKEOFF_FROMPARKING ||
				wpt.Fly_over == PRT_LAND)
			{
				ObjectID id		  = read_airdrome_id(config);
				woPointer adr_ptr = globalWorldManager->Get_woPointer(id);
				if (adr_ptr.Obj())
				{
					cPosition pos	  = adr_ptr->Position();
					bool isMillitary  = true;
					bool isAerodrome  = adr_ptr->Class()		 == woClass_Airdrome;
					bool isAirCarrier = adr_ptr->Class()		 == woClass_Ship &&
										adr_ptr->Type().Level3() == wsType_AirCarrier;
					bool isHeliPad    = adr_ptr->Class() == woClass_Heliport;
					if(isAerodrome)
					{
						wAirdrome * adr  = static_cast<wAirdrome*>(adr_ptr.Obj());
						isMillitary		 = adr->isMilitary();
						pos				 = adr->GetRunwayPosition();
						wpt.name		 = adr->getName();
					}
					else if (isHeliPad)
					{
						wpt.name		 = globalDispatcher->get_unit_property(adr_ptr->ID(),UNIT_NAME);
					}
					else
					{
						wpt.name		 = globalDispatcher->get_unit_property(adr_ptr->ID(),UNIT_NAME);
					}
					wpt.point[0] = pos.p.x;
					wpt.point[2] = pos.p.z;
				}
			}
			else
			{

			}
			double latitude;
			double longitude;
			MapCoordsSpace::MapCoords::LatLon(wpt.point[0],wpt.point[2],latitude,longitude);
			avMovingMapPoint * new_point = new avMovingMapPoint();	
			new_point ->setType(PTYPE_WAYPOINT);
			if (!wpt.name.empty())
				new_point->setName(wpt.name);
			else
				new_point->setNumber(i);
			new_point->setOrientedBox(element_box);
			set_symbol_soordinates(new_point,latitude,longitude);
			storage->AddObject(new_point);
			route.push_back(new_point);
			//////////////////////////////////////////////////////////////////////////
			device_config.call(0,"on_waypoint_adding",latitude,longitude,0.0);
			//////////////////////////////////////////////////////////////////////////
			config.pop();
		}
		config.pop();
	}
}

void avKneeboard::init_zones()
{
	Box bounds;
	bounds.min.x  = -180.0; 
	bounds.max.x  =  180.0; 
	bounds.min.z  = -180.0;
	bounds.max.z  =  180.0;
	bounds.min.y  = -300.0f;	 
	bounds.max.y  =  30000.0f;

	zones_storage = SimpleOctTree::create_quad_tree(bounds,1.0);

	Lua::Config config(ensured_Lua()); 
	if (config.open("zones"))
	{
		unsigned i = 0;
		while(config.open(++i))
		{
			avKneeboardZoneObject * zone = new avKneeboardZoneObject(config,lat_lon_aspect_);
			zones_storage->AddObject(zone);
			config.pop();
		}
		config.pop();
	}
}

double avKneeboard::get_symbol_latitude(avMovingMapPoint * point) const
{
	assert(point);
	return point->getPoint().x * lat_lon_aspect_;
}
double avKneeboard::get_symbol_longitude(avMovingMapPoint * point) const
{
	assert(point);
	return point->getPoint().z;
}
void avKneeboard::set_symbol_soordinates(avMovingMapPoint * point,double latitude,double longitude)
{
	assert(point);
	point->setPoint(dVector(latitude / lat_lon_aspect_,point->getHeight(), longitude));
}

void avKneeboard::set_symbol_heading    (avMovingMapPoint * point,double heading)
{
	assert(point);
	point->setRotate(heading);
}

//////////////////////////////////////////////////////////////////////////

avKneeboardZoneObject::avKneeboardZoneObject(Lua::Config & config,float lat_lon_aspect)
{
	points.reserve(10);
	int i = 0;
	while (config.open(++i))
	{
		points.push_back(Vector3(config.get(1,0.0f) / lat_lon_aspect, //latitude
								 0,									  //altitude always zero
								 config.get(2,0.0f)));				  //longitude
		config.pop();
	}
	set_points(points);
}

void  avKneeboardZoneObject::set_points(const ed::vector<Vector3> & pnts)
{
	points = pnts;
	Box bound;
	for (int i = 0; i < points.size();++i)
		bound.InsertPoint(points[i],i == 0);
	setBoundingBox(bound);
}

void avKneeboard::SetCommand( int command,float value /*= 0*/ )
{
	switch (command)
	{
	case KNEEBOARD_NEXT_PAGE:
		next_page();
		break;
	case KNEEBOARD_PREV_PAGE:
		prev_page();
		break;
	case KNEEBOARD_MAKE_MARK:
		mark_self();
		break;
	}
}

void avKneeboard::mark_self()
{
	cPosition pos	 = get_context()->human_ptr->Position();
	double    course = get_context()->human_ptr->Course();

	double latitude;
	double longitude;
	
	MapCoordsSpace::MapCoords::LatLon(pos.p.x,pos.p.z,latitude,longitude);

	avMovingMapPoint * new_point = new avMovingMapPoint();	

	double t = fmod(wTime::GetAbsoluteModelTime(),86400);

	int HH = floor(t/3600.0);
	int MI = floor((t - HH * 3600.0)/60.0);

	char buff[256];
	sprintf_s(buff,256,"%02d:%02d",HH,MI);

	new_point->setName(buff);
	new_point->setType(PTYPE_SELF_MARK_POINT);

	new_point->setOrientedBox(element_box);
	set_symbol_soordinates(new_point,latitude,longitude);
	set_symbol_heading(new_point,-course);
	storage->AddObject(new_point->getName(),new_point);
	marks.push_back(new_point);

	update_change_counter();
}

unsigned avKneeboard::form_page_id_for_indicator() const
{
	if (map_page_id_ < map_pages_.size())
		return	0;
	return map_page_id_ - map_pages_.size() + 1;
}

void avKneeboard::OnCockpitEvent( const std::string & event, const Graphics::DParamList & params )
{
	if		(event == "AI::AttackGroup")
	{
		task_data.cleanup();
		AI::AttackGroup * task  = (AI::AttackGroup*)params[0].p;
	}
	else if (event == "AI::AttackUnit")
	{
		task_data.cleanup();
		AI::AttackUnit * task  = (AI::AttackUnit*)params[0].p;
	}
	else if (event == "AI::Bombing")
	{
		task_data.cleanup();
		AI::Bombing * task  = (AI::Bombing*)params[0].p;
	}
	else if (event == "AI::AttackMapObject")
	{
		task_data.cleanup();
		AI::AttackMapObject * task  = (AI::AttackMapObject*)params[0].p;
	}
	else if (event == "AI::BombingRunway")
	{
		task_data.cleanup();
		AI::BombingRunway * task  = (AI::BombingRunway*)params[0].p;
	}
	else if (event == "AI::Orbit")
	{
		task_data.cleanup();
		AI::Orbit * task  = (AI::Orbit*)params[0].p;
	}
	else if (event == "AI::Land")
	{
		task_data.cleanup();
		AI::Land * task  = (AI::Land*)params[0].p;
	}
	else if (event == "AI::Refueling")
	{
		task_data.cleanup();
		AI::Refueling * task  = (AI::Refueling*)params[0].p;
	}
	else if (event == "AI::Follow")
	{
		task_data.cleanup();
		AI::Follow * task  = (AI::Follow*)params[0].p;
	}
	else if (event == "AI::Escort")
	{
		task_data.cleanup();
		AI::Escort * task  = (AI::Escort*)params[0].p;
	}
	else if (event == "AI::Task")
	{
		task_data.cleanup();
		AI::Task * task  = (AI::Task*)params[0].p;
	}
	else if (event == "AI::resetTask")
	{
		task_data.cleanup();
	}
}

void TaskRelatedData::cleanup()
{
	if (task)
		task = nullptr;
	for (int i = 0; i < marks.size(); ++i)
		delete marks[i];
	marks.clear();
}

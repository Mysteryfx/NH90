#include "stdafx.h"
#include "Avionics/Sensors/avSimpleRadar.h"
#include "ccModuleManager.h"
#include "Avionics/avSimpleElectricSystem.h"
#include "iCommand.h"
#include "ClipVolumes\ClipVolumeCamera.h"
#include "viObjectManager.h"
#include "wsType_Level4.h"
#include "wConst.h"
#include "ITerrain.h"
#include "wsInitData.h"
#include "IwInfo.h"
#include "Math\_math.h"
#include "wRocketData.h"
#include "IwoLA.h"
#include "IwHumanPLane.h"
#include "Renderer\DbgGraph.h"
#include "Modeler\ModelParser.h"
#include "IwWorld.h"
#include "Utilities/ccTriggerSystem.h"
#include "woSpot.h"
#include "wDetectable.h"

using namespace cockpit;

REGISTER_DYNAMIC_FACTORY(cockpit::avSimpleRadar,avSimpleRadar);

static const float onSurfaceRadarRangeModifier = 0.6666666667f;//на сколько уменьшается дальность обнаружения радаром цели на фоне земли

void contact_export::update_with_contact(const scan_contact & contact, int index)
{
	if (time_handle.empty())
	{
		std::string name("RADAR_CONTACT_");
		char buffer[8];
		sprintf_s(buffer,8,"%02d_",index);
		name += buffer;

		time_handle			 .init_with((name + "TIME").c_str());
		range_handle		 .init_with((name + "RANGE").c_str());
		azimuth_handle		 .init_with((name + "AZIMUTH").c_str());
		elevation_handle	 .init_with((name + "ELEVATION").c_str());
		friendly_handle		 .init_with((name + "FRIENDLY").c_str());
		rcs_handle			 .init_with((name + "RCS").c_str());
		nctr_handle			 .init_with((name + "NCTR").c_str());
		velocity_x			 .init_with((name + "VX").c_str());
		velocity_y			 .init_with((name + "VY").c_str());
		velocity_z			 .init_with((name + "VZ").c_str());
		service_randomization.init_with((name + "RND").c_str());
		is_it_noise_mark	 .init_with((name + "NOISE").c_str()); 
	}
	time_handle			  = wTime::GetModelTime() - contact.time_stamp;
	range_handle		  = contact.polar_position.distance;
	azimuth_handle		  = contact.polar_position.azimuth;
	elevation_handle	  = contact.polar_position.elevation;
	friendly_handle		  = contact.friendly;
	rcs_handle			  = contact.RCS;
	nctr_handle			  = contact.NCTR_result.c_str();
	velocity_x			  = contact.velocity.x;
	velocity_y			  = contact.velocity.y;
	velocity_z			  = contact.velocity.z;
	is_it_noise_mark      = contact.noise_mark;	
	service_randomization = contact.service_randomization;
}


void contact_export::mark_invalid()
{
	time_handle = -1.0;
}


static const float range_solution(double RCS,
								  double antenna_directivity,
								  double antenna_effective_area,
								  double signal_power_out,
								  double signal_power_in_min)
{
	return pow(signal_power_out * 
			   antenna_directivity * 
			   antenna_effective_area * RCS /(16.0 * M_PI * M_PI *signal_power_in_min ),
			   0.25);
}

scan_pattern_data::scan_pattern_data()
{
	volume_vertical			 = Math::toRad(17.8 * 2.0);
	volume_horizontal		 = Math::toRad(28.0 * 2.0);
	volume_origin_horizontal = 0;
	volume_origin_vertical   = 0;
}


avSimpleRadar::avSimpleRadar():
spot(nullptr)
{
	p_search_timer = new avSimpleRadarTimer(this);

	antenna_roll_compensation  = 0;
	antenna_pitch_compensation = 0;

	ground_clutter.max_distance	  = 0;
	antenna_azimuth			  = 0;
	antenna_elevation		  = 0;
	horizontal_scan_direction = true;
	mode					  = NONE;
	scan_beam				  = Math::toRad(3.0);
	max_available_distance	  = 60000.0;
	dead_zone				  = 300.0;
	contacts.reserve(64);


	TDC_aqusition.volume_vertical	 = Math::toRad(17.8 * 2.0);
	TDC_aqusition.volume_horizontal	 = Math::toRad(5.0);
	TDC_aqusition.volume_origin_horizontal = 0;
	TDC_aqusition.volume_origin_vertical   = 0;

	TDC_range.init_with("RADAR_TDC_RANGE");
	TDC_azimuth.init_with("RADAR_TDC_AZIMUTH");
	TDC_range_carret_size.init_with("RADAR_TDC_RANGE_CARRET_SIZE");

	STT_azimuth.init_with("RADAR_STT_AZIMUTH");
	STT_elevation.init_with("RADAR_STT_ELEVATION");
	STT_range.init_with("RADAR_STT_RANGE");

	RDR_mode_export.init_with("RADAR_MODE");
	IFF_interrogator_is_on.init_with("IFF_INTERROGATOR_STATUS");


	SCAN_volume_origin_azimuth  .init_with("SCAN_ZONE_ORIGIN_AZIMUTH");
	SCAN_volume_origin_elevation.init_with("SCAN_ZONE_ORIGIN_ELEVATION");

	SCAN_volume_azimuth	 .init_with("SCAN_ZONE_VOLUME_AZIMUTH");
	SCAN_volume_elevation.init_with("SCAN_ZONE_VOLUME_ELEVATION");
	PITCH_BANK_stabilization.init_with("RADAR_PITCH_BANK_STABILIZATION");

	ACQUSITION_volume_azimuth			.init_with("ACQUSITION_ZONE_VOLUME_AZIMUTH");

	BIT_in_progress.init_with("RADAR_BIT");

	PITCH_BANK_stabilization = 1.0;
	STT_azimuth			  = 0.0;
	STT_elevation		  = 0.0;
	STT_range			  = 0.0;
	RDR_mode_export		  = 0.0;
	TDC_range			  = 15000.0;
	TDC_azimuth			  = 0.0; 
	TDC_range_carret_size = 6000.0;
	antenna_pos_local.p.x = 6.0f;
	BIT_in_progress		  = 0;

	IFF_interrogator_is_on = 0.0;
	scan_speed			   =  Math::toRad(150.0);
	tracking_azimuth.min   = -Math::toRad(30.0);
	tracking_azimuth.max   =  Math::toRad(30.0);
	tracking_elevation.min = -Math::toRad(30.0);
	tracking_elevation.max =  Math::toRad(30.0);


	render_debug_info	   = false;
}


void avSimpleRadar::initialize( unsigned char ID, const std::string& Name, const std::string& script_name )
{
	avLuaDevice::initialize(ID,Name,script_name);
	get_context()->link_to_target = this;




	roll_compensation_limits.max  =  TO_RAD(70.0);
	roll_compensation_limits.min  = -TO_RAD(70.0);
	pitch_compensation_limits.min = -TO_RAD(8.0);
	pitch_compensation_limits.max =  TO_RAD(25.0);

	if (!script_name.empty())
	{
		Lua::Config config(ensured_Lua("avSimpleRadar"));

		std::string power_bus;

		if (config.get("power_bus_handle",&power_bus))
		{
			param_bus.setNominalVoltage(28);
			param_bus.init_with(power_bus.c_str());
		}

#ifdef _DEBUG
		config.get("render_debug_info",&render_debug_info,true);
#else
		config.get("render_debug_info",&render_debug_info,false);
#endif
		if (config.open("perfomance"))
		{
			if (config.open("tracking_azimuth"))
			{
				tracking_azimuth.min = config.get(1,-Math::toRad(30.0));
				tracking_azimuth.max = config.get(2, Math::toRad(30.0));
				config.pop();
			}
			if (config.open("roll_compensation_limits"))
			{
				roll_compensation_limits.min = config.get(1,-Math::toRad(70.0));
				roll_compensation_limits.max = config.get(2, Math::toRad(70.0));
				config.pop();
			}
			if (config.open("pitch_compensation_limits"))
			{
				pitch_compensation_limits.min = config.get(1,-Math::toRad(8.0));
				pitch_compensation_limits.max = config.get(2, Math::toRad(25.0));
				config.pop();
			}
			scan_pattern.volume_horizontal = config.get("scan_volume_azimuth"   ,Math::toRad(30.0));
			scan_pattern.volume_vertical   = config.get("scan_volume_elevation" ,Math::toRad(20.0));
			scan_beam					   = config.get("scan_beam"				,Math::toRad(3.0));
			max_available_distance		   = config.get("max_available_distance",60000.0);


			if (config.open("ground_clutter"))
			{
				if (config.open("sea"))
				{
					config.get(1,&ground_clutter.sea.x,0.0f);
					config.get(2,&ground_clutter.sea.y,0.0f);
					config.get(3,&ground_clutter.sea.z,0.0f);
					config.pop();
				}
				if (config.open("land"))
				{
					config.get(1,&ground_clutter.land.x,0.0f);
					config.get(2,&ground_clutter.land.y,0.0f);
					config.get(3,&ground_clutter.land.z,0.0f);
					config.pop();
				}
				if (config.open("artificial"))
				{
					config.get(1,&ground_clutter.artificial.x,0.0f);
					config.get(2,&ground_clutter.artificial.y,0.0f);
					config.get(3,&ground_clutter.artificial.z,0.0f);
					config.pop();
				}
				config.get("max_distance",&ground_clutter.max_distance,0.0f);

				if (ground_clutter.sea.lengthSq()        < 1e-8 && 
					ground_clutter.land.lengthSq()	     < 1e-8 &&
					ground_clutter.artificial.lengthSq() < 1e-8)
				{
					ground_clutter.max_distance = 0;
				}
				config.pop();
			}
			config.pop();
		}
	}
	SCAN_volume_azimuth			= scan_pattern.volume_horizontal;
	SCAN_volume_elevation		= scan_pattern.volume_vertical;

	ACQUSITION_volume_azimuth	= TDC_aqusition.volume_horizontal;
}

void avSimpleRadar::post_initialize()
{
	avLuaDevice::post_initialize();

	if (!param_bus.empty())
		 param_bus >> power_switch;
	else if (avSimpleElectricSystem::get())
		avSimpleElectricSystem::get()->getACbus(1) >> power_switch;
	
	listen_command(this,iCommandPlane_LockOn_start);
	listen_command(this,iCommandPlane_LockOn_finish);

	listen_command(this,iCommandPlaneRadarUp);
	listen_command(this,iCommandPlaneRadarDown);

	listen_command(this,iCommandPlaneRadarLeft);
	listen_command(this,iCommandPlaneRadarRight);


	power_line.m_signalPowerOn  +=	lwl::delegate(this, &avSimpleRadar::on_power_on);
	power_line.m_signalPowerOff +=	lwl::delegate(this, &avSimpleRadar::on_power_off);

	power_switch >> power_line;
	power_switch.setOnOff(true);

	if (p_search_timer)
		p_search_timer->Start(0);
}

void avSimpleRadar::update_spot()
{
	if (!getElecPower() || mode ==  NONE)
		spot_cache.clear();
	if (spot_cache.empty())
	{
		kill_spot();
		return;
	}
	//spot = woSpot::create_radar_spot(get_context()->human_ptr->ID(),)
}

void avSimpleRadar::kill_spot()
{
	if (spot)
	{
		wTime::KillMe(spot);
		spot = nullptr;
	}
}

void avSimpleRadar::release()
{
	kill_spot();
	avLuaDevice::release();
	if (p_search_timer)
	{
		p_search_timer->Kill();
		p_search_timer = nullptr;
	}
}

void avSimpleRadar::update_radar()
{
	if (!param_bus.empty())
		 param_bus.update_connection();

	if (!getElecPower() )
	{
		mode = NONE;
	}

	//i/o from user
	scan_pattern.volume_origin_horizontal  = SCAN_volume_origin_azimuth;
	scan_pattern.volume_origin_vertical	   = SCAN_volume_origin_elevation;

	scan_pattern.volume_horizontal		   = SCAN_volume_azimuth;
	scan_pattern.volume_vertical		   = SCAN_volume_elevation;

	TDC_aqusition.volume_horizontal		   =  ACQUSITION_volume_azimuth;
	TDC_aqusition.volume_origin_horizontal = -TDC_azimuth;
	TDC_aqusition.volume_vertical		   =  scan_pattern.volume_vertical;
	TDC_aqusition.volume_origin_vertical   =  scan_pattern.volume_origin_vertical;



	auto move_variable = [this](double & variable,double target)
	{
		static const double speed = TO_RAD(120.0);
		if (variable <  target)
		{
			variable += speed * this->p_search_timer->GetDeltaTime();
			if (variable > target)
				variable = target;
		}
		else if (variable >  target)
		{
			variable -= speed * this->p_search_timer->GetDeltaTime();
			if (variable < target)
				variable = target;
		}
	};

	if (PITCH_BANK_stabilization > 0)
	{
		move_variable(antenna_roll_compensation , roll_compensation_limits.to_limit(get_context()->human_ptr->Roll()));
		move_variable(antenna_pitch_compensation,pitch_compensation_limits.to_limit(get_context()->human_ptr->Tang()));
	}
	else
	{
		move_variable(antenna_roll_compensation ,0);
		move_variable(antenna_pitch_compensation,0);
	}

	radar_work_mode m = mode;
	if (BIT_in_progress > 0.0 && m != NONE)
	{
		m = BUILT_IN_TEST;
	}
	switch (m)
	{
	case NONE:
		break;
	case BUILT_IN_TEST:
		break;
	case SCAN:
		scan_for_targets();
		break;
	case ACQUISITION:
		update_acquisition();
		break;
	case TRACKING:
		update_STT();
		break;
	}
	update_export();
}

void avSimpleRadar::SetCommand( int command, float value)
{
	switch (command)
	{
	case iCommandPlaneRadarOnOff:
		power_switch.setOnOff(!power_switch.getIsOn());
		break;
	case iCommandPlane_LockOn_start:
		if (mode == SCAN)
			mode = ACQUISITION;
		else if (mode == TRACKING)
			mode = SCAN;
		break;
	case iCommandPlane_LockOn_finish:
		if (mode == ACQUISITION)
			mode = SCAN;
		break;
	case iCommandPlaneRadarUp:
		{
			TDC_range = TDC_range + 100.0;
		}
		break;
	case iCommandPlaneRadarDown:
		{
			TDC_range = TDC_range - 100.0;
		}
		break;
	case iCommandPlaneRadarLeft:
		{
			TDC_azimuth = TDC_azimuth - Math::toRad(0.1);
		}
		break;
	case iCommandPlaneRadarRight:
		{
			TDC_azimuth = TDC_azimuth + Math::toRad(0.1);
		}
		break;
	}
	avLuaDevice::SetCommand(command,value);
}

bool avSimpleRadar::is_tracking( ObjectID ID, unsigned weapon_type ) const
{
	if (mode == TRACKING)
		return stt.id == ID;
	else if (spot && spot->ID() == ID)
		return true;
	return false;
}


void avSimpleRadar::update_STT()
{
	auto update_stt = [& , this] () -> bool
	{
		woPointer ptr = globalWorld->Get_woPointer(stt.id);
		if (!ptr)
			return false;
		detect_status val = get_detection_status(ptr);
		if (val.can_feel_radiation)
		{
			IwoLA* theHuman = dynamic_cast<IwoLA*>(ptr.Obj());
			if (theHuman)
				theHuman->RadarModeMessage(get_context()->human_ptr->ID(), wsMailRadarModeLock, wsRadarAir);
		}
		if (!val.can_be_locked)
			return false;
		Vector3 new_velocity =  ptr->VectorVelocity();	
		stt.pos				 =  ptr->GetPosition();
		stt.acceleration     = (new_velocity - stt.velocity)/(wTime::GetModelTime() - stt.time_stamp);
		stt.velocity		 =  new_velocity;
		stt.time_stamp		 =  wTime::GetModelTime();
		stt.polar			 =  PolarPosition(get_context()->human_ptr->Position(wTime::GetModelTime()),stt.pos.p); 
		//export
		STT_azimuth			 = stt.polar.azimuth;
		STT_elevation		 = stt.polar.elevation;
		STT_range			 = stt.polar.distance;
		if (!tracking_azimuth  .in_limit(stt.polar.azimuth) ||
			!tracking_elevation.in_limit(stt.polar.elevation))
		{
			return false;
		}
		return true;
	};

	if (stt.id && !update_stt())
	{
		mode = SCAN;
		stt.clear();
	}
}

void avSimpleRadar::update_acquisition()//завязка траектории на цель
{
	scan_for_targets();

	auto it		 = acquistion_data.begin();
	auto nearest = acquistion_data.end();

	while (it != acquistion_data.end())
	{
		if (nearest == acquistion_data.end())
			nearest = it;
		else if (it->second.distance_from_center < nearest->second.distance_from_center)
			nearest = it;
		++it;
	}

	if (nearest != acquistion_data.end())
	{
		if (nearest->second.spotted_count > 4)
		{
			stt.id = nearest->first;
			mode   = TRACKING;
			contacts.clear();
			acquistion_data.clear();
			return;
		}
	}
}


void avSimpleRadar::scan_for_targets()
{
	auto contact = contacts.begin();

	double t = wTime::GetModelTime();
	while (contact != contacts.end())
	{
		scan_contact & c = *contact;
		if (t - c.time_stamp > 10.0)
		{
			contact = contacts.erase(contact);
			continue;
		}
		++contact;
	}

	next_scan_step();

	spot_cache.clear();
	jamm_sources.clear();
			
	antenna_pos_scan_origin	= get_context()->human_ptr->Position(wTime::GetModelTime());
	antenna_pos				= antenna_pos_scan_origin;

	antenna_pos_local.ONEBASIS();
	antenna_pos_local.rotZ(-antenna_pitch_compensation);
	antenna_pos_local.rotX(-antenna_roll_compensation);
	
	cPosition antenna_local_scan_origin = antenna_pos_local;

	antenna_local_scan_origin.rotZ(get_current_pattern().volume_origin_vertical);

	antenna_pos_scan_origin.mult(antenna_local_scan_origin,antenna_pos_scan_origin);
	antenna_pos_local.rotY(antenna_azimuth);
	antenna_pos_local.rotZ(antenna_elevation);
	antenna_pos.mult(antenna_pos_local,antenna_pos);
	
	double    h_size    = scan_speed * p_search_timer->GetDeltaTime();

	ClipVolumeCamera beam_volume(antenna_pos, 
								 get_max_available_distance(),
								 0.5 * h_size,
								 h_size/scan_beam);

	int number = viObjectManager::instance->GetObjects (&beam_volume, 
														&handler_, 
														this,
														viWorldHeavyObject);

	make_ground_spots();
	make_jammer_spots();
}

void avSimpleRadar::make_jammer_spots()
{
	static const int marks_per_contact = 24;
	for (auto it  = jamm_sources.begin(); 
			  it != jamm_sources.end();
			++it)
	{
		woPointer & jamm = *it;
		dVector pnt = jamm->GetPoint();
		for (int i = 0; i < marks_per_contact; ++i)
		{
			contacts.resize(contacts.size() + 1);
			scan_contact & new_contact			 = contacts.back();
			new_contact.RCS						 = 25.0 * device_random_evenly.get();
			new_contact.point					 = pnt;
			new_contact.polar_position			 = PolarPosition(antenna_pos_scan_origin,new_contact.point);
			new_contact.polar_position.distance  = get_max_available_distance() * device_random_evenly.get();
			new_contact.noise_mark		       = true;
			new_contact.friendly		       = -1;//UNKNOWN
			new_contact.time_stamp			   = wTime::GetModelTime();
			new_contact.service_randomization  = device_random_evenly.get();
			spot_cache.push_back(new_contact.point);
		}
	}
}


void avSimpleRadar::make_ground_spots()
{
	if (ground_clutter.max_distance < 300.0)
		return;

	surf_info.set(antenna_pos.p.x,antenna_pos.p.z);


	dVector start_ = antenna_pos.p;
	dVector res_;
	float rnd_y	   = 2.0 * (device_random_evenly.get() - 0.5);
	float rnd_z	   = 2.0 * (device_random_evenly.get() - 0.5);

	dVector end_   = antenna_pos.p + antenna_pos.x * ground_clutter.max_distance
								   + antenna_pos.y * 0.5 * scan_beam * ground_clutter.max_distance *rnd_y
								   + antenna_pos.z * 0.5 * scan_beam * ground_clutter.max_distance *rnd_z;
	if (surf_info.intersectSurface(start_,end_,&res_))
		make_ground_spot(res_);
}

double avSimpleRadar::synthetic_RCS_for_land(const dVector & pnt,int surfType) const
{
	float rnd_1		 = device_random_evenly.get((unsigned)fabs(pnt.x));
	float rnd_2		 = device_random_evenly.get((unsigned)fabs(pnt.z));
	switch (surfType)
	{
	case typeMeshSea:    // Вода
	case typeMeshShelf:  // Шельф (поверхность дна)
	case typeMeshChannel:// Каналы
	case typeMeshRiver:  // Реки
	case typeMeshLake:   // Озера
	case typeMeshBeach:  // Береговые линии
		return ground_clutter.sea.x + ground_clutter.sea.y * rnd_1 + ground_clutter.sea.z * rnd_2 ;
	case typeMeshIsoline: // Линии высот для карты
	case typeMeshLine: // Географическая сетка на карте
	case typeMeshMGRS: // MGRS сетка на карте
	case typeMeshLamp: // Лампочки	
		return 0;
	case typeMeshRoad:  // Автодороги
	case typeMeshRailway: // Ж.дороги
	case typeMeshELT: // ЛЭП (для карты)
	case typeMeshRunway: // аэродром (впп)
	case typeMeshTown:   // Города
	case typeMeshBuilding: // Строения
		return ground_clutter.artificial.x + ground_clutter.artificial.y * rnd_1 + ground_clutter.artificial.z * rnd_2 ;
	case typeMeshStone:  // Stone
	case typeMeshLand:   // Земля
	case typeMeshIsland: // Острова
	case typeMeshForest: // Forest
	case typeMeshTrees:  // Деревья на кварталах	
	case typeMeshGrass:  // Grass
	case typeMeshField:  // поля
	case typeMeshPlant:  // растения
	case typeMeshSmallShit: // 
	default:
		return ground_clutter.land.x + ground_clutter.land.y * rnd_1 + ground_clutter.land.z * rnd_2 ;
	}

};
void avSimpleRadar::make_ground_spot(const dVector & pnt)
{
	surf_info_2.set(pnt.x,pnt.z);
	float RCS_rnd = synthetic_RCS_for_land(pnt,surf_info_2.getTypeSurface());
	if (RCS_rnd < 1e-5)
		return;

	range_result range_res = get_range_for_target(RCS_rnd,pnt,cVector(0,1,0),cVector(0,0,0));

	double D = (pnt - antenna_pos.p).length2();

	if (D < 300.0 * 300.0)
		return;
	if (range_res.range * range_res.range < D)
		return;

	contacts.resize(contacts.size() + 1);
	scan_contact & new_contact = contacts.back();
	new_contact.RCS				   = RCS_rnd;
	new_contact.point			   = pnt;
	new_contact.polar_position     = PolarPosition(antenna_pos_scan_origin,new_contact.point);
	new_contact.noise_mark		   = true;
	if (IFF_interrogator_is_on > 0.0)
		new_contact.friendly = 0;
	else
		new_contact.friendly = -1;//UNKNOWN
	new_contact.time_stamp = wTime::GetModelTime();
	new_contact.service_randomization  = avDevice::device_random_evenly.get();



	spot_cache.push_back(new_contact.point);
}


bool TypeAvailable_Radar(const wsType &type)
{
	bool wing_part    = type.Level1() == wsType_Air &&
						type.Level2() == wsType_WingPart;

	bool air_vehicle =  type.Level1() == wsType_Air &&
					   (type.Level2() == wsType_Airplane ||
				        type.Level2() == wsType_Helicopter);

	bool weapon      =  type.Level1() == wsType_Weapon &&
						type.Level2() == wsType_Missile &&
					   (type.Level3() == wsType_AS_Missile  || // Ракетa воздух-земля
						type.Level3() == wsType_SS_Missile); // Ракетa земля-земля

	bool free_fall   =  type.Level1() == wsType_Air &&
						type.Level2() == wsType_Free_Fall &&
						type.Level3() == wsType_Snars && 
						type.Level4() == wsType_Chaff;

	return  wing_part   ||
			air_vehicle ||
			weapon      ||
			free_fall ;
}


detect_status avSimpleRadar::get_detection_status(const woPointer  & theTargetPointer) const//видит ли Human цель радаром?
{
	detect_status ret_val;
	ret_val.can_be_locked      = false;
	ret_val.can_be_detected    = false;
	ret_val.can_feel_radiation = false;
	ret_val.RCS				   = 0;

	const wsType & theTargetType = theTargetPointer->Type();//тип цели

	if (theTargetType.Level1() == wsType_Air &&
	   (theTargetType.Level2() == wsType_Airplane ||
		theTargetType.Level2() == wsType_Helicopter))
	{
		ret_val.can_feel_radiation = true;
	}

	ret_val.can_be_detected    = true;
	//если цель не в воздухе - радар ее не видит
	if (!theTargetPointer->In_Air_())
	{
		ret_val.can_be_detected = false;
	}
	cPosition theTargetPosition = theTargetPointer->Position();//позиция цели
	cPosition theHumanPosition  = get_context()->human_ptr->Position(); //позиция human-а

	
	float theHumanMaxViewDistance  = 1.f + 3570.f*sqrt(theHumanPosition.p.y);							//расстояние от human-а до горизонта
	float theTargetMaxViewDistance = theHumanMaxViewDistance + 3570.f * sqrt(theTargetPosition.p.y);	//дистанция с которой human может увидеть цель

	//определяем расстояние до цели
	cVector theVectorToTarget   = theTargetPosition.p - theHumanPosition.p;
	float	theDistanceToTarget = theVectorToTarget.length();

	if (theDistanceToTarget < dead_zone)
	{
		return ret_val;
	}
	//Если цель за горизонотом - она не видна
	if (theDistanceToTarget > theTargetMaxViewDistance)
	{
		ret_val.can_be_detected    = false;
		ret_val.can_feel_radiation = false;
		return ret_val;
	}

	//если скорость цели относительно Human-а меньше минимальной, цель не видна
	//считаем эту самую скорость отностельно Human-а
	float theRelativeTargetVelocity = (get_context()->human_ptr->VectorVelocity() - 
									   theTargetPointer->VectorVelocity()).length(); 
	//определяем минимальную скорость
	float theMinimalRelativeVelocity = RelativVelocityMin;
	if (theRelativeTargetVelocity < theMinimalRelativeVelocity &&
		theDistanceToTarget > 5000.0f)
	{
		ret_val.can_be_detected = false;
	}
	//Проверка  - цель на фоне земли или нет?
	bool isTheTargetOnTerrainsBackground = false;

	if (theDistanceToTarget < theHumanMaxViewDistance)
	{
		if (theTargetPosition.p.y < 
			(theHumanPosition.p.y *	(1.f - (theDistanceToTarget/theHumanMaxViewDistance))))
		{
			isTheTargetOnTerrainsBackground = true;
		}
		else
		{
			isTheTargetOnTerrainsBackground = false;
		}
	}

	bool flagNotch = false;

	if (isTheTargetOnTerrainsBackground)
	{
		//проверим, не меньше ли радиальная скорость самолета минимальной
		//определим минимальную радиальную скорость
		float theMinimalRadialVelocity = RadialVelocityMin;

		if (theDistanceToTarget < 3000.f)
		{
			theMinimalRadialVelocity = 0.f;
		}
		else if (theDistanceToTarget < 10000.f)
		{
			theMinimalRadialVelocity *= 0.5f;
		}

		//на фоне земли или гарантированно пропадают цели с малой радиальной скоростью
		//относительно самолета - эффект notch
		if(fabs(((theTargetPointer->VectorVelocity())*theVectorToTarget)/
				(theVectorToTarget.length())) < theMinimalRadialVelocity)
		{
			flagNotch = true;
		}
	}
	if (flagNotch)
	{
		ret_val.can_be_detected = false;
	}

	//дистанция, с которой радар может видеть цель
	range_result dist_RCS = get_range_for_target(theTargetPointer);

	float theTargetMaxDetectDistance = dist_RCS.range;


	float      delta_psi = theTargetPointer->Course() - get_context()->human_ptr->Course();
//	whTargetData data;
//	data.position = theTargetPosition;
//	UpdateGroundTargetsProperties(data,-1);
	
	//float delta = aspect_relative_sensor(theTargetPointer->VectorVelocity()/* - get_context()->human_ptr->VectorVelocity()*/,data.fim_hor,data.fin_hor);
	switch (get_pulse_repeat_frequency())
	{
	case MEDIUM_PRF://		theTargetMaxDetectDistance *= 0.5f + 0.25f*(1.0f + cos(delta));				break;
	case HIGH_PRF:	//	    theTargetMaxDetectDistance *= 0.5f + 0.25f*(1.0f - cos(delta));				break;
		break;
	}
	

	if (isTheTargetOnTerrainsBackground)//если цель на фоне земли -  эта дистанция сокращается 
	{
		theTargetMaxDetectDistance *= onSurfaceRadarRangeModifier;
	}
	//////////////////////////////////////////////////////////////////////////
	float max_lock_distance = 0.85f * theTargetMaxDetectDistance;
	//////////////////////////////////////////////////////////////////////////
	if(theTargetMaxDetectDistance < FiveNM&&
		theTargetMaxDetectDistance > 0.0f)
	{
		theTargetMaxDetectDistance = FiveNM;
		max_lock_distance          = FiveNM;
	}
	if (theDistanceToTarget > theTargetMaxDetectDistance)//Если дистанция до цели больше, нежели максимальная, радаром цель не видна
	{
		ret_val.can_be_detected = false;
	}
	if (0)
	{
		if (fabs(get_context()->human_ptr->Roll()) > Math::toRad(120.0f) && 
			get_context()->human_ptr->Hrad() < 1500.0f)
		{
			if (theDistanceToTarget > 5000.0f)
			{
				ret_val.can_be_detected = false;
			}
		}
	}
	if (theTargetPointer->IsFlag(wsFlag_Disturbance))
	{
		ret_val.can_be_detected = theDistanceToTarget < 200e3f; 
	}
	//Не прячется ли цель в складках местности? Если нет - ОНА ВИДНА!!!
	if (!globalLand->isVisible(theHumanPosition.p, theTargetPosition.p))
	{
		ret_val.can_be_detected    = false;
		ret_val.can_feel_radiation = false;
	}
	if (ret_val.can_be_detected)
		ret_val.can_be_locked = theDistanceToTarget < max_lock_distance;

	ret_val.RCS = dist_RCS.RCS;
	return ret_val;
}

double avSimpleRadar::get_RCS_for_target(const woPointer  & Enemy_ptr) const
{
	// Вычисление площади отражающей поверхности с учетом 
	// радиоэлектронного противодействия.
	float Reflection   = 100.0f;
	wDetectable * detect = Enemy_ptr->getDetectable();
	if (detect)
		Reflection = detect->getRCS(0,cVector(1.0f, 0.0f, 0.0f));
	else
		Reflection = 0;
	if(Enemy_ptr->IsFlag(wsFlag_Disturbance)) 
		Reflection *= 0.1785f;	
	return Reflection;
}

range_result avSimpleRadar::get_range_for_target(double RCS,
												 const cVector & point,
												 const cVector & target_nose_dir,
												 const cVector & target_velocity) const
{
	float D			 = get_max_available_distance() * sqrt(sqrt(0.01f * RCS));
	float H_koeff	 = 1.0f; // высотный коэффициент
	float h_ac		 = get_context()->human_ptr->Hrad();
	if (h_ac < 800.0f)
		H_koeff = 0.6f;
	else if (h_ac < 1400.0f)
		H_koeff = 0.00066*h_ac + 0.07f; 

	float cos_Rak = -1.0f; // косинус ракурса цели 
	cVector nn = point - get_context()->human_ptr->Position().p;
	float nn_d = nn.length();

	if (nn_d > 1.0f)
		nn = nn/nn_d;
	cos_Rak = nn * target_nose_dir;  
	if(target_velocity.length2() < 30.0 * 30.0)
		cos_Rak = 0.0;
	D *= H_koeff*(2.0f - cos_Rak)/3.0f;

	if (D < FiveNM)
		D = FiveNM;
	range_result res;

	res.range = D;
	res.RCS   = RCS;

	return res;
}

range_result avSimpleRadar::get_range_for_target(const woPointer  & theTargetPointer) const
{
	return get_range_for_target(get_RCS_for_target(theTargetPointer),
								theTargetPointer->Position().p,
								theTargetPointer->Position().x,
								theTargetPointer->VectorVelocity());
}




bool avSimpleRadar::handler_impl_( viObject *item, viSearch *search )
{
	if (item->ID() == get_context()->human_ptr->ID())
		return false;
	MovingObject * objPointer	  = static_cast<MovingObject*>(item);
	if (!TypeAvailable_Radar(objPointer->Type()))
		return false;
	detect_status val = get_detection_status(objPointer);
	if (val.can_feel_radiation)
	{
		IwoLA* theHuman = dynamic_cast<IwoLA*>(objPointer);
		if (theHuman)
		{
			wsMailType message = mode == SCAN ? wsMailRadarModeScan : wsMailRadarModeLock;
			theHuman->RadarModeMessage(get_context()->human_ptr->ID(), message, wsRadarAir);
		}
	}
	if (val.can_be_detected)
	{
		contacts.resize(contacts.size() + 1);

		scan_contact & new_contact = contacts.back();

		new_contact.RCS				   = val.RCS;
		new_contact.point			   = objPointer->GetPoint();
		new_contact.polar_position     = PolarPosition(antenna_pos_scan_origin,new_contact.point);
		new_contact.velocity		   = objPointer->VectorVelocity();
		new_contact.noise_mark		   = false;
		if (IFF_interrogator_is_on > 0.0)
		{
			if (get_context()->human_ptr->Coalition() == objPointer->Coalition())
			{
				new_contact.friendly = 1;
			}
			else if (objPointer->Coalition() == COALITION_NEUTRAL)
			{
				new_contact.friendly = 2;//NEUTRAL
			}
			else
			{
				new_contact.friendly = 0;
			}
		}
		else
		{
			new_contact.friendly = -1;//UNKNOWN
		}
		new_contact.NCTR_result		   = objPointer->unit_type;
		new_contact.time_stamp = wTime::GetModelTime();
		new_contact.service_randomization  = avDevice::device_random_evenly.get();

		if (mode == ACQUISITION)
		{

			if (val.can_be_locked)

			{
				double TDC = TDC_range;
				if (fabs(new_contact.polar_position.distance - TDC) < TDC_range_carret_size)
				{	
					acqusition_cache & new_data = acquistion_data[objPointer->ID()];
					new_data.spotted_count++;

					float dh = TDC_aqusition.volume_origin_horizontal - new_contact.polar_position.azimuth;
					float dv = TDC_aqusition.volume_origin_vertical   - new_contact.polar_position.elevation;
					new_data.distance_from_center = dh*dh + dv*dv;
				}
			}
		}

		spot_cache.push_back(new_contact.point);
	}
	if (objPointer->IsFlag(wsFlag_Disturbance))
	{
		jamm_sources.push_back(objPointer);
	}
	return val.can_be_detected;
}

double avSimpleRadar::get_max_available_distance() const
{
	return max_available_distance;
}

unsigned cockpit::avSimpleRadar::get_pulse_repeat_frequency() const
{
	return 0;
}

void cockpit::avSimpleRadar::next_scan_step()
{
	const scan_pattern_data & pattern = get_current_pattern();
	double width_volume  = scan_speed * p_search_timer->GetDeltaTime();
	double h_limit	     = 0.5 * pattern.volume_horizontal - 0.5 * width_volume;
	if (horizontal_scan_direction)
	{
		antenna_azimuth += scan_speed * p_search_timer->GetDeltaTime();
		if (antenna_azimuth >  pattern.volume_origin_horizontal + h_limit + 1e-4)
		{
			antenna_azimuth  = pattern.volume_origin_horizontal + h_limit;
			next_scan_line();
		}
	}
	else
	{
		antenna_azimuth -= scan_speed * p_search_timer->GetDeltaTime();
		if (antenna_azimuth <  pattern.volume_origin_horizontal - h_limit - 1e-4)
		{
			antenna_azimuth  = pattern.volume_origin_horizontal - h_limit;
			next_scan_line();
		}
	}
}

void avSimpleRadar::next_scan_line()
{
	const scan_pattern_data & pattern = get_current_pattern();

	horizontal_scan_direction = !horizontal_scan_direction;
	double height_volume  = scan_beam;
	double v_limit		  = 0.5 * pattern.volume_vertical - 0.5 * height_volume;
	antenna_elevation    += height_volume;
	if (antenna_elevation >  pattern.volume_origin_vertical + v_limit + 1e-4)
		antenna_elevation =  pattern.volume_origin_vertical - v_limit;
}

void cockpit::avSimpleRadar::update_export()
{
	RDR_mode_export = (double)mode;

	if (contacts_exported.size() < contacts.size())
		contacts_exported.resize(contacts.size());


	for (int i = 0; i < contacts_exported.size(); ++i)
	{
		contact_export & data = contacts_exported[i];
		if (i < contacts.size())
		{
			data.update_with_contact(contacts[i],i + 1);
		}
		else
		{
			data.mark_invalid();
		}
	}
}

void avSimpleRadar::on_power_on( const lwl::CSignalInfo* pInfo )
{
	mode = SCAN;
}

void avSimpleRadar::on_power_off(const lwl::CSignalInfo* pInfo)
{
	mode = NONE;
}

void avSimpleRadar::external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects)
{
	if (!render_debug_info)
		return;

	if (parser.Purpose() != Graphics::mpRender)
	{
		return;
	}
	static dbg_frustum * dbg_obj = 0;
	if (!dbg_obj)
		dbg_obj  = new dbg_frustum(10,scan_beam,scan_beam);
	else
	{
		dbg_obj->set_frustum(10,scan_speed * p_search_timer->GetDeltaTime(),scan_beam);
		dbg_obj->get_pos() = antenna_pos_local;
		dbg_obj->parse(static_cast<Graphics::RenderParser&>(parser), pos);
	}

	static dbg_frustum * dbg_volume = 0;
	if (!dbg_volume)
		dbg_volume  = new dbg_frustum(10,scan_beam,scan_beam);
	else
	{
		dbg_volume->set_frustum(10,get_current_pattern().volume_horizontal,get_current_pattern().volume_vertical);

		cPosition antenna_local_scan_origin = antenna_pos_local;

		antenna_local_scan_origin.ONEBASIS();
		antenna_local_scan_origin.rotZ(-antenna_pitch_compensation);
		antenna_local_scan_origin.rotX(-antenna_roll_compensation);
		antenna_local_scan_origin.rotZ(get_current_pattern().volume_origin_vertical);

		dbg_volume->get_pos()  = antenna_local_scan_origin;
		dbg_volume->parse(static_cast<Graphics::RenderParser&>(parser), pos);
	}
}

int  avSimpleRadar::l_set_power(lua_State *Lua_s)
{
	avSimpleRadar  *device = l_get_device_from_meta<avSimpleRadar>(Lua_s);
	if (device)
		device->setElecPower(lua_toboolean(Lua_s,2));
	return  0;
}

int  avSimpleRadar::l_get_power(lua_State *Lua_s)
{
	avSimpleRadar  *device = l_get_device_from_meta<avSimpleRadar>(Lua_s);
	lua_pushboolean(Lua_s, device && device->getElecPower());
	return  1;
}

void  avSimpleRadar::l_register_functions( lua_State *Lua_s,int index )
{
	avLuaDevice::l_register_functions(Lua_s,index);
	lua_pushstring(Lua_s, "get_power");	lua_pushcfunction(Lua_s,l_get_power);	lua_settable  (Lua_s,index);
	lua_pushstring(Lua_s, "set_power");	lua_pushcfunction(Lua_s,l_set_power);	lua_settable  (Lua_s,index);
}
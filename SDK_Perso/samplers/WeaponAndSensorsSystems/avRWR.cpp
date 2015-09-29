#include "stdafx.h"

#include "Avionics\RWR\avRWR.h"
#include "IwHumanPLane.h"
#include "cAlgorithm.h"
#include "IwWorld.h"
#include "woCar.h"
#include "woMissile.h"
#include "wRocketData.h"
#include "wsInitData.h"
#include "wsType_Level4.h"
#include "wsType_Level4_tech.h"

using namespace cockpit;

const float PositionLimit = 0.027f;

avRWR_Threat_symbol::avRWR_Threat_symbol(int n) :
avMovingMapPoint(),
is_valid(false), threat_id(n), is_locked(false)
{
	char str[3];
	itoa(n, str, 10);
	setName(str);

	dPosition p;

	p.p = dVector(0., 0., 0.);
	setPosition(p);
}

void avRWR_Threat_symbol::updateInfo(int k, const RWR_Emitter& em)
{
	threat_id = k;

	if(!em.isLocked)
	{
		azimuth  = em.Azimuth;
		strength = em.SignalStreight;
		priority = em.Priority;
		update_rwr_position();
	}

	is_valid = em.IsValid && strength > 0.0;
}


void avRWR_Threat_symbol::update_rwr_position()
{
	dPosition p;
	double x = -0.001499, 
		  y = 0.0,
		  z = 0.0;
	
	p.p = dVector(x, y, z);

	p.rotY(azimuth);
	p.p += p.x * (1.0 - MIN(strength, 0.7903099)) * 0.06;// * 0.031599f;

	if((p.p.x * p.p.x + p.p.z * p.p.z) >= (PositionLimit * PositionLimit))
	{	
		double L = sqrt(p.p.x * p.p.x + p.p.z * p.p.z) / PositionLimit;
		p.p.x /= L;
		p.p.z /= L;
	}

	p.scale(1000.);
	p.p = p.p.Scale(1000.0);

	setPosition(p);
}


bool sort_emitters_by_priority(const RWR_Emitter &a, const RWR_Emitter &b)
{
 return (a.Priority > b.Priority);
}


RWR_Emitter::RWR_Emitter()
{
	reset();
}

void RWR_Emitter::reset()
{
	IsValid = false;
	isLocked = false;
	Priority = SignalStreight = 0.0f;
	Azimuth = Elevation = 0.0f;
	BirthTime = LaunchTime = 0.0;
	client = 0;
	pFrom = 0;
	missile_id = 0;
	RadarMode = RadarType = 0;
	SignalType = RWR_SIGNAL_UNKNOWN;
	NewEmitter = Spike = true;
	WarningCounter = 0;
	DoNotControlEmissionTime = IsActiveMissile = false;
}

avRWR::avRWR() : Brightness(0.0f),
	MainEmitter(0), MainEmitterPlace(-1),
	MaxThreats(0), EmitterLiveTime(0.0),
	EmittersSize(0), SoundsOn(false),
	LaunchEventIsActive(false), LockEventIsActive(false), 
	symbols_storage(0), IsTargetSeparated(false),
	RWR_detection_coeff (1.0)
{}

avRWR::~avRWR()
{
	if (symbols_storage)
	{
		symbols_storage->DeleteAll();
		delete symbols_storage;
		symbols_storage = 0;
	}
}

void avRWR::initialize(unsigned char ID, const std::string& Name, const std::string& script_name)
{
	avLuaDevice::initialize(ID, Name, script_name);
    Lua::Config config(ensured_Lua());

	if (config.open("eyes"))
	{
		config.iterate_begin();
		while (config.iterate_next())
		{
			avSensorEye new_eye;
			new_eye.failure = false;
			if(config.open("position"))
			{
				config.get("x",&new_eye.pos.p.x);
				config.get("y",&new_eye.pos.p.y);
				config.get("z",&new_eye.pos.p.z);
				config.pop();
			}
			if(config.open("orientation"))
			{
				double   azimuth;
				double elevation;
				config.get("azimuth"  ,&azimuth);
				config.get("elevation",&elevation);
				{
					new_eye.pos.rotY(-azimuth);
					new_eye.pos.rotZ( elevation);
				}
				config.pop();
			}
			config.get("field_of_view",&new_eye.fov);
			eyes.push_back(new_eye);
		}
		config.iterate_end();
		config.pop();
	}
    config.get<unsigned short>("MaxThreats"			,&MaxThreats);
    config.get				  ("EmitterLiveTime"	,&EmitterLiveTime);
	config.get				  ("RWR_detection_coeff",&RWR_detection_coeff,1.0);
	
    emitters.resize(MaxThreats);
	EmittersSize = 0;
    make_default_activity();

	initialize_storage();
}


void avRWR::initialize_storage()
{
	Box bounds;

	bounds.min.x =  -21.0; 
	bounds.max.x =  21.0; 
	bounds.min.z = -21.0;
	bounds.max.z =  21.0;

	bounds.min.y  = -1.0f;	 
	bounds.max.y  =  1.0f;


	symbols_storage = SimpleOctTree::create_quad_tree(bounds, 0.5);
	if (!symbols_storage)
		return;

	avRWR_Threat_symbol* symbol;
	char str[3];

	for(int k = 0; k < emitters.size(); k++)
	{
		// добавляем новый символ в хранилище
		symbol = new avRWR_Threat_symbol(k);
		itoa(k, str, 10);
		symbols_storage->AddObject(str, symbol);
	}
}

// Рекурсивная функция поиска группы
void avRWR::search(int i, int gr_number)
{
	for(int j = 0; j < emitters.size(); j++)
		if(cross[i][j] == 1 && 
		   search_set.find(j) != search_set.end())
		{
			search_set.erase(j);
			groups[gr_number].insert(j);
			search(j, gr_number);
		}
}

void avRWR::dismissTgtSeparartion()
{
	char str[3];

	SimpleOctTreeLoadPtr symbol_pt;
	avRWR_Threat_symbol* symb;
    
	for(int i = 0; i < emitters.size(); i++)
	{
		// читаем из дерева текущий символ
		itoa(i, str, 10);
		symbol_pt = symbols_storage->GetByName(str);
		symb = static_cast<avRWR_Threat_symbol*>(symbol_pt.get()); 

		symb->setLocked(false);

		emitters[i].isLocked = false;

	}

}


void avRWR::calculateGroups()
{
	const int N = 16;

	IsTargetSeparated = true;

	char str[3];
	
	SimpleOctTreeLoadPtr symbol_pt;
	avRWR_Threat_symbol* symb;

	Graphics::Camera camera(Graphics::Camera::ctParallel);
	Box bounds;

	bounds.min.y =  -1.0; 
	bounds.max.y =   1.0; 

	int i, j;

	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			cross[i][j] = 0;

	symbol_ptr_list::iterator it;

	for(int i = 0; i < N; i++)
	{
		// читаем из дерева текущий символ
		itoa(i, str, 10);
		symbol_pt = symbols_storage->GetByName(str);

		if (!symbol_pt)
			continue;

		symb = static_cast<avRWR_Threat_symbol*>(symbol_pt.get()); 

		if(!symb->getValidity())
			continue;

		// настраиваем "окно" камеры для поиска пересечений с другими символами
		const Position3& pos = symb->getPosition();

		bounds.min.z = -2.f + pos.p.z;
		bounds.max.z =  2.f + pos.p.z;
		
		bounds.min.x  = -2.f + pos.p.x;	 
		bounds.max.x  =  2.f + pos.p.x;

		camera.setVolume(bounds);

		// проводим поиск всех символов, пересекающихся с текущим
		rwr_nearest_checker checker((symb->getPosition()).p,true);

		if (symbols_storage)
		{
			ClipVolume *volume = 	camera.getClipVolume();
			symbols_storage->Walk(checker,volume);
		}

		// анализ результатов поиска
		if (checker.result.empty())
			continue;
		else
		{
			// формирование матрицы пересечения
			for ( it=checker.result.begin() ; it != checker.result.end(); it++ )
			{
				j = (it->get())->getThreatId();
				if(i != j)
				{
					cross[i][j] = 1;
					emitters[j].isLocked = true;

					search_set.insert(j);
				}			
			}
		}
	}

	// Анализируем матрицу и формируем группы
	i = 0; // временный индекс для перебора всех символов
	j = 0; // текущий номер группы

	while(!search_set.empty())
	{
		if(search_set.find(i) != search_set.end())
		{
			search_set.erase(i); // символ вставлен в группу и исключается из дальнейшего поиска
			groups[j].insert(i);
			search(i, j); // поиск остальных элементов данной группы
			j++; // текущая группа сформирована
		}
		i++;
	}

	return;
}

bool avRWR::isPositionEmpty(const dPosition& pos, int exclude)
{
	Graphics::Camera camera(Graphics::Camera::ctParallel);
	Box bounds;

	bounds.min.y =  -1.0; 
	bounds.max.y =   1.0; 

	bounds.min.z = -2.f + pos.p.z;
	bounds.max.z =  2.f + pos.p.z;

	bounds.min.x  = -2.f + pos.p.x;	 
	bounds.max.x  =  2.f + pos.p.x;

	camera.setVolume(bounds);

	// проводим поиск всех символов, пересекающихся с текущим
	rwr_nearest_checker checker(pos.p, true);

	if (symbols_storage)
	{
		ClipVolume *volume = 	camera.getClipVolume();
		symbols_storage->Walk(checker,volume);
	}

	// анализ результатов поиска
	if (checker.result.empty())
		return true;
	else
		if(checker.result.size() == 1 && 
		   (checker.result.begin()->get())->getThreatId() == exclude)
		   return true;

	return false;
}

void avRWR::separateGroups()
{
	int n;
	int n_ext = 0;
//    int i;
	char str[3];

	SimpleOctTreeLoadPtr symbol_pt;
	avRWR_Threat_symbol* symb;

	double    ref_azimuth; // азимут символа с максимальным приоритетом
	dPosition ref_pos;     // позиция символа с максимальным приоритетом

	// разреживаем символы в группах
	for(int k = 0; k < 8; k++)
	{
		if(!groups[k].empty())
		{
			n = 0;
			// ищем символ с максимальным приоритетом
			for(int j = 0; j < emitters.size(); j++)
			{
				if(groups[k].find(j) != groups[k].end())
				{
					n++;
					
					
					itoa(j, str, 10);
					symbol_pt = symbols_storage->GetByName(str);
					symb = static_cast<avRWR_Threat_symbol*>(symbol_pt.get()); 

					// cимвол с максимальным приоритетом оставляем на месте
					if(n <= 1)
					{
						ref_azimuth = symb->getAzimuth();
						ref_pos = symb->getPosition();
						continue;
					}

					// на радиусе помещаются максимум 2 символа
					// поэтому только второй символ проверяем на возможность
					// поместить на радиус
					if(n == 2)
					{
						double d = sqrt(ref_pos.p.x * ref_pos.p.x + ref_pos.p.z * ref_pos.p.z);
		
						if((PositionLimit * 1000) - d >= 8.0)
						{
							// вставляем символ
							dPosition norm = ref_pos;
							norm.scale(1. / d);
							norm.p = norm.p.Scale(1. / d);
							norm.scale(10.);
							norm.p = norm.p.Scale(10.0);
							dPosition new_pos = dPosition(ref_pos.x,ref_pos.y,ref_pos.z,ref_pos.p + norm.p);
							if(isPositionEmpty(new_pos, j))
							{
								symb->setPosition(new_pos);
							    continue;
							}
						}
					}
					
					// ищем новое место для символа					
					//Position3 p = symb->getPosition();
					//Position3& p = element->get_position_ref();
					//p.RotateX(-90. + TO_RAD(args[0] * 22.5f));
					//p.p += p.y * (1.0 - emitter.SignalStreight) * ind->getCurrentVertexScaleModifier();
					//p.RotateX(-TO_RAD(args[0] * 22.5f));

					//while()
					n_ext++;

					ref_pos.rotY(-TO_RAD((n_ext) * 22.5));
					ref_pos.p = ref_pos.x * (1.0) * 0.9 * 0.031599;
					ref_pos.rotY(TO_RAD((n_ext) * 22.5));
					
					symb->setPosition(ref_pos);
					symb->setLocked(true);

				}
			}
		}
	}
	// очищаем группы
	for(int k = 0; k < 8; k++)
		groups[k].clear();
	
}


void avRWR::changeBrightness()
{
	Brightness = Brightness <= 0.0f ? 1.0f : Brightness - 0.1f;
}

void avRWR::update()
{
	if (MaxThreats > 0)
	{
	    IwHumanRWR* RWR_Interace = aircraft().human ? aircraft().human->getRWRInterface(): 0;
		if (RWR_Interace)
		{
			RWR_events_queue& events_queue = RWR_Interace->get_RWR_events_queue();
			if (getElecPower() && !getflag_failure())
			{
				while (events_queue.size())
				{
					tryInsertEmitter(events_queue.back());
					events_queue.pop_back();
				
					// sort by priority
					if(emitters.size() > 5)
						sort(emitters.begin(), emitters.end(), sort_emitters_by_priority);
				
					// update tree
					char str[3];
					SimpleOctTreeLoadPtr symbol_pt;
					avRWR_Threat_symbol* symb;

					for(int k = 0; k < emitters.size(); k++)
					{
						itoa(k, str, 10);
						symbol_pt = symbols_storage->GetByName(str);
						symb = static_cast<avRWR_Threat_symbol*>(symbol_pt.get()); 

						if(symb)
							symb->updateInfo(k, emitters[k]);
					}
				}

				if(SeparateMode && !IsTargetSeparated)
				{
					calculateGroups();
					separateGroups();
				}
				updateEmitters();
			}
			else
			{
				if (events_queue.size())
					events_queue.clear();

				if (EmittersSize)
					ClearEmitters();
			}
		}
		checkLaunchEvent();
		checkLockEvent();
	}
}

void avRWR::tryInsertEmitter(const RWR_event& event)
{
	if (!event.client)
		return;

	woPointer pFrom = globalWorld->Get_woPointer(event.client);
	if (!pFrom)
		return;
	
	wModelTime CurrentTime = wTime::GetModelTime();

	int L1 = pFrom->Type().Level1();
	int L2 = pFrom->Type().Level2();
    if (L1 == wsType_Static && 
		(L2 == wsType_Airdrome || L2 == wsType_AirdromePart))
	{
		return;
	}

	int L3 = pFrom->Type().Level3();
	int L4 = pFrom->Type().Level4();
	
	bool IsActiveMissile = false;
	bool standalone_missile = false;

	if (L1 == wsType_Weapon && L2 == wsType_Missile)
	{
		if (event.RadarMode_ == wsRadarActiveHoming &&
			 L3 == wsType_AA_Missile &&
			 (L4 == MICA_R ||
			  L4==P_27AE ||
			  L4==P_77 ||
			  L4==P_37 ||
			  L4==AIM_54 ||
			  L4==AIM_120 ||
			  L4==AIM_120C)
			)
		{
			IsActiveMissile = true;
		}

		woPointer pGun = globalWorld->Get_woPointer(pFrom->GunId());

		if (!pGun)
		{
			if (!IsActiveMissile)
				return;
			else
				standalone_missile = true;
		}

		woMissile* theMissile = static_cast<woMissile*>(&*pFrom);
		int Head_Type = theMissile->Get_Head_Type();

		if (Head_Type != AntiRadar && 
			Head_Type != ActiveRadar &&
			Head_Type != SemiActiveRadar)
			return;

		bool EmitterIsInHOJ = (Head_Type == AntiRadar) ? true : false;
		
		if (!standalone_missile)
		{
			ObjectID parent_fromID = pGun->ID();

			woPointer pFromParent = globalWorld->Get_woPointer(parent_fromID);
			
			if (!pFromParent)
				return;

			int PL1 = pFromParent->Type().Level1();
			int PL2 = pFromParent->Type().Level2();
			int PL3 = pFromParent->Type().Level3();
			int PL4 = pFromParent->Type().Level4();

			bool RadarInCRMode	= false;
			int Parent_i		= -1;

			for (int i = 0; i < emitters.size(); i++)
			{
				if (!emitters[i].IsValid)
					continue;

				if (emitters[i].client == parent_fromID)
				{
					if (emitters[i].RadarType == wsMailRadarModeCR)
					{
						RadarInCRMode = true;
						continue;
					}
					
					if (emitters[i].RadarType == wsMailRadarModeTWS)
						continue;

					if (EmitterIsInHOJ && 
						(emitters[i].RadarType != wsMailRadarModeLock)
					   )
					 	continue;

					Parent_i = i;
					break;
				}
			}

			// устанавливаем пуск
			if (Parent_i > -1)
			{
				emitters[Parent_i].RadarType	= wsMailRadarModeCR;
				emitters[Parent_i].SignalType	= RWR_SIGNAL_LAUNCH;
				emitters[Parent_i].missile_id	= event.client;

				// это новый факт пуска
				emitters[Parent_i].LaunchTime        = CurrentTime;

				selectMainEmitter(); // ищем главный эмиттер по приоритету
			}
			else
			{
				// радары наведения ракет
				if (!RadarInCRMode && 
					(PL1 == wsType_Ground && PL2==wsType_SAM) &&
					(PL4 == V_40B6M ||
					 PL4 == RLS_5H63C || 
					 PL4 == RLS_9C32_1 || 
					 PL4 == Hawk_Track_Radar || 
					 PL4 == BUK_PU)
				   )
				{
					float Azimuth = 0.0f, Elevation = 0.0f;
					cPosition currPlane_pos = aircraft().human_ptr->Position(wTime::GetModelTime());
					cVector world_direction;

					if (getEmitterPos(pFromParent, currPlane_pos, Azimuth, Elevation, world_direction) == true)
					{
						int L1 = pFromParent->Type().Level1(); 
						int L2 = pFromParent->Type().Level2();
						int L3 = pFromParent->Type().Level3();
						int L4 = pFromParent->Type().Level4();

						float SignalStreight = getSignalStreigth(pFromParent, currPlane_pos, L1, L2);

						if (SignalStreight > 0.0f)
						{
							woCar* theTrackRadar	 = static_cast<woCar*>(&*pFromParent);
							unsigned int RadarMode_ = theTrackRadar->getRadarType();
							unsigned int RadarType_ = wsMailRadarModeCR;
							float Priority = calc_priority(RadarMode_,
								RadarType_, SignalStreight);

							checkEmittersSize();
							
							// удаляем облучатель с минимальным приоритетом
							if (EmittersSize == MaxThreats)
							{
								float LowestPriority = 0.0f;
								int LP_i = findWithLowestPriority(LowestPriority);

								if (LP_i > -1)
								{
									if (Priority > LowestPriority)
									{
										// вставляем новый вместо облучателя с минимальным
										// приоритетом
										SetEmitterIsNotValid(LP_i);
									}
									else
										// приоритет нового меньше наименьшего из уже имеющихся
										return;
								}
							}

							int New_i = findNewPlace();

							if (New_i <= -1)
								return;
										
							emitters[New_i].client			= parent_fromID;
							emitters[New_i].pFrom			= pFromParent;
							emitters[New_i].EmitterType		= wsType(L1, L2, L3, L4);
							emitters[New_i].EmitterTypeStr	= emitters[New_i].pFrom ? emitters[New_i].pFrom->unit_type : "";
							
							emitters[New_i].Azimuth			= Azimuth;
							emitters[New_i].Elevation		= Elevation;
							emitters[New_i].Direction		= world_direction;
							emitters[New_i].BirthTime		= CurrentTime;
							emitters[New_i].RadarMode		= RadarMode_;
							emitters[New_i].RadarType		= RadarType_;
							emitters[New_i].SignalStreight   = SignalStreight;
							emitters[New_i].Priority         = Priority;
							
							NewestEmitter				= parent_fromID;
					
							setEmitterTypes(pFromParent, emitters[New_i]);
							emitters[New_i].SignalType		= RWR_SIGNAL_LAUNCH;

							emitters[New_i].missile_id		= event.client;
							// это новый факт пуска
							emitters[New_i].LaunchTime		= CurrentTime;

							SetEmitterIsValid(New_i);
			
							//... уникальная ты блин наша
							if (PL1 == wsType_Ground && 
								PL2 == wsType_SAM && 
								PL4 == BUK_PU)
								emitters[New_i].DoNotControlEmissionTime = true;

							selectMainEmitter();
						}
					}
				}
			}
		}

		if (!IsActiveMissile)
			return;
	}

	float Azimuth = 0.0f, Elevation = 0.0f;
	cPosition currPlane_pos = aircraft().human_ptr->Position(wTime::GetModelTime());
	cVector world_direction;
	
	if (!getEmitterPos(pFrom, currPlane_pos, Azimuth, Elevation, world_direction))
		return;
	
	float SignalStreight = 0.0f, Priority = 0.0f;
	
	SignalStreight = getSignalStreigth(pFrom, currPlane_pos, L1, L2);
	if (!SignalStreight)
		return;

	unsigned int RadarType_ = event.RadarType_;
	ObjectID missile_id		 = 0;
	int WarningCounter       = 0;
	bool IsNewEmitter        = true;
	wModelTime LaunchTime	 = CurrentTime;
	int PlaceToInsert		 = -1;

	// здесь также находим подсвечивающий ракету эмиттер
	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid || emitters[i].client != event.client)
			continue;
		
		WarningCounter = emitters[i].WarningCounter;
		LaunchTime     = emitters[i].LaunchTime;

		if (emitters[i].RadarType == wsMailRadarModeCR)
		{
			if (checkEmitter(emitters[i].client, emitters[i].missile_id))
			{
				RadarType_ = wsMailRadarModeCR;
				missile_id = emitters[i].missile_id;
			}
			else
			{
				// отдельностоящие радары наведения ракет
				if ((L1 == wsType_Ground && L2 == wsType_SAM) &&
					(L4 == V_40B6M ||
						L4 == RLS_5H63C ||
						L4 == RLS_9C32_1 ||
						L4 == Hawk_Track_Radar ||
						L4 == BUK_PU)
					)
				{
					SetEmitterIsNotValid(i);
					selectMainEmitter(); // ищем главный эмиттер по приоритету
					return;
				}

				// или обнуляем счетчик сигнала о пуске для остальных
				WarningCounter = 0;
				LaunchTime     = CurrentTime;
			}
		}
		
		if (event.RadarMode_ != wsRadarActiveHoming && 
			RadarType_ != wsMailRadarModeCR && 
			RadarType_ != wsMailRadarModeLock)
		{
			// здесь искусственно увеличиваем время, 
			// через которое источник облучения обновляется
			if (CurrentTime - emitters[i].BirthTime < 3.0f)
				return;
		}

		IsNewEmitter   = false;
		PlaceToInsert  = i;
		break;
	}

	// Отдельно расположенные радары наведения ракет
	// должны присутствовать на экране только в процессе подсвета ракеты.
	if (PlaceToInsert <= -1 &&
		(L1 == wsType_Ground && L2==wsType_SAM) &&
		(L4 == V_40B6M ||
		 L4 == RLS_5H63C ||
		 L4 == RLS_9C32_1 ||
		 L4 == Hawk_Track_Radar ||
		 L4 == BUK_PU)
		)
		return;

	/*
	// самое время проверить
	// соответствие режиму работы системы
	if (((RadarMode != wsRadarActiveHoming) && 
		(RadarType_ != wsMailRadarModeCR)    && 
		(RadarType_ != wsMailRadarModeLock)) &&
		(human->cws->getRWRMode() != whRWRMode_All))
	{
		selectMainEmitter();
		return true;
	}
	*/

	Priority = calc_priority(event.RadarMode_, RadarType_, SignalStreight);
	
	// удаляем, если находим
	
	// это условие работает при наличии ограничения на 
	// количество одновременно отображаемых эмиттеров
	if (PlaceToInsert <= -1) // если новый
	{
		checkEmittersSize();

		// удаляем облучатель с минимальным приоритетом
		if (EmittersSize == MaxThreats)
		{
			float LowestPriority = 0.0f;
			int LP_i = findWithLowestPriority(LowestPriority);

			if (LP_i > -1)
			{
				if (Priority > LowestPriority)
				{
					// вставляем его вместо облучателя с минимальным
					// приоритетом
					SetEmitterIsNotValid(LP_i);
				}
				else
					// приоритет нового меньше наименьшего из уже имеющихся
					return;
			}
		}
		
		NewestEmitter = event.client;
		PlaceToInsert = findNewPlace();
	}

	if (PlaceToInsert > -1)
	{
		emitters[PlaceToInsert].client			  = event.client;
		emitters[PlaceToInsert].pFrom			  = pFrom;
		emitters[PlaceToInsert].EmitterType		  =	wsType(L1, L2, L3, L4);
		emitters[PlaceToInsert].EmitterTypeStr	  = pFrom ? pFrom->unit_type : "";
		emitters[PlaceToInsert].Azimuth			  = Azimuth;
		emitters[PlaceToInsert].Elevation		  = Elevation;
		emitters[PlaceToInsert].Direction		  = world_direction;
		emitters[PlaceToInsert].BirthTime		  = CurrentTime;
		emitters[PlaceToInsert].RadarMode		  = event.RadarMode_;
		emitters[PlaceToInsert].RadarType		  = RadarType_;
		emitters[PlaceToInsert].SignalStreight    = SignalStreight;
		emitters[PlaceToInsert].Priority          = Priority;
		emitters[PlaceToInsert].missile_id        = missile_id;
		emitters[PlaceToInsert].Spike			  = true;
		emitters[PlaceToInsert].WarningCounter    = WarningCounter;
		emitters[PlaceToInsert].LaunchTime        = LaunchTime;
		emitters[PlaceToInsert].IsActiveMissile	  = IsActiveMissile;
		SetEmitterIsValid(PlaceToInsert);

		// если новый то фиксируем это
		emitters[PlaceToInsert].NewEmitter		  = IsNewEmitter;

		setEmitterTypes(pFrom, emitters[PlaceToInsert]);
		selectMainEmitter();	
	}
}

void avRWR::updateEmitters()
{
	if (MaxThreats <= 0)
		return;

	/*
	if (getflag_failure() && plane->FlagFailureRWS)
		return;

	if (plane->FlagFailureRWS && !failure)
	{
		SetShootOffSounds(true);
		return;
	}
	*/

	// перезаполняем
	bool deletedEmitters = false;
	wModelTime CurrentTime = wTime::GetModelTime();
	
	// удаляем, если истекло время, или облучатель пропал
	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid)
			continue;

		if (!emitters[i].DoNotControlEmissionTime && 
			CurrentTime - emitters[i].BirthTime > EmitterLiveTime)
		{
			if (MainEmitter && emitters[i].client == MainEmitter->client)
			{
				MainEmitter			= 0;
				MainEmitterPlace	= -1;
			}

			deletedEmitters = true;
			SetEmitterIsNotValid(i);
		}
		// убираем признак пуска
		else if (emitters[i].RadarMode == wsRadarActiveHoming ||
			emitters[i].RadarType == wsMailRadarModeCR)
		{
			if (!checkEmitter(emitters[i].client, emitters[i].missile_id))
			{
				// отдельный радар наведения просто удаляем
				if ((emitters[i].EmitterType.Level1() == wsType_Ground &&
					 emitters[i].EmitterType.Level2() == wsType_SAM) &&
					(emitters[i].EmitterType.Level4() == V_40B6M ||
					 emitters[i].EmitterType.Level4() == RLS_5H63C ||
					 emitters[i].EmitterType.Level4() == RLS_9C32_1 ||
					 emitters[i].EmitterType.Level4() == Hawk_Track_Radar ||
					 emitters[i].EmitterType.Level4() == BUK_PU)
					)
				{
					if (MainEmitter && emitters[i].client == MainEmitter->client)
					{
						MainEmitter			= 0;
						MainEmitterPlace	= -1;
					}

					deletedEmitters = true;
					SetEmitterIsNotValid(i);
				}
				else // иначе ставим признак "обзор"
				{
					if (MainEmitter && emitters[i].client == MainEmitter->client) 
					{
						MainEmitter			= 0;
						MainEmitterPlace	= -1;
					}

					emitters[i].RadarType       = wsMailRadarModeScan;
					emitters[i].SignalType		= RWR_SIGNAL_SEARCH;
					emitters[i].Priority        = calc_priority(emitters[i].RadarMode, emitters[i].RadarType, emitters[i].SignalStreight);
					emitters[i].WarningCounter	= 0;
					emitters[i].missile_id		= 0;
				}
			}
		}
	}

	if (!MainEmitter)
		selectMainEmitter();

	/*
	// для СПО-15ЛМ
	if (deletedEmitters)
		UpdateTypes();
	*/

	cPosition currPlane_pos = aircraft().human_ptr->Position(wTime::GetModelTime());
	cVector world_direction;
	
	float Azimuth = 0.0f, Elevation = 0.0f;
	deletedEmitters = false;
	
	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid)
			continue;

		if (emitters[i].RadarMode == wsRadarActiveHoming ||
			emitters[i].RadarType == wsMailRadarModeCR ||
			emitters[i].RadarType == wsMailRadarModeLock)
		{
			// это для источников непрерывного облучения
			if (!getEmitterPos(emitters[i].pFrom, currPlane_pos, Azimuth, Elevation, world_direction))
			{
				SetEmitterIsNotValid(i);
				deletedEmitters = true;
			}
		}
	}

	if (deletedEmitters)
	{
		selectMainEmitter();
		//UpdateTypes(); // для СПО-15ЛМ
	}
}

// функция для постоянно облучающих источников (ракет с активной ГСН),
// и для проверки, что пущенная ракета еще жива
bool avRWR::checkEmitter(ObjectID fromID, ObjectID missile_id)
{
	if (!fromID)
		return false;

	woPointer pFrom = globalWorld->Get_woPointer(fromID);

	if (!pFrom)
		return false;

	int L1 = pFrom->Type().Level1();
	int L2 = pFrom->Type().Level2();

	// проверяем не ракета ли, и есть ли захват на нас
	if (L1 == wsType_Weapon && L2 == wsType_Missile)
	{
		woMissile* theMissile = static_cast<woMissile*>(&*pFrom);
		if (theMissile->GetNoTarget() || theMissile->GetTargetID() != aircraft().human_ptr->ID())
			return false;
	}
	else
	// проверяем наличие ракеты у подсвечивающего объекта,
	// и есть ли захват на нас у той ракеты
	//
	//else if ((L1 == wsType_Ground) && (L2==wsType_SAM))
	{
		woPointer pFrom = globalWorld->Get_woPointer(missile_id);

		if (!pFrom)
			return false;

		int L1 = pFrom->Type().Level1();
		int L2 = pFrom->Type().Level2();

		if (L1 == wsType_Weapon && L2 == wsType_Missile)
		{
			woMissile* theMissile = static_cast<woMissile*>(&*pFrom);
			if (theMissile->GetNoTarget() || theMissile->GetTargetID() != aircraft().human_ptr->ID())
				return false;
		}
	}

	return true;
}

bool avRWR::getEmitterPos(woPointer pFrom, cPosition& currPlane_pos,
		float& Azimuth, float& Elevation, cVector& world_direction)
{
	if (!pFrom)
		return false;

	//Проверяем видимость угрозы датчиками
	static cVector LocalDir;
	mult_transp(LocalDir,currPlane_pos,currPlane_pos.p - (pFrom->Position()).p);

	bool success = false;
	for (int i = 0; 
		i < eyes.size();
		++i)
	{
		if (!eyes[i].failure && eyes[i].inside(LocalDir))
		{
			success = true;
			break;
		}
	}

	if (!success)
		return false;

	//Переворачиваем вектор
	LocalDir *= -1.0;

	LocalDir.norm();

	//Заполняем выходные данные
	Elevation = atan2(LocalDir.y, sqrt(LocalDir.x * LocalDir.x + LocalDir.z * LocalDir.z));
	Azimuth = -atan2(LocalDir.z, LocalDir.x);
	if (Azimuth < 0.0)
		Azimuth = Pi_2 + Azimuth;

	return true;
}

void avRWR::selectMainEmitter()
{
	float MaxPriority = 0.0f;

	MainEmitter			= 0;
	MainEmitterPlace	= -1;
	LockLaunch = RWR_SIGNAL_SEARCH;

	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid)
			continue;

		if (MaxPriority < emitters[i].Priority)
		{
			MaxPriority			= emitters[i].Priority;
			MainEmitter			= &emitters[i];
			MainEmitterPlace	= i;
		}
	}

	if (MainEmitter)
	{
		if (MainEmitter->RadarType == wsMailRadarModeLock)
			LockLaunch = RWR_SIGNAL_LOCK;
		else if ((MainEmitter->RadarMode == wsRadarActiveHoming) ||
			(MainEmitter->RadarType == wsMailRadarModeCR))
			LockLaunch = RWR_SIGNAL_LAUNCH;
	}
}

float avRWR::getSignalStreigth(woPointer pFrom, const cPosition& currPlane_pos, int L1, int L2) const
{
	if (!pFrom)
		return 0.0f;

	float SignalStreight, currDistance;
	float maxDistance = 0.0;

	float radar_detection_distance = 0.0f;
	bool lockedOn = false;
	
	switch (L1)
	{
	case wsType_Air:
	case wsType_Ground:
	case wsType_Navy:

		if(pFrom->isRadiatedByRadar(aircraft().human_ptr->ID(),lockedOn,radar_detection_distance))
		{
			currDistance = ((pFrom->Position()).p - currPlane_pos.p).length();
			maxDistance  = RWR_detection_coeff * radar_detection_distance;
		}
		break;

	case wsType_Weapon:
		if (L2==wsType_Missile)
		{
			currDistance = ((pFrom->Position()).p - currPlane_pos.p).length();
			maxDistance  = RWR_detection_coeff * 
							15000.0f;
		}
		break;
    default:
		break;
	}

	if (currDistance > maxDistance || !maxDistance)
		SignalStreight = 0.0f;
	else
		SignalStreight	= 1.0f - currDistance/maxDistance;

	return SignalStreight;
}

float avRWR::calc_priority(unsigned int RadarMode, unsigned int RadarType_, float SignalStreight) const
{
	float Priority = 0;

	// считаем приоритет
	// принцип расчета взят из описания в методичке №2 по БП МиГ-29
	// т.е. для СПО и RWR они одинаковы, что близко к истине

	// ракета с активной ГСН
	if (RadarMode == wsRadarActiveHoming)
		Priority += 1000;

	// тип: пуск ракеты/сопровождение/обзор
	if (RadarType_ == wsMailRadarModeCR)
		Priority += 300;
	else if (RadarType_ == wsMailRadarModeLock)
		Priority += 200;
	else 
		Priority += 100;

	// тип заложенный в логику прибора
	if (RadarMode == wsRadarAir)
		Priority += 60;
	else if (RadarMode == wsRadarLongRange)
		Priority += 50;
	else if (RadarMode == wsRadarMidRange)
		Priority += 40;
	else if (RadarMode == wsRadarShrtRange)
		Priority += 30;
	else if (RadarMode == wsRadarEWS)
		Priority += 20;
	else if (RadarMode == wsRadarAWACS)
		Priority += 10;

	// наконец, мощность сигнала
	Priority += SignalStreight;

	return Priority;
}

void avRWR::setEmitterTypes(woPointer pFrom, RWR_Emitter& TempEmitter)
{
	if (!pFrom)
		return;

	const unsigned int& RadarMode = TempEmitter.RadarMode;
	const unsigned int& RadarType = TempEmitter.RadarType;

	if ((RadarMode == wsRadarActiveHoming) || (RadarType == wsMailRadarModeCR))
        TempEmitter.SignalType = RWR_SIGNAL_LAUNCH;
	else if (RadarType == wsMailRadarModeLock)
        TempEmitter.SignalType = RWR_SIGNAL_LOCK;
	else
        TempEmitter.SignalType = RWR_SIGNAL_SEARCH;

	int L1 = pFrom->Type().Level1();
	int L2 = pFrom->Type().Level2();
	int L3 = pFrom->Type().Level3();
	int L4 = pFrom->Type().Level4();

	if (L1 == wsType_Weapon && L2 == wsType_Missile)
	{
		TempEmitter.SignalType = RWR_SIGNAL_LAUNCH;
						
		if (L3 == wsType_AA_Missile)
		{
			if (!TempEmitter.IsActiveMissile)
			{
				TempEmitter.PlatformType = RWR_PLARFORM_UNKNOWN;
				return;
			}
		}
	}

    switch (L1)
	{
	case wsType_Air:
		TempEmitter.PlatformType = RWR_PLARFORM_AIR;
		
		break;
	case wsType_Ground:
		TempEmitter.PlatformType = RWR_PLARFORM_GROUND;

		if (L2==wsType_SAM)
		{
			if (L3==wsType_Radar)
			{
				if ((L4==EWR_1L13)||(L4==EWR_55G6))
                	TempEmitter.PlatformType = RWR_PLARFORM_UNKNOWN;
			}
		}

		break;
	case wsType_Navy:
		TempEmitter.PlatformType = RWR_PLARFORM_NAVY;
		break;
	case wsType_Weapon:
		TempEmitter.PlatformType = RWR_PLARFORM_WEAPON;
		break;
    default:
		TempEmitter.PlatformType = RWR_PLARFORM_UNKNOWN;
		break;
	}
}

int avRWR::findNewPlace()
{
	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid)
			return i;
	}

	return -1;
}

int avRWR::findWithLowestPriority(float& LowestPriority)
{
	LowestPriority = 2000.0f;
	int MinPriority_i = -1;

	for (int i = 0; i < emitters.size(); i++)
	{
		if (emitters[i].IsValid)
		{
			if (emitters[i].Priority < LowestPriority)
			{
				LowestPriority = emitters[i].Priority;
				MinPriority_i = i;
			}
		}
	}

	return MinPriority_i;
}

void avRWR::checkEmittersSize()
{
	float LowestPriority = 0.0f;

	while (EmittersSize > MaxThreats)
	{
		int LP_i = findWithLowestPriority(LowestPriority);
		SetEmitterIsNotValid(LP_i);
	}
}

void avRWR::SetEmitterIsValid(int EmitterPos)
{
	if (!emitters[EmitterPos].IsValid)
	{
		emitters[EmitterPos].IsValid = true;
		EmittersSize++;
		if(emitters[EmitterPos].pFrom && sound.lock_new[(emitters[EmitterPos].pFrom->Type()).Type()])
		 EmmitersCounter[(emitters[EmitterPos].pFrom->Type()).Type()]++;
		else
		 EmmitersCounter[defaultWsType.Type()]++;
		 
	}
}

void avRWR::SetEmitterIsNotValid(int EmitterPos)
{
	if (emitters[EmitterPos].IsValid)
	{
		// stop lock sound
		if(emitters[EmitterPos].EmitterType.Type() && sound.lock_new[emitters[EmitterPos].EmitterType.Type()])
		{
			EmmitersCounter[emitters[EmitterPos].EmitterType.Type()]--;

			if (EmmitersCounter[emitters[EmitterPos].EmitterType.Type()] == 0 &&
				sound.lock_new[emitters[EmitterPos].EmitterType.Type()])
				sound.lock_new[emitters[EmitterPos].EmitterType.Type()]->stop();
		}
		else
		{
			EmmitersCounter[defaultWsType.Type()]--;

			if(EmmitersCounter[defaultWsType.Type()] == 0 &&
				sound.lock_new[defaultWsType.Type()])
				sound.lock_new[defaultWsType.Type()]->stop();
		}	

		emitters[EmitterPos].reset();
		EmittersSize--;
	}
}

void avRWR::ClearEmitters()
{
	for (int i = 0; i < emitters.size(); i++)
		SetEmitterIsNotValid(i);
}

void avRWR::checkLaunchEvent()
{
	LaunchEventIsActive = false;

	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid)
			continue;

		if (emitters[i].SignalType == RWR_SIGNAL_LAUNCH)
		{
			LaunchEventIsActive = true;
			LaunchingSource = emitters[i].pFrom;
			break;
		}
	}
}

void avRWR::checkLockEvent()
{
	LockEventIsActive = false;

	for (int i = 0; i < emitters.size(); i++)
	{
		if (!emitters[i].IsValid)
			continue;

		if (emitters[i].SignalType == RWR_SIGNAL_LOCK || 
			emitters[i].SignalType == RWR_SIGNAL_LAUNCH && emitters[i].RadarType == wsMailRadarModeCR)
		{
			LockEventIsActive = true;
			LockingSource = emitters[i].pFrom;
			LockingType = emitters[i].RadarMode;
			break;
		}
	}
}

const bool avRWR::isDegraded()const
{
	for (int i = 0; i < eyes.size(); i++)
		if (eyes[i].failure)
			return true;

	return false;
}

#ifdef _DEBUG
#include "Modeler/ModelParser.h"
void avRWR::external_parse(Graphics::ModelParser& parser,const Position3& pos,Graphics::effectState* effects)
{
	return;
	if (parser.Purpose() != Graphics::mpRender)
	{
		return;
	}
	for (int i = 0 ; i < eyes.size() ; ++i)
	{
		eyes[i].parse(static_cast<Graphics::RenderParser &>(parser),pos);
	}
}
#endif

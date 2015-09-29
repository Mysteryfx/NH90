#ifndef __wsTarget_h__
#define __wsTarget_h__

#include "WorldGeneral.h"
#include "Serializer\Serializer.h"
#include "wMovingObject.h"
#include "wTime.h"
#include "wConst.h"
#include <ed/vector.h>
#include "AI\Detection\wTargetDetectionStatus.h"

enum wsTargetType  // виды и подвиды целей
{ 
	wsTarget_Nothing,	           // Не цель
    wsTarget_Ground,	           // Oбычная наземная цель (мост, дот)
    wsTarget_Air,	               // Воздушная цель
	wsTarget_Plane,				   // Самолет или вертолет
	              // Виды атаки
    wsTarget_Attack_Air_Target_	= Attack_Air_Target_,// Атака воздушной цели (и подвид).  
    wsTarget_Bombing_			= Bombing_,		   // Бомбежка. 
    wsTarget_Missile_Gr_Attack_	= Missile_Gr_Attack_,// Ракетная атака наземной цели ( и подвид) 
    wsTarget_Gun_Gr_Attack_		= Gun_Gr_Attack_,    // Aтака наземной цели из пушки. 
    wsTarget_Attack_Gr_Target_	= Attack_Gr_Target_, // Атака наземной цели всеми средствами ( и подвид). 
    wsTarget_Rocket_Attack_		= Rocket_Attack_,    // Атака неуправляемями реактивными снарядами. 
    wsTarget_Antiradar_			= Antiradar_,        // Antiradar (подвид)
    wsTarget_Antiship_Strike_	= Antiship_Strike_,  // Корабль   (подвид)
    wsTarget_Close_Air_Support_	= Close_Air_Support_,// Close_Air_Support 
    wsTarget_Ground_Attack_		= Ground_Attack_,    // Ground_Attack
    wsTarget_Pinpoint_Strike_	= Pinpoint_Strike_,  // Атака наземной цели точным оружием
    wsTarget_Runway_Attack_		= Runway_Attack_,    // Runway_Attack (подвид)
    wsTarget_Tank				= Tank_Attack_,      // Танк (защищенная наземная цель)(подвид)
	wsTarget_Point,				   // Просто точка с известными координатами
	wsTarget_Structure,			   // Здания, сооружения
	wsTarget_People,			   // Живая сила
	// Виды целей
    wsTarget_GroundTarget,	       // Вид - наземная цель
    wsTarget_AirTarget,	           // Вид - воздушная цель

    wsTarget_AirPlane,	           // Вид - самолет/вертолет
    wsTarget_Weapon,	           // Вид - мелкое оружие (ракета/бомба/снаряд)
    wsTarget_Missile,              // Вид - крупная ракета
    wsTarget_Helicopter            // Вид - вертолет
};

//////////////////////////////////////////////////////////////////////////

struct WORLDGENERAL_API wsTargetState 
{
	struct WORLDGENERAL_API WignmenData
	{
		WignmenData();
	public:
		//detection
		wTargetDetectionStatus detection_status;
		//target
		wModelTime	validTime; //Time when the target will be valid for attack, it can be invalid for attack if weapon is already flying toward the target now
		bool		targetClosed; //Target is impossible to attack due its current location
		bool		attackedAsAThreat; //Is target attacked as a threat
	};
	//
	wsTargetState();
	~wsTargetState();

	// Пришлось декларировать иначе под интелом не линкуется.Mitka
	wsTargetState(const wsTargetState & arg);
	wsTargetState &operator = (const wsTargetState & state);
	WORLDGENERAL_API friend Serializer& operator << (Serializer & serializer, wsTargetState & data);
	//
	bool			isValid(int wingmen_num = -1) const;
	//
	woPointer		ptr;
#ifdef DEBUG_TEXT
	ed::string		obj_type_name;
#endif
	unsigned int	T_ID;       //Target id
	wsTargetType    T_Type;     //Target type
	wsTargetType    T_SubType;  //Target subtype
	int				Who;        //Index of wingmen which the target is assigned to
	int				missiles;	//How many missiles are fired on the target by wing
	unsigned int	Priority;   //Target priority
	unsigned char	reportType; //How to report about the target
#ifdef DEBUG_TEXT
	ed::string		obj_array_name;
#endif
	wsTargetType    Attack_Sort;//Attack method
	wModelTime		last_seen_time;//Last seen time, used to forget the target
	WignmenData		wingmens_data[MAX_WING_MEMBERS_NUM];
};

typedef ed::map<ObjectID, wsTargetState> WingTargets;

#endif


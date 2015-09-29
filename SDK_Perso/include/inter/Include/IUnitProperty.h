#pragma once


enum enum_UnitProperty
{
    UNIT_INVALID_PROPERTY = 0, // for RT checks
    UNIT_RUNTIME_ID, // unique within runtime mission. int
    UNIT_MISSION_ID, // unique within mission file. int>0
    UNIT_NAME, // unit name, as assigned by mission designer.
    UNIT_TYPE, // unit type (Ural, ZU-23, etc)
    UNIT_CATEGORY,
    UNIT_GROUP_MISSION_ID, // group ID, unique within mission file. int>0
    UNIT_GROUPNAME, // group name, as assigned by mission designer.
    UNIT_GROUPCATEGORY,
    UNIT_CALLSIGN,
    UNIT_HIDDEN,// ME hiding  
    UNIT_COALITION,// "blue", "red" or "unknown" 
    UNIT_COUNTRY_ID,
    UNIT_TASK, //"unit.group.task"
    UNIT_PLAYER_NAME, // valid for network "humanable" units
	UNIT_ROLE,//"artillery_commander", "instructor", etc
    UNIT_INVISIBLE_MAP_ICON,//ME invisible map icon
    ///
    UNIT_MAXIMUM_PROPERTY
};
#pragma once

#include "WorldGeneral.h"

#include "cLinear.h"

//Returns maximal angle of elevation from start_point to end_point due the objects (objFlags) on the LOS
float get_max_elevation_with_objects(	const cPoint & start_point,
										const cPoint & end_point,
										unsigned long objFlags); //Object viFlags

//Returns maximal angle of terrain elevation from start_point to end_point. Objects (objFlags) are accounted too.
float WORLDGENERAL_API get_max_elevation(	const cPoint & start_point,
											const cPoint & end_point,
											unsigned long objFlags = 0); //Object viFlags

//Returns maximal angle of terrain elevation from start_point along the direction of azimuth on a horizontal distance of hor_dist
//and within a sector of azimuth_sector. Objects (objFlags) are accounted too.
float WORLDGENERAL_API get_max_elevation_in_sector(	const cPoint & start_point,
													float azimuth, //Azimuth from start_point
													float azimuth_sector, //Azimuth sector
													float azimuth_step, //Azimuth step to build profiles
													float hor_dist, //Horizontal length of the profiles
													unsigned long objFlags = 0); //Object viFlags

//Returns maximal angle of terrain elevation from start_point to end_point within the sector of azimuth_sector. Objects (objFlags) are accounted too.
float WORLDGENERAL_API get_max_elevation_in_sector(	const cPoint & start_point,
													const cPoint & end_point,
													float azimuth_sector,  //Azimuth sector
													float azimuth_step,//Azimuth step to build profiles
													unsigned long objFlags = 0); //Object viFlags
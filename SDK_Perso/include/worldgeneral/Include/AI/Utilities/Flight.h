#pragma once

#include "WorldGeneral.h"

#include "cLinear.h"
#include "cPosition.h"

#include "viFlags.h"

//Flight trajectory calculations

//Maneuvering

//Calculates "take position" (point + direction) maneuver parameters, turns directions are specified
void WORLDGENERAL_API calc_take_position_params(//in
												const cPosition & self_pos, //Initial position of the aircraft
												float turnRadius, //Horizontal turn radius
												const cPoint & point, //Final position
												cVector direction, //Final direction
												int first_turn_side, //Direction of first turn (1 - left, -1 - right)
												int second_turn_side, //Direction of second turn (1 - left, -1 - right)
												//out
												float & first_turn_arc, //Angular size of first turn
												float & med_turn_distance, //Distane between two turns
												float & second_turn_arc, //Angular size of second turn
												float & summary_distance //Summary distance
												);

//Calculates "take position" (point + direction) maneuver parameters
void WORLDGENERAL_API calc_take_position_params(//in
												const cPosition & self_pos,
												float turnRadius,
												const cPoint & point,
												cVector direction,
												//out
												float & first_turn_arc,
												float & med_turn_distance,
												float & second_turn_arc,
												float & summary_distance
												);

//Calculates trajectory length of "take position"  (point + direction) maneuver
inline float calc_take_position_traectory_length(	const cPosition & self_pos,
													float turnRadius,
													const cPoint & point,
													const cVector & direction)
{
	float first_turn_arc, med_turn_distance, second_turn_arc, summary_distance;
	calc_take_position_params(self_pos, turnRadius, point, direction, first_turn_arc, med_turn_distance, second_turn_arc, summary_distance);
	return summary_distance;
}

//Calculates "turn to circle tangent" maneuver parameters
bool WORLDGENERAL_API calc_dir_to_tangent_point(//in
												const cPoint & point, //Initial position of the aircraft
												const cPoint & center, //Center of the circle
												float radius, //Radius of the circle
												//out
												cPoint & dirToTangentPoint); //Result

//Calculates "turn to point" maneuver parameters. Turn direction is specidied.
bool WORLDGENERAL_API calc_turn_to_point_params(//in
												const cPosition & plane_pos, //Initial position of the aircraft
												float turnRadius, //Horizontal turn radius
												const cPoint & point, //The point
												short side,  //Turn direction (1 - left, -1 - right)
												//out
												float & azimuth_from_point, //Azimuth from the point to the final position of the aircraft
												float & turn_arc, //Angular size of the turn
												float & line_traectory_segment_length, //Length of linear segment of the trajectory
												float & traectory_length); //Length of the trajectory

//Calculates "turn to point" maneuver parameters
bool WORLDGENERAL_API calc_turn_to_point_params(//in
												const cPosition & plane_pos, //Initial position of the aircraft
												float turnRadius, //Horizontal turn radius
												const cPoint & point, //The point
												float distMin, //The minimal distance from the point to the final position of the aircraft
												bool enable_breakaway, //It is allowed to do breakaway before turn to point
												//out
												float & azimuth_from_point, //Center of sector from the point to the final position of the aircraft
												float & azimuth_from_point_sector, //Width of sector from the point to the final position of the aircraft
												float & turn_arc, //Angular size of the turn
												float & line_traectory_segment_length, //Length of linear segment of the trajectory
												float & traectory_length, //Length of the trajectory
												bool * breakaway_need = NULL); //Is breakaway needed

//Calculates length of "turn to point" maneuver trajectory
inline float get_turn_to_point_length(	const cPosition & plane_pos,
										float turnRadius,
										short side, const
										cPoint & point)
{
	float azimuth_from_point, azimuth_from_point_sector, turn_arc, line_traectory_segment_length, traectory_length;
	return calc_turn_to_point_params(	plane_pos, turnRadius, point, 0.0f, true,
										azimuth_from_point, azimuth_from_point_sector, turn_arc, line_traectory_segment_length, traectory_length) ? traectory_length : -1.0f;
}

//Checks if "turn to point" maneuver maneuver is possible in the given conditions
inline bool is_turn_to_point_possible(	const cPosition & plane_pos,
										float turnRadius,
										const cPoint & point,
										float distMin) //The minimal distance from the point to the final position of the aircraft
{
	float azimuth_from_point, azimuth_from_point_sector, turn_arc, line_traectory_segment_length, traectory_length;
	return calc_turn_to_point_params(	plane_pos, turnRadius, point, distMin, false,
										azimuth_from_point, azimuth_from_point_sector, turn_arc, line_traectory_segment_length, traectory_length);
}

//Returns azimuth from the point to the position of the aircraft after "turn to point" maneuver
inline void calc_approach_azimuth(	//in
									const cPosition & plane_pos,
									const cPoint & point,
									float distMin,
									bool enable_breakaway,
									float turnRadius,
									//out
									float & azimuth_from_point,
									float & azimuth_from_point_sector)
{
	float turn_arc, line_traectory_segment_length, traectory_length;
	calc_turn_to_point_params(plane_pos, turnRadius, point, distMin, enable_breakaway, azimuth_from_point, azimuth_from_point_sector, turn_arc, line_traectory_segment_length, traectory_length);
}

//Diving

//Calculates dive angle on the target for the aircraft if diving is started now
bool WORLDGENERAL_API get_dive_on_target_angle(	//in
												const cPoint & self_point, //Initial position of the aircraft
												float cur_climb_angle, //Current climb angle
												float verTurnRadius, //Vertical turn radius
												const cPoint & target_point,  //Target position
												//out
												float & dive_angle, //Final dive angle
												float * dive_start_distance = NULL); //Distance from the target on which dive angle is sustained

//Calculates dive start point on the target for the aircraft if diving is started now
bool WORLDGENERAL_API get_dive_on_target_start_point(	//in
														const cPoint & self_point,
														float cur_climb_angle,
														float verTurnRadius,
														const cPoint & target_point,
														//out
														cPoint & dive_on_target_start_point); //Position of the aircraft where dive angle is sustained

//Breakaway (terrain collision avoidance)

//Returns breakaway distance for the given attack
float WORLDGENERAL_API calc_breakaway_dist_simple(	float verTurnRadius, //Vertical turn radius
													const cPoint & target_point, //The target position
													float azimuth, //Center of the sector from the target to the aircraft
													float azimuth_sector, //Width of the sector from the target to the aircraft
													float elevation, //Trajectory elevation from the target
													bool horizontal, //Horizontal attack
													float breakaway_height = 50.0f); //Breakaway altitude (from the ground)

//Returns breakaway distance for the given dive attack
float WORLDGENERAL_API calc_breakaway_dist(	const cPosition & self_pos, //Initial position of the aircraft
											float verTurnRadius, //Vertical turn radius
											const cPoint & target_point, //The target position
											const cVector & dir_to_attack_start_point, //The direction to the attack start point from the target
											float breakaway_height = 50.0f); //Breakaway altitude (from the ground)

//Returns breakaway distance for the given dive attack
inline float calc_breakaway_dist(	const cPosition & self_pos, //Initial position of the aircraft
									float verTurnRadius, //Vertical turn radius
									const cPoint & target_point, //The target position
									float azimuth, //Azimuth from the target to the aircraft
									float elevation, //Trajectory elevation from the target
									float breakaway_height = 50.0f)//Breakaway altitude (from the ground)
{
	cVector dir_to_attack_start_point(cos(azimuth) * cos(elevation), sin(elevation), sin(azimuth) * cos(elevation));
	return calc_breakaway_dist(self_pos, verTurnRadius, dir_to_attack_start_point, target_point, breakaway_height);
}

//Returns max possible elevation of trajectory (from the target) that still makes breakaway distance not greater than specified maximal value
float WORLDGENERAL_API calc_max_elevation_by_breakaway_dist_max_simple(	float verTurnRadius,
																		const cPoint & target_point,
																		float azimuth,
																		float azimuth_sector,
																		float breakaway_dist_max, //Maximal distance of breakaway
																		float elevation_min_, //Minimal elevation of the trajectory
																		float elevation_max_, //Maximal elevation of the trajectory
																		float breakaway_height = 50.0f); //Breakaway altitude

//Returns breakaway distance for the given horizontal attack. Attack direction is specified by sector
inline float calc_breakaway_dist_simple_for_hor_attack(	const cPosition & self_pos,
														float altitude,
														float verTurnRadius,
														const cPoint & target_point,
														float azimuth,
														float azimuth_sector,
														float breakaway_height = 50.0f)
{
	cPoint point_above_target(target_point.x, altitude, target_point.z);
	cVector dir_to_attack_start_point(cos(azimuth), 0.0f, sin(azimuth));
	return calc_breakaway_dist_simple(verTurnRadius, point_above_target, azimuth, azimuth_sector, 0.0f, true, breakaway_height);
}

//Returns breakaway distance for the given horizontal attack
inline float calc_breakaway_dist_for_hor_attack(const cPosition & self_pos,
												float altitude,
												float verTurnRadius,
												const cPoint & target_point,
												float azimuth,
												float breakaway_height = 50.0f)
{
	cPoint point_above_target(target_point.x, altitude, target_point.z);
	cVector dir_to_attack_start_point(cos(azimuth), 0.0f, sin(azimuth));
	return calc_breakaway_dist(self_pos, verTurnRadius, point_above_target, dir_to_attack_start_point, breakaway_height);
}

//Ground target attack maneuvering

//Calculates dive attack parameters
bool get_dive_attack_params(//in
							const cPosition & self_pos,//Initial position of the aircraft
							float velocity, //Velocity of the aircraft
							float turnRadius, //Horizontal turn radius
							float verTurnRadius, //Vertical turn radius
							float VyMax, //Maximal climb rate
							const cPoint & target_point, //The target position
							bool LOS_need, //Specifies is LOS to the target required or not
							//out
							float & azimuth, //Center of the sector from the target to the aircraft when the aircraft started moving to the target
							float & azimuth_sector_width, //Width of the sector from the target to the aircraft when the aircraft started moving to the target
							float & elevation_min, //Minimal elevation from the target to the aircraft
							float & aproach_hor_dist_max, //Length of approach part of the trajectory (turn to point)
							float & approach_traectory_length); //Length of the entire trajectory

//Calculates horizontal attack parameters
bool get_hor_attack_params(	//in
							const cPosition & self_pos, //Initial position of the aircraft
							float velocity, //Velocity of the aircraft
							float turnRadius, //Horizontal turn radius
							float verTurnRadius, //Vertical turn radius
							const cPoint & target_point, //The target position
							bool LOS_need, //Specifies is LOS to the target required or not
							//out
							float & azimuth, //Center of the sector from the target to the aircraft when the aircraft started moving to the target
							float & azimuth_sector_width, //Width of the sector from the target to the aircraft when the aircraft started moving to the target
							float & elevation_min, //Minimal elevation from the target to the aircraft
							float & aproach_hor_dist_max, //Length of approach part of the trajectory (turn to point)
							float & approach_traectory_length); //Length of the entire trajectory

//Checks is it possible to do dive attack on the target
bool WORLDGENERAL_API check_dive_attack_possibility(const cPosition & self_pos, //Initial position of the aircraft
													float velocity, //Velocity of the aircraft
													float turnRadius, //Horizontal turn radius
													float verTurnRadius, //Vertical turn radius
													float VyMax, //Maximal climb rate
													const cPoint & target_point, //The target position
													bool LOS_need, //Specifies is LOS to the target required or not
													bool psi_req_present, //Is attack azimuth is required
													float psi_req, //Required attack azimuth
													float attack_elevation_req, //Required attack elevation
													float attack_elevation_max, //Maximal attack elevation
													float min_approach_hor_dist, //Minimal horizontal distance from the target to the aircraft position when the aircraft started moving to the target
													float min_attack_dist, //Minimal distance from the target to the aircraft position when the aircraft started diving on the target
													float attack_dist_reserve, //Reserve for min_attack_dist
													bool max_attack_distance_to_attack_dist_reserve_ratio,
													double delay, //Delay from current time to attack
													float breakaway_dist, //Breakaway distance (from the target)
													float breakaway_height); //Breakaway altitude (above the terrain)

//Checks is it possible to do horizontal attack on the target
bool WORLDGENERAL_API check_hor_attack_possibility(	const cPosition & self_pos, //Initial position of the aircraft
													float velocity, //Velocity of the aircraft
													float turnRadius, //Horizontal turn radius
													float verTurnRadius, //Vertical turn radius
													const cPoint & target_point, //The target position
													bool LOS_need, //Specifies is LOS to the target required or not
													bool psi_req_present, //Is attack azimuth is required
													float psi_req, //Required attack azimuth
													float attack_elevation_max, //Maximal attack elevation
													float min_approach_hor_dist, //Minimal horizontal distance from the target to the aircraft position when the aircraft started moving to the target
													float attack_dist_reserve, //Reserve for min_attack_dist
													bool max_attack_distance_to_attack_dist_reserve_ratio,
													double delay, //Delay from current time to attack
													float breakaway_dist); //Breakaway distance (from the target)

//Interception maneuvering

//Calculates interception vector
bool WORLDGENERAL_API getInterceptionVector(//in
											const cVector & dirToTarget, //Direction to the target, normalized vector
											float distToTarget, //Distance to the target
											float interceptorVel, //Velocity of the interceptor
											const cVector & targetVel, //Velocity of the target
											//out
											cVector & result); //Interception vector

//Calculates maximal possible distance to the target to intercept it
float WORLDGENERAL_API getInterceptionDistMax(	float cosTargetAspect, //Cosine of the target's aspect
												float interceptorMaxFlightDist, //Maximal flight distance of the interceptor
												float targetToInterceptorVelRatio ); //Target velocity and interceptor velocity ratio

//Calculates interception parameters
bool WORLDGENERAL_API getInterceptionParams(//in
											float cosTargetAspect, //Cosine of the target's aspect
											float distToTarget, //Distance to the target
											float targetToInterceptorVelRatio, //Target velocity and interceptor velocity ratio
											//out
											float & interceptionAngle, //The angle between the direction on target and the direction of interception
											float & interceptorDistToHit); //The length of interceptor path to hit the target

//Collision avoidance

//To determine what maneuvers aircraft should perform to avoid collision with the ground we need to analyze terrain surface profile along the flight path.
//Pull-up trajectory consists of 3 phases:
//1. Linear segment: roll minimization, reaching maximal allowed Ny
//2. Vertical turn with maximal allowed Ny
//3. Linear segment. Here the aircraft must be able to have sustained climb and don't stall.

//Each time the surface profile must be below pull-up trajectory.
//To perform evasive maneuvers ground proximity factor is calculated
//Ground proximity factor is a difference between current climb angle and the minimal safe climb angle.
//The segment of the surface profiles where the ground proximity factor is worst (minimal) is the critical segment.


//*                                       *
//  *    PULL-UP TRAJECTORY           *
//    *                           *
//      *                     *
//	      *               *
//		     *        *        TERRAIN
//         ==    *             ===============
//==     ==  ==             ===               ==
//  =====      =============                   ==
//|  1   |       2         |      3        |
//|inert | vertical G-turn |sustained climb|

//Parameters of the surface profiles calculation procedure
struct PointsSearch
{
	float		altitudeMin;
	viFlag		objMask;
	size_t		pointsCountMax;
};

//Parameters of the surface profile calculation procedure
struct PointsSearchPosition
{
	cPosition	posVel; //Velocity position of the start point
	float		distance0; //Distance from posVel.p along posVel.x for linear part of the vertical maneuver trajectory (Pull-up trajectory, phase 1)
	float		distance; //Distance from posVel.p along posVel.x for all the vertical maneuver trajectory (Pull-up trajectory, phase 1-3)
};

//Profiles are usually calculated for several directions.
//PointsSearch is a parameter for all the directions.
//PointsSearchPosition is a parameter for single direction.

//Surface profiles
struct Points
{
	E2::Vector *	buff; //Points buffer
	size_t *		count0; //Quantity of point within the 1st phase of the pull-up trajectory for each profile
	size_t *		count; //Quantity of points total for each profile
	float *			tanClimb0; //
	unsigned int	positionCount; //Profile count
	size_t			pointsCountMax; //Buffer size
};

//Build size surface profiles for size positions
void WORLDGENERAL_API buildPointsSector(//in
										const PointsSearch & pointsSearch, //Profiles search parameters
										const PointsSearchPosition * positions, unsigned int size, //Profiles search directions
										//out
										Points & points); //Result

template<unsigned int size>
inline void buildPointsSector(	//in
								const PointsSearch & pointsSearch,
								const PointsSearchPosition (&position)[size],
								//out
								Points & points)
{
	buildPointsSector(pointsSearch, position, size, points);
}

//Parameters of collision test
struct PointsTest
{
	enum Flag
	{
		FLAG_NULL = 0,
		FLAG_TAN_CLIMB_MAX = 1 << 0, //Account 3rd phase of the pull-up trajectory
		FLAG_DIST_FACTOR = 1 << 1, //Distance factor: additional coefficient to ground proximity factor that is depended on distance from the aircraft to the point
	};
	unsigned int	flags;
	float			RverMax; //Vertical turn radius
	float			tanClimbMax; //Tangent of maximal sustained climb angle
	float			vel; //Velocity
	float			distFactor; //Distance factor (has effect only if FLAG_DIST_FACTOR is raised)
};

//Segment of the surface profile
struct PointSegment
{
	E2::Vector	point; //Start point
	float		tanSlope; //Tangent of the segment elevation
	float		sizeX; //Horizontal length of the segment
};

//Converts point3D into posVel reference frame in vertical plane
inline E2::Vector toPoint2D(const cPosition & posVel, const cPoint & point3D)
{
	const cVector dirToPoint = point3D - posVel.p;
	return E2::Vector(dirToPoint * posVel.x, dirToPoint * posVel.y);
}

//Calculation of ground proximity factor.

//Calculates ground proximity factor for the given velocity position and the segment of the surface profile
float WORLDGENERAL_API calcGroundProximityFactor(const cPosition & posVel, const PointsTest & pointsTest, const PointSegment & segment);

//Calculates ground proximity factor for the given profiles returns critical segment of the surface profile.
float WORLDGENERAL_API calcGroundProximityFactor(	//in
													const Points & points, //The surface profiles
													const PointsTest & pointsTest, //Surface profiles search parameters
													//out
													PointSegment & segment); //Critical segment of the profile

//Builds surface profiles, calculates ground proximity factor for the given velocity positions and returns critical segment of the surface profile.
float WORLDGENERAL_API getGroundProximityFactor(//in
												const PointsSearch & pointsSearch,
												const PointsSearchPosition * positions, unsigned int size, //Surface profile search positions
												const PointsTest & pointsTest, //Surface profiles search parameters
												//out
												PointSegment & segment); //Critical segment of the profile
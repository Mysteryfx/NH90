#pragma once

#include "WorldGeneral.h"
#include <ed/string.h>

#include "cLinear.h"
#include "cAlgorithm.h"

#include <functional>

struct cstr_less : public std::binary_function<const char*, const char*, bool>	
{
	typedef const char* Type;
	bool operator()(const Type & _Left, const Type & _Right) const
	{
		return strcmp(_Left, _Right) < 0;
	}
};

//Inserts allign spaces (' ') after each '\n' in the str
void WORLDGENERAL_API shift(ed::string & str, int allign);

//Returns sector center and width the for pair of angles and the angle between them
void WORLDGENERAL_API get_azimuth_path(	//in
										float arc_angle1,
										float arc_angle2,
										float mid_arc_angle,
										//out
										float & arc_center,
										float & arc_width);

//Geometry

//Returns bisector of the pair of vectors
template <typename V>
inline V get_bisector(const V & v1, const V & v2)
{
	return 0.5f * (v1.ort() + v2.ort());
}

//Returns the line parameter at the point where perpendicular from the point intersects the line
template <typename V>
inline float getDistToIntersectionPoint(const V & point, const V & line_start, const V & line_dir, float * h)
{
	V line_start_to_point_dir = point - line_start;		
	float l = line_dir.ort() * line_start_to_point_dir;
	if(h != NULL)
		(*h) = sqrt(line_start_to_point_dir.length() * line_start_to_point_dir.length() - l * l);
	return l;
}

//Returns distance from the point to the line (line_start, line_end)
template <typename V>
inline float get_dist_from_point_to_line(	//in
											const V & point,
											const V & line_start,
											const V & line_end,
											bool limited_length, //The line of limited length
											//out
											int & edge_num) //When the perpendicular from the point to the line touches the line (has effect only if limited_length is true): 
															//-1 - before the line start,
															//0 - on the line
															//1 - after the line finish
{
	V line_dir = line_end - line_start;
	V line_start_to_point_dir = point - line_start;

	float line_length = line_dir.length();
	float line_start_to_point_dist = line_start_to_point_dir.length();
	float l = line_dir / line_length * line_start_to_point_dir;
	if(limited_length)
	{
		if(l <= 0.0f)
		{
			edge_num = -1;
			return line_start_to_point_dist;
		}
		else if(l >= line_length)
		{
			edge_num = 1;
			return (point - line_end).length();
		}
	}
	edge_num = 0;
	float h = line_start_to_point_dist > l ? sqrt(line_start_to_point_dist * line_start_to_point_dist - l * l) : 0.0f;
	return h;
}

//Checks if the both points are on the same side of the line (line_start, line_start + line_dir)
inline bool is_points_in_one_side_of_line(const E2::Vector & point1, const E2::Vector & point2, const E2::Vector & line_start, const E2::Vector & line_dir)
{
	const E2::Vector line_dir_Pi_2(line_dir.y, -line_dir.x);
	return SIGN(line_dir_Pi_2 * (point1 - line_start)) == SIGN(line_dir_Pi_2 * (point2 - line_start));
}

//Checks intersection between the line and the circle
bool WORLDGENERAL_API getLineCircleIntersections(	const E2::Vector & line_start,
													const E2::Vector & line_dir,
													const E2::Vector & circle_center,
													float circle_radius,
													float & dist_to_in,
													float & dist_to_out);

//Checks intersection between the line and the cylinder
bool WORLDGENERAL_API getLineCylinderIntersections(	const cVector & line_start, const cVector & line_dir, 
													const cVector & center, float radius, float height, 
													float min_h, float max_h,
													float & dist_to_in, float & dist_to_out);



#ifndef _CCCLIPPING_H
#define _CCCLIPPING_H

#include "Utilities\ccStraightLine.h"
#include <ed/vector.h>
#include <ed/list.h>
#include <ed/map.h>

namespace cockpit
{

typedef ed::vector<Vector3>        vectorarray;
typedef ed::vector<unsigned short> indexarray;
typedef ed::list<vectorarray>		t_poly_list;

t_poly_list create_convex_poly_list(vectorarray&, indexarray&);
bool is_convex_poly(ed::map<unsigned short, Vector3>&);

inline int DivideTriMesh(vectorarray& vertices_out,
						 vectorarray& vertices_in,
						 StraightLine* line)
{
	int changed_out = 0;

	if (vertices_in.size() > 0)
	{
		vectorarray new_vertices_out;
		vectorarray new_vertices_in;

		Vector3 intersection;
		size_t size_in = vertices_in.size();
		double side1, side2;
		
		side2 = line->point_side(vertices_in[0]);

		for (size_t i = 0; i < size_in; i++)
		{
			side1 = side2;
			side2 = line->point_side(vertices_in[(i+1) % size_in]);

			if ((side1 > 0.0 && side2 < 0.0) || (side1 < 0.0 && side2 > 0.0))
			{
				changed_out++;

				if (side1 < 0.0)
					new_vertices_in.push_back(vertices_in[i]);
				else
					new_vertices_out.push_back(vertices_in[i]);				
				
				intersection = line->intersection(StraightLine(vertices_in[i], vertices_in[(i+1) % size_in]));
				new_vertices_in.push_back(intersection);
				new_vertices_out.push_back(intersection);
			}
			else
			{
				if (side1 < 0.0 && side2 < 0.0)
				{
					new_vertices_in.push_back(vertices_in[i]);
					changed_out++;
				}	
				else
					new_vertices_out.push_back(vertices_in[i]);
			}
		}

		vertices_out = new_vertices_out;
		vertices_in = new_vertices_in;
	}

	return changed_out;
}
inline bool point_inside_convex_poly(const Vector3 & pnt,const vectorarray & poly)
{
	for (int i = 0; i < poly.size(); ++i)
	{
		const Vector3 & start = poly[i];
		const Vector3 & end	  = (i == poly.size() - 1) ? poly[0] : poly[i + 1];
		if(StraightLine(start,end).point_side(pnt) < 0)
		{
			return false;
		}
	}
	return true;
}
inline bool point_outside_convex_poly(const Vector3 & pnt,const vectorarray & poly)
{
	for (int i = 0; i < poly.size(); ++i)
	{
		const Vector3 & start = poly[i];
		const Vector3 & end	  = (i == poly.size() - 1) ? poly[0] : poly[i + 1];
		if(StraightLine(start,end).point_side(pnt) > 0)
		{
			return false;
		}
	}
	return true;
}


/*
inline Vector3 find_vec_inside(vectorarray& convex_poly)
{
	Vector3 center(0, 0, 0);
	
	if (convex_poly.size() < 3)
		return center;

	float y_mean_1 = (convex_poly[1].y + convex_poly[2].y) / 2.0f;
	float z_mean_1 = (convex_poly[1].z + convex_poly[2].z) / 2.0f;
	
	center.x = 0.0f;
	center.y = (convex_poly[0].y + y_mean_1) / 2.0f;
	center.z = (convex_poly[0].z + z_mean_1) / 2.0f;

	return center;
};
*/

template<class T> inline Vector3 find_vec_inside(T& convex_poly)
{
	Vector3 center(0, 0, 0);
	
	if (convex_poly.size() < 3)
		return center;

	float y_mean_1 = (convex_poly[1].y + convex_poly[2].y) / 2.0f;
	float z_mean_1 = (convex_poly[1].z + convex_poly[2].z) / 2.0f;
	
	center.x = 0.0f;
	center.y = (convex_poly[0].y + y_mean_1) / 2.0f;
	center.z = (convex_poly[0].z + z_mean_1) / 2.0f;

	return center;
}

inline Vector3 find_vec_inside(ed::map<unsigned short, Vector3>& convex_poly)
{
	Vector3 center(0, 0, 0);
	
	if (convex_poly.size() < 3)
		return center;

	ed::map<unsigned short, Vector3>::iterator it;
	it = convex_poly.begin();

	Vector3& poly0 = it->second;
	it++;
	Vector3& poly1 = it->second;
	it++;
	Vector3& poly2 = it->second;

	float y_mean_1 = (poly1.y + poly2.y) / 2.0f;
	float z_mean_1 = (poly1.z + poly2.z) / 2.0f;
	
	center.x = 0.0f;
	center.y = (poly0.y + y_mean_1) / 2.0f;
	center.z = (poly0.z + z_mean_1) / 2.0f;

	return center;
};

}

#endif // _CCCLIPPING_H

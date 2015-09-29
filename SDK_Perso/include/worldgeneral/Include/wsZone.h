#pragma once
#ifndef __wsZone_h__
#define __wsZone_h__

#include <ed/list.h>
#include <ed/unordered_map.h>

#include "WorldGeneral.h"
#include "Serializer\Serializer.h"

#include "Box.h"

#include "cPosition.h"
#include "Math\Box.h"
#include "wMovingObject.h"

typedef ed::list<E2::Vector> wsIntersections;


enum wsAreaType {TYPE_NONE = -1, TYPE_CIRCLE, TYPE_RECTANGLE, TYPE_POLYGON};

class WORLDGENERAL_API wsArea
{
public:
	virtual void resize(float resize_coeff) = 0;
	//get
	virtual wsAreaType get_type() const = 0;
	//func
	virtual bool	is_inside(const E2::Vector & point) const = 0;
	//virtual E2::Vector	get_nearest_point() const = 0; //ближайшая к данной точке точка фигуры
	virtual int		get_intersections(const E2::Vector & start_point, const E2::Vector & end_point, wsIntersections & intersections) const = 0; //пересечения отрезка с фигурой
	virtual bool	get_nearest_intersection(const E2::Vector & start_point, const E2::Vector & end_point, E2::Vector & intersection) const; //ближайшая точка пересечения отрезка с фигурой
	//gabarites
	virtual E2::Vector		get_center_point() const = 0;
	virtual Math::Box2f		get_bounding_box() const = 0;
	virtual float			get_outer_radius() const = 0;
};

class WORLDGENERAL_API wsCircleArea : public wsArea
{
public:
	//inits
	wsCircleArea() {radius = 0.0f;}

	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wsCircleArea & data);
	void init(const E2::Vector & pos_, float radius_);
	void resize(float resize_coeff);
	//func
	//virtual E2::Vector	get_nearest_point() const;
	virtual bool	is_inside(const E2::Vector & point) const;	
	virtual int		get_intersections(const E2::Vector & start_point, const E2::Vector & end_point, wsIntersections & intersections) const;
	//get
	virtual wsAreaType get_type() const {return TYPE_CIRCLE;}
	void			get_params(E2::Vector & pos_, float	& radius_) {pos_ = pos; radius_ = radius;}
	//gabarites
	virtual E2::Vector		get_center_point() const {return pos;}
	virtual Math::Box2f		get_bounding_box() const;
	virtual float			get_outer_radius() const {return radius;}
private:
	E2::Vector	pos;
	float	radius;
};


class WORLDGENERAL_API wsRectangleArea  : public wsArea
{
public:
	wsRectangleArea() : rot_angle(0) {;}
	void init(const E2::Vector & left_upper, const E2::Vector & right_lower);
	void init(const E2::Vector & pos_, float length_, float width_, float rot_angle_ = 0.0f);
	void resize(float resize_coeff);
	//func
	//virtual E2::Vector	get_nearest_point() const;
	virtual bool	is_inside(const E2::Vector & point) const;
	virtual int		get_intersections(const E2::Vector & start_point, const E2::Vector & end_point, wsIntersections & intersections) const;
	//get
	virtual wsAreaType	get_type() const;
	const Math::Box2f & get_box() const {return box;}
	float				get_angle() const {return rot_angle;}
	//gabarites
	virtual E2::Vector		get_center_point() const;
	virtual Math::Box2f		get_bounding_box() const;
	virtual float			get_outer_radius() const;
private:
	void					build_bounding_box();
	Math::Box2f box;
	float rot_angle;
	Math::Box2f				bounding_box;
};



typedef ed::list<E2::Vector> PolygonVerts;
struct lua_State;

class WORLDGENERAL_API wsPolygonArea  : public wsArea
{
public:
	void init(const PolygonVerts & verts_);
	void resize(float resize_coeff);
	//func
	//virtual E2::Vector	get_nearest_point() const;
	virtual bool	is_inside(const E2::Vector & point) const;
	virtual int		get_intersections(const E2::Vector & start_point, const E2::Vector & end_point, wsIntersections & intersections) const;
	//get
	virtual wsAreaType get_type() const {return TYPE_POLYGON;}
	const PolygonVerts & get_verts() const {return verts;};
	//gabarites
	virtual E2::Vector		get_center_point() const;
	virtual Math::Box2f		get_bounding_box() const;
	virtual float			get_outer_radius() const;
private:
	PolygonVerts			verts;
	Math::Box2f				bounding_box;
};

class WORLDGENERAL_API wsZone
{
public:
	wsZone() : area(NULL), linked_obj_ptr(NULL) {;}
	wsZone(const wsZone & zone) {area = NULL; linked_obj_ptr = NULL; (*this) = zone;}
	~wsZone() {clear();}
	void load_from_state(lua_State * config_);
	void clear();
	void clear_geometry();
	wsZone & operator =(const wsZone & zone);
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wsZone & data);
	//set
	void set_linked_obj_ptr(woPointer linked_obj_ptr_) {linked_obj_ptr = linked_obj_ptr_;}
	void init_circle(const E2::Vector & pos_, float radius_);
	void init_rectangle(const E2::Vector & left_upper_point, const E2::Vector & right_lower_point);
	void init_rectangle(const E2::Vector & pos_, int length_, int width_, float rot_angle_ = 0.0f);
	void init_polygon(const PolygonVerts & verts);
	void set_name(const ed::string & name_) { name = name_; }
	//get
	woPointer get_linked_obj_ptr() const {return linked_obj_ptr;}
	const ed::string & get_name() const { return name; }
    ObjectID            get_missID() const { return missID; }
	//func
	virtual bool check_object_pos(woPointer obj_ptr, woPointer zoneobj_ptr = 0) const;
	virtual bool check_object_pos(const cPoint & obj_pos, woPointer zoneobj_ptr = 0) const;
	//get geometry data
	wsAreaType	get_geometry_type() const;
	cPoint		get_center_point() const;
	::Box	get_bounding_box() const;
	float		get_outer_radius() const;
	const wsArea * get_area() const {return area;}
private:
	ed::string name;
    ObjectID    missID;
	wsArea * area;
	woPointer linked_obj_ptr;
};

typedef ed::unordered_map<ed::string, wsZone*> wsZonesMap;
typedef ed::map<int, wsZone*> wsZonesMapByID;

class WORLDGENERAL_API wsZonesStorage
{
public:
	wsZonesStorage() {clear();}
	void		clear();
	void		register_zone(ObjectID zoneMissID, wsZone * zone);
    bool		remove_zone(ObjectID zoneMissID);
    wsZone *	get_zone(const ed::string& missName);
    wsZone *	get_zone(ObjectID zoneMissID);
	const wsZonesMap & get_zones() const {return zones_map;}
    const wsZonesMap & get_zones_by_id() const {return zones_map;}
private:
	wsZonesMap	zones_map;
    wsZonesMapByID zones_map_by_id;
};

#endif
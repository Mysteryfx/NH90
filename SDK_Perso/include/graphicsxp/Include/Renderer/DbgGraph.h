#ifndef GRAPHICSXP_DBG_HELPER_H
#define GRAPHICSXP_DBG_HELPER_H

#include "Position.h"
#include "Box.h"
#include "cLinear.h"
#include "Renderer/RenderObject.h"

#include <ed/vector.h>

namespace Graphics
{
    class RenderParser;
};

enum enum_obj_geometry_type{
	dbg_obj_none,
	dbg_obj_line,
	dbg_obj_box,
	dbg_obj_sphere,
	dbg_obj_cone,
	dbg_obj_circle,
	dbg_obj_arc,
	dbg_obj_cylinder,
	dbg_obj_limited_line,
	dbg_obj_polyline,
	dbg_obj_cross,
	dbg_obj_vector_field,
	dbg_obj_frustum
};

class vi_helper;

class dbg_obj
{
    friend class vi_helper;
public:
    GRAPHICSXP_API dbg_obj();
    GRAPHICSXP_API virtual ~dbg_obj();
	dbg_obj(const dbg_obj & dbg_obj_) : dbg_robj(0), initialized(false),have_point(false) {*this = dbg_obj_;}
    GRAPHICSXP_API virtual void        parse(Graphics::RenderParser & parser, const Position3 & pos);
	GRAPHICSXP_API virtual void		parse(Graphics::RenderParser & parser, const Vector3 & camera_pos); //если dbg_pos задан в мировых координатах
	GRAPHICSXP_API void		draw();

    Position3 & get_pos()                        {  return dbg_pos; }
	void		set_point(const dVector & pnt_) { pnt = pnt_; dbg_pos.p = toVec3(pnt);  have_point = true; }
    GRAPHICSXP_API void set_color(const Vector3 & col);
	GRAPHICSXP_API dbg_obj & operator =(const dbg_obj & dbg_obj_);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_none;}
	GRAPHICSXP_API void build(const ed::vector<Vector3> & verts, const ed::vector<unsigned short> & inds, const Vector3 & color);

    GRAPHICSXP_API virtual void  initialize();
protected:
	void deinitialize();
    bool  initialized;
    Graphics::RenderObject   dbg_robj;
	Position3				 renderObjectPos;
    Position3                dbg_pos;
	dVector					 pnt;
    Vector3                  color;
private:
	bool	have_point;
};


class dbg_line : public dbg_obj
{
public:
    dbg_line(bool create_coord = false, float length_ = 1.0f):dbg_obj(),coord(create_coord), length(length_) {}
    virtual ~dbg_line(){}
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_line;}
	GRAPHICSXP_API virtual void initialize();
protected:
    bool  coord;
	float length;
};


class dbg_box : public dbg_obj
{
public:
    GRAPHICSXP_API dbg_box(float size = 1.0f);
    virtual  ~dbg_box() {};
	void  set_box(const ::Box & b) { box = b; reset_box();}
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_box;}
	GRAPHICSXP_API virtual void initialize();
protected:
    GRAPHICSXP_API void  reset_box();
    ::Box   box;
};


class dbg_cone : public dbg_obj
{
public:
    dbg_cone(double h = 1.0,double a = 3.14 / 2.0):height(h),angle(a) {}
    virtual  ~dbg_cone() {}
    void  set_cone(double h,double a) {height = h; angle = a; reset_cone();}
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_cone;}
	GRAPHICSXP_API virtual void initialize();
protected:
    GRAPHICSXP_API void  reset_cone();
    double height;
    double angle;
};


class dbg_frustum : public dbg_obj
{
public:
	GRAPHICSXP_API dbg_frustum(double h = 1.0,double horizontal_span = 3.14/2.0,double vertical_span = 3.14/2.0);
	virtual  ~dbg_frustum() {};
	GRAPHICSXP_API void  set_frustum(double h,double horizontal_span,double vertical_span);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_frustum;}
	GRAPHICSXP_API virtual void initialize();
protected:
	void  reset_frustum();
	double height;
	double tan_horizontal;
	double tan_vertical;
};


// Цилиндр в направлении оси OX, длиной L (от 0 до L), радиусом R
class dbg_cylinder : public dbg_obj
{
public:
	GRAPHICSXP_API dbg_cylinder(double l, double r, int s);
	virtual ~dbg_cylinder() {};
	GRAPHICSXP_API void  set_cylinder(double l, double r, int s);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_cylinder;}
	GRAPHICSXP_API virtual void initialize();
protected:
    virtual void  reset_cylinder();
protected:
	double length_, radius_;
	int sectors_;
};

class dbg_poly_line : public dbg_obj
{
public:
	dbg_poly_line() {}
    ~dbg_poly_line() {}
	GRAPHICSXP_API void  set_poly_line(const Vector3 & start_point, const Vector3 & start_end);
	GRAPHICSXP_API void  set_poly_line(const ed::vector<Vector3> & verts_);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_polyline;}
	GRAPHICSXP_API void  initialize();
protected:
    void  reset_poly_line();
	ed::vector<Vector3> verts_buf;
};

class dbg_limited_line : public dbg_obj
{
public:
	dbg_limited_line() : dbg_obj(), length(0.0f) {}
	dbg_limited_line(double length_): dbg_obj() , length(length_) {}
   ~dbg_limited_line() {}
   void set_length(double length_) {length = length_; reset_line();}
   GRAPHICSXP_API dbg_limited_line & operator =(const dbg_limited_line & dbg_limited_line_);
   virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_limited_line;}
   GRAPHICSXP_API void  initialize();
protected:
	GRAPHICSXP_API void  reset_line();
    double  length;
};

class dbg_circle : public dbg_obj
{
	friend class dbg_sphere;
public:
	enum AxisEnum { AXIS_X, AXIS_Y, AXIS_Z};
	dbg_circle() : dbg_obj(), radius(0), side_max_length(0), axis(AXIS_Y) {}
    dbg_circle(double r, double sml):radius(r), side_max_length(sml), axis(AXIS_Y) {}
	dbg_circle(double r, int sides_qty):radius(r), side_max_length(2 * 3.14159 * r / float(sides_qty)), axis(AXIS_Y) {}
    ~dbg_circle() {}
	double getRadius() const {return radius;}
	void  set_axis(AxisEnum _axis) {axis = _axis;}
    void  set_circle(double r, double sml) {radius = r; side_max_length = sml; reset_circle();}
	GRAPHICSXP_API void  set_circle(double r, int sides_qty);
	GRAPHICSXP_API dbg_circle & operator =(const dbg_circle & dbg_circle_);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_circle;}
	GRAPHICSXP_API void  initialize();
protected:
	AxisEnum axis;
    GRAPHICSXP_API void  reset_circle();
    double radius;
    double side_max_length;
};

class dbg_sphere : public dbg_obj
{
public:
	dbg_sphere() : dbg_obj(), radius(0) {}
	~dbg_sphere() {}
	double getRadius() const {return radius;}
	GRAPHICSXP_API void  set_sphere(double r, int sides_qty);
	GRAPHICSXP_API dbg_sphere & operator =(const dbg_sphere & dbg_sphere_);
	GRAPHICSXP_API virtual void        parse(Graphics::RenderParser & parser, const Position3 & pos);
	GRAPHICSXP_API virtual void		parse(Graphics::RenderParser & parser, const Vector3 & camera_pos); //если dbg_pos задан в мировых координатах
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_sphere;}
	GRAPHICSXP_API void  initialize();
protected:
	GRAPHICSXP_API void  reset_sphere();

	dbg_circle circles[3];
	double radius;
};

class dbg_arc : public dbg_circle
{
public:
	dbg_arc() : dbg_circle(), min_angle(0.0), max_angle(6.28318) {}
    dbg_arc(double r, double sml):radius(r), side_max_length(sml) {}
	dbg_arc(double r, int sides_qty):radius(r), side_max_length(2 * 3.14159 * r / float(sides_qty)) {}
    ~dbg_arc() {};
    GRAPHICSXP_API void set_arc(double r, double min_a, double max_a, double sml);
	GRAPHICSXP_API void set_arc(double r, double min_a, double max_a, int sides_qty);
	GRAPHICSXP_API dbg_arc & operator =(const dbg_arc & dbg_arc_);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_arc;}
    GRAPHICSXP_API void  initialize();
protected:
    void  reset_arc();
    double radius;
	double min_angle;
	double max_angle;
    double side_max_length;
};

class dbg_cross : public dbg_obj
{
public:
	dbg_cross() : dbg_obj(), min_radius(0), max_radius(0) {}
    dbg_cross(double max_radius_, double min_radius_): max_radius(max_radius_), min_radius(min_radius_) {}
    ~dbg_cross() {}
    void  set_circle(double max_radius_, double min_radius_) {max_radius = max_radius_; min_radius = min_radius_; reset_cross();}
	GRAPHICSXP_API dbg_cross & operator =(const dbg_cross & dbg_cross_);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_cross;}
    GRAPHICSXP_API void  initialize();
protected:
    void  reset_cross();
    double max_radius;
    double min_radius;
};

class dbg_vector3_field : public dbg_obj
{
public:
	typedef ed::vector<Vector3> Vector3Vector;
	typedef ed::vector<Vector3Vector> Vector3Matrix2D;
	typedef ed::vector<Vector3Matrix2D> Vector3Matrix3D;
	dbg_vector3_field() {}
	GRAPHICSXP_API void set_vector_field(const Vector3 & center, const Vector3Matrix3D & vector3_matrix3D_, const Vector3 & step_);
	GRAPHICSXP_API dbg_vector3_field & operator =(const dbg_vector3_field & dbg_cross_);
	virtual enum_obj_geometry_type get_geometry_type() {return dbg_obj_vector_field;}
	GRAPHICSXP_API void  initialize();
protected:
	void  reset_vector_field();
	Vector3 center;
	Vector3Matrix3D vector3_matrix3D;
	Vector3 step;
};
#endif

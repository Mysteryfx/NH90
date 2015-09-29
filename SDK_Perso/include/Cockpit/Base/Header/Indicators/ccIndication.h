#ifndef ccIndication_H
#define ccIndication_H

#include "Renderer/RenderObject.h"
#include "Renderer/CustomGeometry.h"
#include "Indicators/ccElement.h"
#include "Indicators/ccIndicatorPageImpl.h"
#include "Utilities/ccUtilities.h"
#include "Renderer/Camera.h"

// Конкретная реализация элементов индикации 

namespace cockpit
{

typedef ed::vector<ed::string> cc_strID_array;

typedef enum cc_h_clip_relations
{
	H_CLIP_NULL,
	H_CLIP_COMPARE,
	H_CLIP_REWRITE_LEVEL,
	H_CLIP_INCREASE_LEVEL,
	H_CLIP_INCREASE_IF_LEVEL,
	H_CLIP_DECREASE_LEVEL,
	H_CLIP_DECREASE_IF_LEVEL
};

typedef enum cc_h_clip_compare_types
{
	H_CLIP_GREATEREQUAL = 10,
	H_CLIP_INVERT
};

class COCKPITBASE_API ceSimple : public Element
{
public:
	ceSimple();

	virtual ~ceSimple();


	virtual int  getValue(lua_State*, const ed::string&) const;
	virtual void setValue(lua_State*, const ed::string&);
	virtual void setValue(const char*){};
	
	virtual void draw(Graphics::RenderParser& parser, unsigned char level);

	void update();
	virtual void precreate() {Precreated = true;}
	const Position3& get_position() const {return pos;}
	Position3& get_position_ref() {return pos;}

	void set_position(const Position3& p) {pos = p;}
	void             setparent(ccIndicatorPage* ppage) {parent = ppage;}
	ccIndicatorPage* getparent() {return (root == 0)? parent : root->getparent();}

	virtual void set_material(const ed::string& mat_name_in);
	
	virtual void rescale();
    const ed::string & getName() const {return name;}
	bool is_collimated() const {return collimated;}
		
	virtual bool get_isDraw() const {return isDraw;}
	virtual void set_isDraw(bool flag) {isDraw = flag;}
	virtual void apply() {}
	
	DParamList& get_params_ref()    {return element_params;};
	const DParamList& get_params() const
	{
		if (root &&  params_used_from_root)
			return root->get_params();
		else
			return element_params;
	}

	void setparent_element(Element* elmt);
	void setchild_element(Element* elmt);
	const ed::string& get_rootName() const {return root_name;}
	Element* get_root() {return root;}
	ccElementsArray& get_children_ref() {return children;}
	bool get_use_root_params() {return params_used_from_root;}

	void set_geometry_host(Element* elmt);
	void set_geometry_child(Element* elmt);
	const ccUniqueElementsMap& get_geometry_hosts() {return geometry_hosts;}
	ccElementsArray& get_geometry_children_ref() {return geometry_children;}
	void changeGeometryChildrenStat();
	void setToCheckGeometry(bool flag) {ToCheckGeometry = flag;}

	void set_use_temporal_set(bool flag_in,unsigned used_set) {temporal_set_used = flag_in; temporal_set = used_set;}
	void set_use_root_params (bool flag_in) 
	{
		params_used_from_root = flag_in;
		for (int i = 0; i < children.size();++i)
			 children[i]->set_use_root_params(true);//насильно заставляем использовать параметры родителя
	}

	void set_use_forced_color(bool flag_in) {use_forced_color = flag_in;}
	void set_forced_color(float R,float G,float B)
	{
		forced_color.r = R;
		forced_color.g = G;
		forced_color.b = B;
	}
    void set_forced_color(const Vector3 & col)  { set_forced_color(col.x,col.y,col.z); } ;


    void set_use_contrast(bool flag_in) { use_contrast = true; };
    void set_contrast(float power) { contrast_power = power;}; 


	void set_break_update_cycle(bool flag_in) { break_update_cycle = flag_in;};

	// Функция перегружает virtual const ed::string& class_name() const = 0,
	// поэтому inline будет проигнорировано при компиляции
	//
	//inline const ed::string & class_name() const 
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceSimple";
		return class_name_;
	}

	Element* copy();
	unsigned char  get_draw_level() const {return screenspace;}
	const Vector3& get_init_pos() {return init_pos;}


    static float     get_screenspace_plane(const Graphics::Camera & cam);
    static Position3 get_camera_orient(const Graphics::Camera & cam,bool simmetric_scale = false);

    void             set_custom_mfd_material_params(bool colored,bool inverted,float R,float G,float B);

    void             log(ed::string & out,bool full);

	float			 get_z_enable_mat_param();
protected:
    virtual void     log_self(ed::string & out);;
	virtual void OnFirstTime();
	void		 OnExternalUPDATE(); // обработчик внешнего управления 
	void         set_mat_params(const Graphics::RenderParser& parser,Graphics::RenderObject* R);
	virtual void set_position_to_render();
	virtual void self_render(Graphics::RenderParser& parser) {}

    Position3    get_orient();

	inline float param_additive_alpha()
	{
		if (use_forced_color && 
			forced_color.r < 0.01 &&
			forced_color.g < 0.01 && 
			forced_color.b < 0.01)
		{
			//complete black doesn't work with additive alpha
			return 0.0f;
		}
		return additive_alpha ? 1.0f : 0.0f;
	}

	ed::string   name;
	ed::string   mat_name;
	Position3	  pos;
	Position3     init_Position3;
	ccIndicatorPage* parent;
	Vector3		  init_pos;
	Vector3		  init_rot;
	bool          firstUPDATE; // для начального пересчета вертексов в нормальные координаты
	bool          isDraw; // Отрисовывается или нет. Определяется в update и при клиппировании
	bool		  collimated;
	bool          need_apply;
	bool          temporal_set_used;
	unsigned	  temporal_set;
	bool          params_used_from_root;

	ed::vector<element_controller> element_controllers;
	ccElementsArray  children;
	Element         *root;
	ed::string      root_name;
	DParamList       element_params;

	ccElementsArray		geometry_children;
	ccUniqueElementsMap geometry_hosts;
	bool				ToCheckGeometry;

	int					level;
	bool				isvisible;
	cc_h_clip_relations h_clip_relation;
	unsigned char		screenspace;
	bool				worldspace;
	
	bool	use_mipfilter;
	bool	use_forced_color;
	bool	additive_alpha;
    bool    use_contrast;
    float   contrast_power; 

	bool	z_enabled;
	bool	z_enable_flag_known;


	bool    break_update_cycle;
	Graphics::Color  forced_color;

	bool	Precreated;

private:
    bool custom_mfd_material_params;
    bool cmfd_colored;
	bool cmfd_inverted;
};

class COCKPITBASE_API ceHWLine : public ceSimple
{
public:
	ceHWLine();
	virtual ~ceHWLine();
	int			 getValue(lua_State *L, const ed::string& field) const;
	virtual void setValue(lua_State *L, const ed::string& field);
	virtual void rescale();

	void setVerts(const vectorarray&);
	void setIndices(const indexarray&);
		
	float get_length() const {return length;};
	void set_length(const float length_in);
	
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceHWLine";
		return class_name_;
	}

	Element * copy();
	
protected:
	virtual void OnFirstTime();
	void calculate_texcoords(Graphics::RenderObject*);
	void set_position_to_render();
	void self_render(Graphics::RenderParser& parser);

	Graphics::RenderObject robj_line1;
	Graphics::RenderObject robj_line2;
	Graphics::RenderObject mask_obj1;
	Graphics::RenderObject mask_obj2;

	Graphics::PrimitiveType prim;
	vectorarray verts;
	indexarray indices;
	vectorarray maskvertices;


	Position3 pos_line2;
	Position3 pos_mask_obj1;
	Position3 pos_mask_obj2;
	float length, maxlength;

	bool      use_single_line;
	
	Vector4 tex_params; // x - dx в координатах текстуры, y - dy в коорд. текст., z,w - постоянный множитель (коорд. на текстуре -> координаты при отрисовке).
};

class COCKPITBASE_API ceHWSector : public ceSimple
{
public:
	ceHWSector();
	virtual ~ceHWSector();
	int			 getValue(lua_State*, const ed::string&) const;
	virtual void setValue(lua_State*, const ed::string&);
	virtual void rescale();

	void setVerts1(const vectorarray&);
	void setVerts2(const vectorarray&);
	void setIndices(const indexarray&);
		
	float get_angle() const {return angle;};
	void set_angle(const float angle_in){angle = MIN_MAX(minangle, angle_in, maxangle);};
	
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceHWSector";
		return class_name_;
	}
	
	Element * copy();
protected:
	virtual void OnFirstTime();
	void calculate_texcoords(Graphics::RenderObject*, char);
	void get_vertices(lua_State*, char) const;
	void set_vertices(lua_State*, char);
	void set_position_to_render();
	void self_render(Graphics::RenderParser& parser);

	Graphics::RenderObject robj_element1;
	Graphics::RenderObject robj_element2;
	Graphics::RenderObject mask_obj1;
	Graphics::RenderObject mask_obj2;

	Graphics::PrimitiveType prim;
	vectorarray verts1;
	vectorarray verts2;
	indexarray indices;
	vectorarray maskvertices;

	Position3 pos_element2;
	Position3 pos_mask_obj1;
	Position3 pos_mask_obj2;
	float angle, minangle, maxangle;
	
	// x - dx в координатах текстуры, y - dy в коорд. текст., z, w - постоянный множитель (коорд. на текстуре -> координаты при отрисовке).
	Vector4 tex_params1;
	Vector4 tex_params2;
};

class COCKPITBASE_API ceMeshPoly : public ceSimple
{
public:
	ceMeshPoly();
	virtual ~ceMeshPoly();
	virtual int	getValue(lua_State *L, const ed::string& field) const;
	virtual void setValue(lua_State *L, const ed::string& field);
	virtual void rescale();
	
	void setVerts(const vectorarray&);
	void setIndices(const indexarray&);
		
	Graphics::PrimitiveType prim;
	vectorarray verts;
	indexarray indices;
	
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceMeshPoly";
		return class_name_;
	}

	Element *copy();
	void	 apply();
	virtual const vectorarray* getGeometry() const {return &verts;}

protected:
	Graphics::RenderObject robj;
	void    self_render(Graphics::RenderParser & parser);

	virtual void OnFirstTime();
};

class COCKPITBASE_API ceTexPoly : public ceMeshPoly
{
public:
	ceTexPoly();
	virtual ~ceTexPoly(){};
	int  getValue(lua_State *L, const ed::string& field) const;
	void setValue(lua_State *L, const ed::string& field);
	
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceTexPoly";
		return class_name_;
	}
	
	void  setState(int texture_state);
	int	  getState() const	{ return state;	}

	Element *copy();
protected:
	virtual void OnFirstTime();
	void calculate_texcoords(Graphics::RenderObject*);

	Vector4					tex_params; // x - dx в координатах текстуры, y - dy в коорд. текст., z,w - постоянный множитель (коорд. на текстуре -> координаты при отрисовке).
	bool					direct_assignment_tex_coords;
	ed::vector<vectorarray> statesTexCoords;
	int						state;
};

class COCKPITBASE_API ceSimpleLineObject : public ceSimple
{
public:
	 ceSimpleLineObject(); 
		~ceSimpleLineObject();
	int  getValue(lua_State *L, const ed::string& field) const;
	void setValue(lua_State *L, const ed::string& field);
	void addSegment(const Position3 & pos_ ,float length)
	{
		addSegment(pos_.p,pos_.p + pos.x * length);
	}
	void addSegment(const Vector3 & start,const Vector3 & end);

	void setPointAtIndex(unsigned i,const Vector3 & pnt,bool check_difference = false);
	void setLineStrip(const vectorarray & pnts,unsigned * count_of_points = NULL);
	void reset();
	void apply();
	void rescale();
	unsigned get_points_number() const
	{
		return LineVerts.size();
	}

	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceSimpleLineObject";
		return class_name_;
	}

	Element *copy();
	virtual const vectorarray* getGeometry() const {return &LineVerts;}

protected:
	void OnFirstTime();

protected:
	void  self_render(Graphics::RenderParser & parser);
	float length_of_texcoords() const { return (tex_start - tex_end).length();}

	int   get_number_of_segments() const
	{
		unsigned sz = LineVerts.size();
		return   sz < 2 ? 0 : sz - 1;
	}

	Graphics::RenderObject  robj;
	Graphics::PrimitiveType prim;
	vectorarray				verts;
	vectorarray				LineVerts;
	indexarray				indices;
	vectorarray				current_tex_coords;

	Vector3					tex_start;
	Vector3                 tex_end;
	float                   tex_coefficient_x;
	float                   tex_coefficient_y;

	float					segment_width;
	float					center_line_offset;

	bool                    scaled;
	bool					tiled;
};

class COCKPITBASE_API ceCircle : public ceMeshPoly
{
public:
	ceCircle();
	virtual ~ceCircle(){};
	int  getValue(lua_State *L, const ed::string& field) const;
	void setValue(lua_State *L,const ed::string &field);
	void set_arc_start        (float angle_rad);
	void set_arc_end          (float angle_rad);
	void set_arc_segment_width(float angle_rad);
	void set_radius_inner     (float radius);
	void set_radius_outer     (float radius);
	void apply();
	
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceCircle";
		return class_name_;
	}

	Element *copy();
protected:
	bool isSolid;
	float arc_start;
	float arc_end;
	float arc_segment_width;
	float radius_inner;
	float radius_outer;
};

class COCKPITBASE_API ceGeometryClone : public ceSimple
{
public:
	ceGeometryClone() 
	{
		robj.geometry = 0;
		robj.material = 0;
		robj.pos      = &pos;
	};
	ceGeometryClone(const Graphics::RenderObject & obj)
	{
		robj.geometry = obj.geometry;
		robj.material = obj.material;
		robj.pos      = &pos;
	}
	virtual ~ceGeometryClone(){};
	int  getValue(lua_State *L, const ed::string& field) const;
	void setValue(lua_State *L,const ed::string &field);
	void set_geometry(Graphics::Geometry *geom,Graphics::Material *mat)
	{
		robj.geometry = geom;
		robj.material = mat;
	}
	
	virtual const ed::string& class_name() const 
	{
		static ed::string class_name_ = "ceGeometryClone";
		return class_name_;
	}

	Element *copy();
protected:
	void self_render(Graphics::RenderParser& parser);
	Graphics::RenderObject robj;
	ed::string			   geometry_source_name;
};

class COCKPITBASE_API ceBoundingMeshBox : public ceMeshPoly
{
public:
	ceBoundingMeshBox();
	virtual ~ceBoundingMeshBox() {}

	int			 getValue(lua_State *L, const ed::string& field) const;
	virtual void setValue(lua_State *L, const ed::string& field);
	virtual void OnFirstTime();
	virtual void rescale();
	void setVerts(const vectorarray&);
	virtual void apply();

	virtual const ed::string& class_name() const 
	{
		static ed::string class_name_ = "ceBoundingMeshBox";
		return class_name_;
	}

	Element* copy();	

protected:
	vectorarray DefaultVerts;
};

class COCKPITBASE_API ceBoundingTexBox : public ceSimpleLineObject
{
public:
	ceBoundingTexBox();
	virtual ~ceBoundingTexBox() {}

	int			 getValue(lua_State *L, const ed::string& field) const;
	virtual void setValue(lua_State *L, const ed::string& field);
	virtual void rescale();
	void setVerts(const vectorarray&);
	virtual void apply();
	void		 set(float miny = 0.0f, 
				     float maxy = 0.0f, 
					 float minz = 0.0f,
					 float maxz = 0.0f);
	void		 set_default();

	virtual const ed::string& class_name() const 
	{
		static ed::string class_name_ = "ceBoundingTexBox";
		return class_name_;
	}

	Element* copy();	

protected:
	virtual void OnFirstTime();

	vectorarray DefaultLineVerts;
};

void copy_geometry(Graphics::Geometry *out,Graphics::Geometry * in);
void copy_robj(Graphics::RenderObject & out,const Graphics::RenderObject & in);

inline void calc_screen(const Graphics::Camera & cam,double & tan_h,double & tan_v)
{
    tan_v  = tan(double(0.5*cam.getVAngle()));
    tan_h  = tan_v * cam.getAspect();
}
}

#endif // ccIndication_H

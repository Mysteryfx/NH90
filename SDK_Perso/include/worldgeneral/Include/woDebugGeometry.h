#pragma once
#include "WorldGeneral.h"

	#include "dPosition.h"
	#include "gMath.h"	
	#include <ed/string.h>
	#include "Modeler\ModelParser.h"	
	#include "Renderer/DbgGraph.h"	
	#include <ed/map.h>

	class WORLDGENERAL_API dbg_draw_structure
	{
	public:
		dbg_draw_structure() : in_world_coords(false), draw_enable(true) {;}
		//dbg_draw_structure(const dbg_draw_structure & dbg_draw_structure_) {(*this) = dbg_draw_structure_;}
		~dbg_draw_structure() {clear();}
		virtual void set() {;}
		virtual void update_position();
		virtual const Position3 get_obj_position() const {return Position3();}
		virtual void Parse(Graphics::ModelParser& parser, const Position3& pos);
		virtual void force_color(const Vector3 & color);
		virtual void clear();
		virtual dbg_draw_structure & operator = (const dbg_draw_structure & dbg_draw_structure_);
		virtual void set_draw_enable(bool draw_enable_) { draw_enable = draw_enable_; }
		//
		bool empty() const { return dbg_objs.empty(); }	
		bool is_draw_enable() const { return draw_enable; }
		//
		static float height;
		static cPosition get_position(const cVector & start_point,	const cVector & end_point);
		static dPosition get_position(const dVector & start_point,	const dVector & end_point);
		static Position3 get_position(const Vector3 & start_point,	const Vector3 & end_point);
	protected:
		const dbg_obj * get_dbg_obj(const ed::string & name) const;
		void add_dbg_obj(const ed::string & name, dbg_obj* dbg_obj_);
		void remove_dbg_obj(const ed::string & name);
		ed::map<ed::string, dbg_obj*> dbg_objs;
		bool in_world_coords;
		bool draw_enable;
	};


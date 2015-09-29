#pragma once

#include "CockpitBase.h"
#include "Utilities/ccSimpleOctTree.h"
#include "Indicators/ccIndicatorPageImpl.h"
#include "Indicators/ccIndicationTemplateBuffer.h"

#include <ed/vector.h>

struct lua_State;
namespace cockpit
{
	class COCKPITBASE_API chart_item : public SimpleOctTreeLoad
	{
	public:
				  chart_item();
 		virtual  ~chart_item();
		void	  set_limits (double min_x,
							  double max_x,
							  double min_z,
							  double max_z);
		void					add(Element * el);
		void					set_metatable(lua_State *Ls);
		static void				register_metatable(lua_State *Ls);
		void					update();
		void					draw(Graphics::RenderParser &parser,unsigned char level);
	protected:
		ed::vector<Element*>   elements;

		static int   l_add_element(lua_State *Ls);
		static int   l_center(lua_State *Ls);
		static int   l_AABB(lua_State *Ls);
		static int   l_OBB(lua_State *Ls);
	};
	typedef Mem::Ptr<chart_item>  chart_item_ptr;
	typedef ed::vector<SimpleOctTree*> charts_vector;
	class COCKPITBASE_API ccMovingMap : public ccIndicatorPage
	{
	public:
		 ccMovingMap();
		~ccMovingMap();

		void  draw(Graphics::RenderParser &parser,unsigned char elem_level)
		{
			if (map_available)
			{	
				render(parser,elem_level);
			}
			ccIndicatorPage::draw(parser,elem_level);
		}
		bool	  to_texture() const {	return full_render_to_texture;}
		void	  set_current_level(unsigned level);
		unsigned  get_current_level() const { return current_level;};
		void      push_to_remove(Element * el) { elements_in_charts.push_back(el);};

		void	  set_map_availability(bool flag_in) { map_available = flag_in;};
		bool      check_availability(unsigned level) const; 
	protected:
		void		 reg_in_reusable_state(lua_State *Ls);
		static int   l_create_chart(lua_State *Ls);
		static int   l_set_render_to_texture(lua_State *Ls);
		static int   l_get_render_to_texture(lua_State *Ls);
		static int   l_set_lat_lon_aspect(lua_State *Ls);
		static int   l_set_clip_by_planes(lua_State *Ls);

		void		 distribute_elements();

	

		bool		 full_render_to_texture;
		bool		 in_geo_space;
		bool         use_clip_planes;
		bool		 map_available;
		
		SimpleOctTree * get();
		SimpleOctTree * check_charts(unsigned level,double cell);
		Box             get_bound() const;
		float			get_minimum_cell() const;

		void			render(Graphics::RenderParser &parser,unsigned char elem_level);
		charts_vector   charts;
		unsigned	    current_level;

		float			lat_lon_aspect;

		ccElementList	elements_in_charts;
	};


	class avMovingMapPoint;
	class COCKPITBASE_API ccMapObjectsBuffer : public ccIndicationTemplateBuffer
	{
	public:
		ccMapObjectsBuffer(unsigned number);
       ~ccMapObjectsBuffer();

		void rebuild(Element *start);
		void draw	(Graphics::RenderParser & parser,unsigned char elem_level);
		void initialize(ccIndicator *parent,lua_State *L,const char *name);
		void add_waypoint  (avMovingMapPoint *wpnt,bool directly = false);
		void reset();
		void set_cash(unsigned new_cash) { cash = new_cash;}
	protected:
		virtual Element	* get_element(avMovingMapPoint *wpnt) = 0;
		virtual ccElementList::iterator	  where_insert(Element * el) = 0;
		ccElementList   elements;

        unsigned        cash;

        Element * get_template_by_index(unsigned index)
        {
            assert(index < number_of_elements); 
            return specific_elements[index];
        }
    private:
		Element**       specific_elements;
		unsigned		number_of_elements;
	};

}
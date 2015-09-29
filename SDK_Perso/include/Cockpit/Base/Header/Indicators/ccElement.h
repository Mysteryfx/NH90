#ifndef ccElement_H
#define ccElement_H

#include "CockpitBase.h"
#include "Common.h"
#include "Modeler/ModelParser.h"
#include "Renderer/DynamicParams.h"
#include "Utilities/ccUtilities.h"

#include <ed/string.h>
#include "ed/unordered_map.h"

namespace cockpit
{

class ccIndicatorPage;
class ccDrawable;
class Element;

struct element_controller
{
	void (*func)(Element*, ccDrawable*, const Graphics::DParamList& args);
	Graphics::DParamList args;

	inline void apply(Element * elem, ccDrawable* indicator)
	{
		if (func)
		{
			(*func)(elem,indicator,args);
		}
	}
};

enum enum_draw_level
{
    DRAW_LEVEL_COMMON,          //позиция в мире
    DRAW_LEVEL_SCREEN,    //объект в плоскости отстоящей на 1 метр от точки камеры по ее оси (позиция ненормирована)
    DRAW_LEVEL_RENDER_TARGET,//объект в плоскости отстоящей на 1 метр от точки камеры по ее оси (позиция ненормирована) , только для рендера в текстуру
};            


typedef ed::vector<Element*> ccElementsArray;
typedef ed::unordered_map<ed::string, Element*> ccUniqueElementsMap;

class COCKPITBASE_API Element : public Common::FakeUnknown<Common::Identifiable>
{
public:
	virtual ~Element(){};

    virtual const ed::string & getName() const = 0;
	virtual void draw(Graphics::RenderParser&,unsigned char level) = 0;
	virtual void update() = 0;
	virtual void precreate() = 0;
	virtual const Position3& get_position() const = 0;
	virtual		  Position3& get_position_ref() = 0;
	virtual void set_position(const Position3&) = 0;
	
	virtual int  getValue(lua_State*, const ed::string&) const = 0;
	virtual void setValue(lua_State*, const ed::string&) = 0;

	virtual const ed::string& get_rootName() const= 0;
	virtual void setparent(ccIndicatorPage*) = 0;
	virtual ccIndicatorPage* getparent() = 0;
	virtual void setparent_element(Element*) = 0;
	virtual void setchild_element(Element*)  = 0;

	virtual void set_geometry_host(Element* elmt) = 0;
	virtual void set_geometry_child(Element* elmt) = 0;
	
	virtual const ccUniqueElementsMap& get_geometry_hosts() = 0;
	virtual ccElementsArray& get_geometry_children_ref() = 0;
	
	virtual bool is_collimated() const = 0;
	virtual bool get_isDraw() const = 0;
	virtual void set_isDraw(bool) = 0;
	virtual void apply() = 0;
	virtual void setToCheckGeometry(bool flag) = 0;

	virtual Graphics::DParamList & get_params_ref()= 0;
	virtual const   Graphics::DParamList & get_params() const= 0 ;
	
	virtual Element* get_root() = 0;
	virtual void set_use_temporal_set(bool flag_in,unsigned used_set)= 0;
	virtual void set_use_root_params(bool flag_in)=0;
	virtual bool get_use_root_params()=0;

    virtual void set_use_contrast(bool flag_in) = 0;
    virtual void set_contrast(float power) = 0;

	virtual void set_use_forced_color(bool flag_in) = 0;
	virtual void set_forced_color(float R,float G,float B) = 0;
    virtual void set_forced_color(const Vector3 & col)  =0;
	virtual void set_break_update_cycle(bool flag_in) = 0;
	virtual Element *copy() = 0;
	virtual const ed::string& class_name() const = 0;
	virtual const vectorarray* getGeometry() const {return 0;}

    virtual unsigned char get_draw_level() const = 0;

    virtual void          log(ed::string & out,bool full = false) = 0;
};

}

#endif // ccElement_H

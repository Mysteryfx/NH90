#include "Indicators/ccElement.h"
#include "ccClickableElementData.h"

namespace cockpit
{

class COCKPITBASE_API ceHint : public Element
{
public:
	ceHint();
	virtual ~ceHint();
	int				 getValue(lua_State* L, const ed::string& field) const;
	void			 setValue(lua_State* L, const ed::string& field);
	void			 draw(Graphics::RenderParser& parser,unsigned char level = 0);
	void			 update();
	void			 set_position(const Position3& p){};
	void             setparent(ccIndicatorPage* ppage){parent = ppage;};
	ccIndicatorPage* getparent() {return parent;};
	const ed::string &  getName() const {return name;};
	void			 apply();
	
	void						 precreate() {}
	const Position3&			 get_position() const	{static Position3 pos;return pos;};
	Position3&					 get_position_ref()		{static Position3 pos;return pos;};
	bool						 is_collimated()  const  {return false;};
	bool						 get_isDraw() const	{return is_draw;}
	void						 set_isDraw(bool flag) { is_draw = flag;update_buffer = hint_text; need_apply = true;}
	DParamList		&			 get_params_ref()      {	return element_params;};
	const DParamList&			 get_params()   const 	{	return element_params;};
	void						 setparent_element(Element* elmt) {};
	void						 setchild_element(Element* elmt)  {};
	const ed::string&			 get_rootName() const  {static ed::string str ; return str;}
	Element*					 get_root()			 {return 0;}
	bool						 get_use_root_params() {return false;}

	void						 set_geometry_host(Element* elmt) {};
	void						 set_geometry_child(Element* elmt){};
	const ccUniqueElementsMap&	 get_geometry_hosts()	   { static ccUniqueElementsMap p; return p;}
	ccElementsArray&			 get_geometry_children_ref() { static ccElementsArray p; return p;}

	void setToCheckGeometry(bool flag) {}
	void set_use_temporal_set(bool flag_in,unsigned used_set) {}
	void set_use_root_params (bool flag_in){}
	void set_use_forced_color(bool flag_in){}
	void set_forced_color(float R,float G,float B){}
    void set_forced_color(const Vector3 & col) {}
	void set_break_update_cycle(bool flag_in) { break_update_cycle = flag_in;};
    void set_use_contrast(bool flag_in){};
    void set_contrast(float power){};



	// Функция перегружает virtual const ed::string& class_name() const = 0,
	// поэтому inline будет проигнорировано при компиляции
	//
	//inline const ed::string & class_name() const 
	const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceHint";
		return class_name_;
	}

	Element* copy();

	void set_formatted_string(const char* format, ...);
	void set_string(const ed::string& txt);
	void set_strcat(const char* format, ...);
	void set_strcat_by_predefined_format(int format_number, ...);

    unsigned char get_draw_level() const { return DRAW_LEVEL_COMMON; }

    void          log(ed::string & out,bool full){};
protected:
	Mem::Ptr <clickableElementData>		hint;
	ed::string							hint_name;
	ed::string                         hint_text;
	ed::string							name;
	ed::string							update_buffer;
	ed::vector<ed::string>			formats;


	ccIndicatorPage* parent;
	bool			 need_apply;
	ed::vector<element_controller> element_controllers;
	DParamList       element_params;
	bool			 break_update_cycle;
	bool             is_draw;
};

}

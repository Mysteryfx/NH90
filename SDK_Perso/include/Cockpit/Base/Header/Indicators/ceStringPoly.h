#ifndef ccStringPoly_H
#define ccStringPoly_H

#pragma  once
#include "Indicators\ccIndication.h"
#include "Indicators\ceFont.h"
#include "Utilities\ccClipping.h"
#include "Utilities\ccUtilities.h"

typedef ed::list<Box> box_list;
typedef ed::vector<ed::string> formates_set;

namespace cockpit
{

class COCKPITBASE_API ceStringPoly : public ceMeshPoly
{
public:
	ceStringPoly();
	~ceStringPoly() {}
	int			 get_length(){return (int)text.size();};
	virtual int  getValue(lua_State *L, const ed::string& field) const;
	virtual void setValue(lua_State *L, const ed::string& field);
	void         set_string(const ed::string& txt);
    void         set_string(const char* txt);

	void         set_formatted_string(const char* format, ...);
	void         set_strcat(const char* format, ...);
	void         set_strcat_by_predefined_format(int format_number, ...);
	void         apply();
	virtual void set_material(const ed::string& mat_name_in);
	void         rescale();
	void		 self_render(Graphics::RenderParser& parser);
	const formates_set & get_formats() {return formats;};
	
	virtual const ed::string & class_name() const 
	{
		static ed::string class_name_ = "ceStringPoly";
		return class_name_;
	}

	Element		*copy();
	virtual void precreate();

	void         set_use_background(bool flag_in) { UseBackground = flag_in;}
	void         set_background_use_forced_color(bool flag_in) { background_use_forced_color = flag_in;}
    void         set_background_forced_color(float R,float G,float B);
    inline void  set_background_forced_color(const Vector3 & col) { set_background_forced_color(col.x,col.y,col.z); } ;

    void         reverse_video(bool value);//swap background color and font color 

    void         UTF8_substr_buffer(size_t pos,size_t n);
protected:
    void         log_self(ed::string & out);

	void		 OnFirstTime(); // подготовка и перевод нормированных данных к нормальному виду
private:        
    void         apply_text();
public://data
	vectorarray	   texture;
protected: //data
	ed::string          text;
	ed::string          update_buffer;
	formates_set         formats;
	ceFont              *font;
	Box					 bound;
	
	ceFontStringDefinition defs;

	bool					UseBackground;
	Graphics::RenderObject	RobjBackground;
	ed::string				BackgroundMatName;

	bool                    background_use_forced_color;
	Graphics::Color         background_forced_color;
};

#include "ceStringPoly.inl"

};

#endif // ccStringPoly_H

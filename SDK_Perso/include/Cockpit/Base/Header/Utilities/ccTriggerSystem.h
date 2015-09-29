#pragma once;
#include "CockpitBase.h"
#include <ed/string.h>
struct lua_State;

namespace cockpit { namespace triggers {

class input_listener 
{
public:
    ed::string		flag_to_set;
    int				target_hit_count;// if 0 then always
    int				hit_count;
    float			min_value;
    float			max_value;
	unsigned char	device;
    bool        SetCommand(int command,float value);//return true if need to be deleted from queue
};

class event_listener 
{
public:
    ed::string flag_to_set;
    bool        OnCockpitEvent(const ed::string & event);
};


void   reg_in_script(lua_State * L);
void   on_cockpit_initialize();
void   on_cockpit_release();

void   dbg_draw();







COCKPITBASE_API void * get_parameter_handle(const char * name);
COCKPITBASE_API void   update_parameter    (void * handle,const char * string_value);
COCKPITBASE_API void   update_parameter    (void * handle,double       number_value);
COCKPITBASE_API bool   parameter_value     (const void * handle,double & res,bool interpolated = false);
COCKPITBASE_API bool   parameter_value     (const void * handle,ed::string & res);
COCKPITBASE_API bool   parameter_value     (const void * handle,char * buffer,unsigned buffer_size);
COCKPITBASE_API void   push_parameter_to_state(lua_State * L, const char * name);
COCKPITBASE_API int    compare_parameters  (void * handle_1,void * handle_2);  //return 0 if equal , -1 if first less than second 1 otherwise

COCKPITBASE_API unsigned   highlight_new();
COCKPITBASE_API void	   highlight_clickable (unsigned hightlight_id,const char * element);
COCKPITBASE_API void	   highlight_remove    (unsigned hightlight_id); 
COCKPITBASE_API bool	   highlight_visible   (unsigned hightlight_id);


typedef std::function<double()>  param_proxy_get;

class COCKPITBASE_API double_param_proxy
{
protected:
	void * handle;
public:
	bool empty() const { return !handle; }
	double_param_proxy() { handle = nullptr; };

	void init_with(const char * name) 
	{
		handle = get_parameter_handle(name); 
	}

	operator double() const
	{
		double v = 0;
		parameter_value(handle,v);
		return v;
	}

	double_param_proxy & operator = (double v)
	{
		update_parameter(handle,v);
		return *this;
	}
};

class COCKPITBASE_API double_lambda_proxy : public double_param_proxy
{
public:
	~double_lambda_proxy();
	void init_with(const char * name,const param_proxy_get & lambda);
};

class COCKPITBASE_API string_param_proxy
{
	void * handle;
public:
	bool empty() const { return !handle; }
	string_param_proxy() { handle = nullptr; };

	void init_with(const char * name) { handle = get_parameter_handle(name); }

	operator const char * () const;
	string_param_proxy & operator =(const char *);
	string_param_proxy & operator =(const ed::string & str);
};


}}
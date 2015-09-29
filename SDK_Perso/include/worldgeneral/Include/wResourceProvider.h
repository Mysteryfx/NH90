#ifndef _W__ResourceProvider_h__
#define _W__ResourceProvider_h__

#include "WorldGeneral.h"

#include "wTime.h"
#include "wMovingObject.h"

#include <ed/string.h>
#include <ed/vector.h>

namespace Lua
{
	class Config;
};

class WORLDGENERAL_API wResourceProvider
{
public:
	struct WORLDGENERAL_API Params
	{
	public:
		typedef ed::vector<ed::string> ObjectModelNames;
		typedef ed::vector<ObjectModelNames> ObjectModelNamesByResources;
		typedef ed::vector<wResourceProvider::Params::ObjectModelNamesByResources> ObjectModelNamesByResourcesByCountries;
	public:
		Params() { clear(); }
		void clear();
		virtual void load(Lua::Config & config);
		void load1(Lua::Config & config, unsigned int resource0, unsigned int resources_qty);
		void load2(Lua::Config & config);
	public:
		ObjectModelNamesByResourcesByCountries	object_model_names_by_resources_by_countries;
		unsigned int							resource0;
		float									search_radius;
		wModelTime								update_dt;
	};
	typedef ed::vector<std::pair<bool, unsigned int> > ResourcesValue;
	typedef ed::list< ObjectID > RequiredStaticObjects;
	typedef ed::vector<RequiredStaticObjects> ResourceRequiredStaticObjects;
public:
	wResourceProvider();
	inline void					set_object(const woPointer & objectIn) { object = objectIn; }
	inline void					reset() { first_update = true; }
	virtual bool				check_resource(unsigned char resource_name, unsigned int * obj_ID = NULL, float * value = NULL) const;
	woPointer					get_object() const { return object; }
protected:
	virtual viFlag				is_objects_required() const = 0;	
	virtual const Params *		get_params() const = 0;
	virtual const Params::ObjectModelNames * get_model_names_by_resource(unsigned resource_name) const;
	virtual const unsigned int	get_resource_qty() const = 0;
	virtual void				update_resources() const;
	virtual	void				clear_cash() const;
	virtual	void				after_update() const { }
	virtual bool				get_resource(unsigned char resource_name, unsigned int * obj_ID, float * value = NULL) const;
	virtual void				set_resource(unsigned char resource_name, bool b, unsigned int obj_ID, float value = 0.0f) const;
	virtual float				on_resource_expenditure(unsigned char resource_name, float value = 0.0f) { return 0.0f; }
#ifdef DEBUG_TEXT
public:
	virtual ed::string			get_resources_str() const;
protected:
	virtual ed::string			get_resource_name_str(unsigned char resource_name) const { return ""; }	
#endif
protected:
	woPointer					object;
	mutable	ResourcesValue		resources_value;	
	mutable	bool				first_update;
	mutable wModelTime			resource_last_update_time;
	//
	mutable ResourceRequiredStaticObjects resource_req_static_obj_ids;
};

#endif
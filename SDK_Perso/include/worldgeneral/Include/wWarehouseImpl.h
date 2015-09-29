#pragma once
#include "wWarehouse.h"
#include "wsType.h"
#include "ed/map.h"
#include "ed/vector.h"
#include "Common\Link.h"
#include "Common\cPointerTemplate.h"
#include "cLinear.h"
#include "ed/unordered_map.h"
#include "IwNetObject.h"
#include "Mail\Reader.h"
#include "Mail\GenMessage.h"
#include "WorldMail.h"

#include "LandObjects/lLandObject.h"

class  wWarehouse;
class  wsPilonLoad;
class  lWarehouseOwner;
class  MovingObject;
class  viObject;
class  suppliers_rearrange;

struct warehouse_shipping;

template <class T> class stored_entry_data 
{
	void update_data(T delivery)
	{
		if (delivery > 0) //прибавка
		{
			T old_on_the_way = on_the_way;
			if (on_the_way < delivery)
			{
				on_the_way = 0;
			}
			else
			{
				on_the_way -= delivery;
			}
			change_on_the_way_from_last += on_the_way - old_on_the_way;
		}
		change_from_last		    += delivery;
		current					    += delivery;
	}
public:
	stored_entry_data():current(0),desired(0),on_the_way(0),unlimited(false),dirty(false),resupply_level(0.75),	change_from_last(0),change_on_the_way_from_last(0){}
	T current;
	T desired;//initial 
	T on_the_way;//requests sended and stuff is on the way to warehouse
	bool unlimited;

	//network sync
	mutable  bool   dirty;
	mutable  T		change_from_last;
	mutable  T		change_on_the_way_from_last;

	double resupply_level;


	T get_shortage() const
	{
		if (current + on_the_way >= desired)
			return 0;
		return desired - current - on_the_way;
	}

	bool  need_resupply() const
	{
		return (double)get_shortage() > double(desired) * (1.0 - resupply_level);
	}

	void accept(T delivery)
	{
		assert(delivery >= 0);
		update_data(delivery);
		dirty  = true;
	}
	T substract(T desired_)
	{
		if (unlimited)
			return desired_;
		if (current  < desired_)
			desired_ = current;
		update_data(-desired_);
		if (desired_)
			dirty = true;
		return desired_;
	}

	void damage(double percentage)
	{
		T lost(double(desired) * percentage);
		if (lost >= current)
		{
			lost	  = current;
			unlimited = false;
		}
		update_data(-lost);
		dirty    = true;
	}

	void clean_up() const
	{
		change_from_last			= 0;
		change_on_the_way_from_last = 0;
		dirty						= false;
	}
};


class warehouse_state
{
public:
	struct resource_state
	{
		wsType	 type_;
		int		 count_;
		int		 on_the_way_;
	};
	struct resource_state_named
	{
		ed::string	type_;
		int			count_;
		int			on_the_way_;
	};

	ed::string name_;
	bool		disabled_;
	double		fuel_;
	double		fuel_on_the_way_;

	bool		unlimited_aircraft;
	bool		unlimited_weapon;
	bool		unlimited_fuel;

	mutable bool is_it_incremental;


	ed::vector<resource_state>		 updated_resources_;
	ed::vector<resource_state_named> updated_resources_named_;
};


enum stored_resource_type
{
	WSRT_WEAPON_AND_EQUIPMENT,
	WSRT_FUEL,
	WSRT_VEHICLE
};

class wWarehouse : public LinkHost,
		   		   public IwWarehouse
{


	LandObjectDestroyNotifierBase * damage_listener;


    friend  class resupply_activity;
    friend  class shipping_activity;

	void	on_create();

	void	onLinkedObjectDestroy(lLandObject * obj);
public:
	typedef  cPointerTemplate<wWarehouse> warehouse_ptr;
	typedef  stored_entry_data<int>  equipment_entry_data;
public:

	wWarehouse(MovingObject * obj)	 :damage_listener(NULL),static_obj_owner(NULL),world_obj_owner(obj)	{	on_create();	}
	wWarehouse(lWarehouseOwner * obj):damage_listener(NULL),static_obj_owner(obj) ,world_obj_owner(NULL){	on_create();	}

	virtual ~wWarehouse();

	unsigned ship_weapon(const wsPilonLoad & loadout);  
	unsigned ship(const wsType & type,unsigned desired_count)					{   return ship_res_(type		   ,desired_count);}
	unsigned ship(const ed::string & named_resource,unsigned desired_count)    {   return ship_res_(named_resource,desired_count);}


	double wWarehouse::ship_fuel( double desired_volume )
	{
		if (is_disabled())
			return 0;
		double shipped  = ship_(fuel_,desired_volume);
		double remained = desired_volume - shipped;
		if (remained > 0)
		{
			for (auto it  = suppliers_instant_.begin();
					  it != suppliers_instant_.end() && remained > 0;
					++it)
			{
				warehouse_ptr & ptr = *it;
				if (!ptr || ptr->is_disabled())
					continue;
				double volume = ptr->ship_fuel(remained);
				shipped		  += volume;
				remained	  -= volume;
			}
		}
		return shipped;
	}

	void     accept_weapon_shipping(const wsPilonLoad & loadout);
	void     accept_shipping(const wsType & equipment,unsigned count)			{	accept_shipping_res_(equipment	   ,count);}
	void     accept_shipping(const ed::string & named_resource,unsigned count) {	accept_shipping_res_(named_resource,count);}
	void     accept_fuel_shipping(double fuel);

	void     accept_shipping(const warehouse_shipping & shipping);

	unsigned get_available_count(const wsPilonLoad & loadout)  const;
	unsigned get_available_count(const wsType & type)   const		{ return get_available_res_count_(type);}
	unsigned get_available_count(const ed::string & type)   const  { return get_available_res_count_(type);}
	double   get_available_fuel()   const;

	bool	 is_weapon_available() const;

	unsigned request_shipping_equipment(wWarehouse * consumer,const wsType & type,unsigned desired_count);//return remained to ship
	unsigned request_shipping_equipment(wWarehouse * consumer,const ed::string & named_resource,unsigned desired_count);//return remained to ship
	double   request_shipping_fuel     (wWarehouse * consumer,double desired_volume);//return remained to ship
	const dVector & get_pos() const;

	void	 set_pos(const dVector & pos);
	void	 init(Lua::Config & config);

	void     set_as_disabled(bool value)
	{ 
		if (disabled_ != value) 
			dirty_	  = true;
		disabled_ = value;
	};
    bool     is_disabled() const { return disabled_; }

    void     take_mission_name(const ed::string & name);

	const ed::string & get_mission_name() const { return mission_name; }//DEBUG

	wcCoalitionName Coalition() const;

	bool	 is_movable() const;

	bool	 get_net_state(warehouse_state  & w_state) const;
	bool     set_net_state(const warehouse_state  & w_state);

	bool	 get_net_update(warehouse_state & w_state) const;
	bool	 set_net_update(const warehouse_state & w_state);


	void	 attach_object(use_object_as_store kind,lLandObject * store);
	void	 check_instant_suppliers(double range);

	inline MovingObject		   * get_owner_as_moving_object() const
	{
		return world_obj_owner;
	}

	bool	 is_weapon_unlimited() const { return unlimited_weapon_; }
	bool	 is_aircraft_unlimited() const { return unlimited_aircrafts_; }

private: 
    void     set_need_resupply();
    void     read_equipment(Lua::Config & config);

	void     check_resupply();
    void     check_shipping();
    void     check_resupply_timer();
    void     check_shipping_timer();
	void	 add_supplier(const warehouse_ptr &  sup);

	unsigned get_resource_type(const wsType		 & type) const;
	unsigned get_resource_type(const ed::string & type) const { return WSRT_WEAPON_AND_EQUIPMENT; }//TODO: make check of res type

	template<class res_type>
	bool     check_unlimited(const res_type & type) const
	{
		if (get_resource_type(type) == WSRT_VEHICLE)
			return unlimited_aircrafts_;
		return unlimited_weapon_;
	}


	template<class res_type>
	double	 resupply_level_(const res_type & res) const
	{
		if (get_resource_type(res) == WSRT_VEHICLE)
			return OperatingLevel_Air;
		return OperatingLevel_Eqp;
	}

	bool res_type_invalid(const wsType & type)
	{
		return type.Type() == 0;
	}
	bool res_type_invalid(const ed::string & type)
	{
		return type.empty();
	}


	template<class res_type>
	unsigned check_store_resupply_(const res_type & type,equipment_entry_data & store,suppliers_rearrange & maybe_need_to_sort);	 

	template<class res_type>
	unsigned ship_res_(const res_type & t,unsigned desired_count)
	{
		if (is_disabled())
			return 0;
		if (res_type_invalid(t))
			return 0;
		if (check_unlimited(t))
			return desired_count;
		unsigned shipped   = 0;
		equipment_entry_data * store = get_entry(t);
		if (store)
		{
			shipped = ship_(*store,desired_count);
		}
		unsigned remained = desired_count - shipped;	
		if (remained)
		{
			for (auto it  = suppliers_instant_.begin();
					  it != suppliers_instant_.end() && remained;
					++it)
			{
				warehouse_ptr & ptr = *it;
				if (!ptr || ptr->is_disabled())
					continue;
				unsigned count = ptr->ship(t,remained);
				shipped		  += count;
				remained	  -= count;
			}
		}
		return shipped;
	}

	template<class res_type>
	void accept_shipping_res_(const res_type & t,unsigned count)
	{
		if (is_disabled())
			return;
		if (!count || res_type_invalid(t))
			return;
		equipment_entry_data * store = get_entry(t,/*create*/true);
		if (store)
		{
			store->accept(count);
			if (store->dirty)
				dirty_ = true;
		}
	}

	template<class res_type>
	unsigned get_available_res_count_(const res_type & t) const
	{
		if (check_unlimited(t))
			return 1e6;	
		const equipment_entry_data * store = get_entry(t);
		unsigned self_storage = 0;
		if (store)
			self_storage = store->current;
		unsigned instant_available  = 0;
		for (auto it  = suppliers_instant_.begin();
				  it != suppliers_instant_.end();
				++it)
		{
			const warehouse_ptr & ptr = *it;
			if (!ptr || ptr->is_disabled())
				continue;
			instant_available += ptr->get_available_count(t);
		}
		return self_storage + instant_available;
	}

	typedef ed::map<wsType, equipment_entry_data > equipment_store;
	typedef ed::unordered_map<ed::string,equipment_entry_data > named_equipment_store;//some weapon doesn't have wstype,(shells for example)


	equipment_entry_data       * get_entry(const wsType & type,bool create = false);
	const equipment_entry_data * get_entry(const wsType & type) const;
	
	equipment_entry_data       * get_entry(const ed::string & name,bool create = false);
	const equipment_entry_data * get_entry(const ed::string & name) const;

	template <class store_type,class T> 
	unsigned					 ship_(store_type & store,T desired_count)
	{
		T shipped = store.substract(desired_count);
		if (store.need_resupply())
		{
			set_need_resupply();
		}
		if (shipped && store.dirty)
			dirty_	= true;
		return shipped;
	}

	template<class res_type>
	unsigned					 request_(wWarehouse * consumer,const res_type & type,unsigned desired_count);


	
	warehouse_shipping		   * get_shipping_for_consumer(wWarehouse * consumer);

	viObject				   * get_owner() const;


	template < class res_type> 
	unsigned					 instant_from_suppliers(const res_type & type ,unsigned desired_count)
	{
		unsigned shortage = desired_count;
		unsigned shipped  = 0;
		for (auto it  = suppliers_instant_.begin();
				  it != suppliers_instant_.end() && shortage;
				++it)
		{
			warehouse_ptr & ptr = *it;
			if (!ptr || ptr->is_disabled())
				continue;
			unsigned count = ptr->ship(type,shortage);
			shipped  += count;
			shortage -= count;
		}
		return shipped;
	}

	equipment_store					equipment_;   
	named_equipment_store			named_equipment_;


	ed::list<equipment_entry_data*> list_of_weapons;
	ed::list<equipment_entry_data*> list_of_aircrafts;


	stored_entry_data<double>		fuel_;
	ed::vector<warehouse_ptr>		suppliers_;
	ed::vector<warehouse_ptr>		suppliers_instant_;
	ed::vector<warehouse_shipping*>	requests_;
	mutable dVector					pos_;
    bool                            disabled_;
	bool							one_of_suppliers_is_movable_;//

    bool                            unlimited_weapon_;
    bool                            unlimited_aircrafts_;
    bool                            need_resupply_;

	double                          OperatingLevel_Eqp;
	double                          OperatingLevel_Air;  

    double                          caravan_periodicity_;
    double                          caravan_speed_;
    double                          caravan_capacity_;

    resupply_activity *p_resupply_timer_;
    shipping_activity *p_shipping_timer_;

	ed::string						mission_name;//DEBUG

	//owner info 
	lWarehouseOwner * static_obj_owner;
	MovingObject	* world_obj_owner;


	double		  lost_per_fuel_tank;
	double		  lost_per_weapon_store;
	double		  lost_per_technics_store;

	unsigned	  attached_fuel_tanks;
	unsigned	  attached_weapon_stores;
	unsigned	  attached_technics_stores;

	ed::map<lLandObject*,unsigned> attached;


	mutable	bool	  dirty_;

	friend class	  wWarehouseStatusDialog;
};

class wWarehouse_Manager : public IwNetObject
{
    typedef ed::unordered_map<ed::string,wWarehouse::warehouse_ptr>				  warehouses_map;
	typedef Mail::GenMessage<wMissionResourceMsgID + 0, Mail::CreateMsg,ed::vector<warehouse_state> > MsgWarehousesState;
	typedef Mail::GenMessage<wMissionResourceMsgID + 1, Mail::Reliable ,ed::vector<warehouse_state> > MsgWarehousesUpdate;

	warehouses_map named_;
	warehouses_map::iterator _enumPos;

	wWarehouse_Manager():init_done_(false){}
public:
	static wWarehouse_Manager & instance()
	{
		static wWarehouse_Manager mngr;
		return mngr;
	}
	wWarehouse * get(const ed::string & name) const
	{
		warehouses_map::const_iterator it = named_.find(name);
		if (it != named_.end())
			return it->second.Obj();
		return NULL;
	}    
	
	void  reg(wWarehouse * ptr,const ed::string & name)
	{
		named_[name] = ptr;
	}
	void		 start();
	void		 finish();

	// enumeration
	bool			startEnum()
	{
		_enumPos = named_.begin();
		return _enumPos != named_.end();
	}

	wWarehouse *	getNext()
	{
		if(_enumPos == named_.end())
			return NULL;

		wWarehouse* ptr = _enumPos->second.Obj();
		++_enumPos;

		return ptr;
	}


	 void   read_mission();
	/**
	 * IwNetObject interface
	 */
	virtual bool netGetCreate(Mail::Message&) const;
	virtual bool netSendUpdate(int dest, int prio, int& cookie);
private:
	void OnStateReceive(const ed::vector<warehouse_state> & net_state);
	void OnUpdateReceive(const ed::vector<warehouse_state> & net_state);

	void apply(const ed::vector<warehouse_state> & net_state);
	void force_update() const;

	bool init_done_;
};


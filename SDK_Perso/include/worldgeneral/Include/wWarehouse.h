#pragma once
#include "WorldGeneral.h"
#include "../../../Projects/math/Include/cLinear.h"
#include <ed/string.h>

namespace Lua
{
	class Config;
}

extern WORLDGENERAL_API bool _use_warehouse_;

class  IwWarehouse;
class  wsPilonLoad;
class  lWarehouseOwner;
class  MovingObject;
class  wsType;
class  lLandObject;

namespace warehouse_manager
{
	WORLDGENERAL_API ed::string	   warehouse_id_for_airport(int airport);
	WORLDGENERAL_API IwWarehouse  *get(const ed::string & name);
	WORLDGENERAL_API void		   start();
	WORLDGENERAL_API void		   read_mission();
	WORLDGENERAL_API void		   finish();
	WORLDGENERAL_API IwWarehouse * create_warehouse(MovingObject * obj);
	WORLDGENERAL_API IwWarehouse * create_warehouse(lWarehouseOwner * obj);
	WORLDGENERAL_API void		   show_stats();
	WORLDGENERAL_API void		   change_stats_target(IwWarehouse * warehouse);
	WORLDGENERAL_API bool		   startEnum();
	WORLDGENERAL_API IwWarehouse * getNext();
};

enum wcCoalitionName;

class WORLDGENERAL_API IwWarehouse
{
public:
	virtual ~IwWarehouse();	

	virtual  void	  init(Lua::Config & config) = 0;

	virtual  unsigned ship_weapon(const wsPilonLoad & loadout) = 0;  
	virtual  unsigned ship	     (const wsType	    & type		    ,unsigned desired_count) = 0;  
	virtual  unsigned ship	     (const ed::string & named_resource,unsigned desired_count) = 0;  
	virtual  double   ship_fuel  (double desired_volume) = 0;   

	virtual  void     accept_weapon_shipping(const wsPilonLoad & loadout) = 0;
	virtual  void     accept_shipping(const wsType & equipment,unsigned count) = 0;
	virtual  void     accept_shipping(const ed::string & named_resource,unsigned count) = 0;
	virtual  void     accept_fuel_shipping(double fuel) = 0;

	virtual  unsigned get_available_count(const wsPilonLoad & loadout)  const = 0;

	virtual  unsigned get_available_count(const wsType & type)  const = 0;
	virtual  unsigned get_available_count(const ed::string & type)  const = 0;
	virtual  double   get_available_fuel()   const = 0;

	virtual  bool	  is_weapon_available() const = 0;

	virtual   void				 set_as_disabled(bool value) = 0;
    virtual   bool				 is_disabled() const = 0;

    virtual   void				 take_mission_name(const ed::string & name) = 0;
	enum use_object_as_store
	{
		AS_FUEL_STORE,
		AS_WEAPON_STORE,
		AS_TECHNICS_STORE,
	};
	virtual	  void				 attach_object(use_object_as_store kind,lLandObject * store) = 0;

	// функциональность, необходимая для работы с наземными юнитами
	virtual wcCoalitionName	Coalition() const = 0;
	virtual const dVector &	get_pos() const = 0;
};



#pragma once
#include "CockpitBase.h"
#include "ed/map.h"
#include "ed/unordered_map.h"
#include "Utilities/avDevLink.h"
#include "Factory/SharedObject.h"
#include "Resourcer/HeapVector.h"
#include "Serializer/Serializable.h"
#include "wMovingObject.h"

#pragma region Forward Declarations
class  wHumanCommunicator;
class  IwHumanPlane;
struct wsInitData;
struct customUnitProperty;
struct wsPlaneJump;
class  Position3;
struct lua_State;
namespace Graphics
{
	class ModelParser;
	class VolumeEffect;
}
namespace Lua
{
	class Config;
}
namespace AI
{
	class wPlayerTaskIndicator;
}
#pragma endregion


namespace cockpit
{
	class ccDrawable;
	class ccCockpitContext;
	class SimpleOctTree;
	class ccMainPanel;
	class avLinkToTargetResponder;

	enum avUnique_enum
	{
		unique_Helmet,
		unique_Padlock,
		unique_SIZE_MAX
	};


	class  COCKPITBASE_API  devices_keeper
	{
		friend class ccCockpitContext;

		typedef ed::map<unsigned char,avDevice*>				 avDevicesByID;
		typedef ed::map<unsigned char,ed::string>				 avDevicesNamesByID;
		typedef ed::unordered_map<ed::string, avDevice*>		 avDevicesByName;
		avDevicesByID			by_id;
		avDevicesNamesByID		names_by_id;
		avDevicesByName			by_name;
		bool					initialized;
		avDevice *				uniques[unique_SIZE_MAX];
	public:
		devices_keeper();
		~devices_keeper(){ clear(); }
		void clear();
		void start();

		void load_names(Lua::Config & config);
		bool is_initialized() const { return initialized; }
		
		void add(avDevice * device);

		void clear_type_singletons();

		void link_all();
		void unregister(avDevice * dev);

		avDevice * get(unsigned char ID);
		avDevice * get(const ed::string & name);

		avDevice * get_type_singleton(unsigned char singletion_type)
		{
			return uniques[singletion_type];
		}
		void	   set_as_type_singleton(avDevice * device,unsigned char singletion_type);

		avDevice * get_link(avDevice * dev,const ed::string & link_var_name);


		void	   external_parse(Graphics::ModelParser& parser,const Position3& pos,HeapVector<Graphics::VolumeEffect*> * effects);

	};

	class  COCKPITBASE_API  indicators_keeper // to store and release independent indicators
	{
		friend class ccCockpitContext;

	public:
		 indicators_keeper(){};
		~indicators_keeper() { clear();};
		void clear();
		void start();
		void add(ccDrawable * indicator);
		void link_all(devices_keeper & devices);

		ccDrawable * get(unsigned i)
		{
			if (i < by_order.size())
				return by_order[i];
			else
				return NULL;
		}
		ed::vector<ccDrawable*> by_order;
	};


	enum contexts_e
	{
		CC_GENERAL_AIRCRAFT,
		CC_GROUND_UNIT,
		CC_INDEPENDENT,
		CC_MAX_CONTEXTS
	};


	class COCKPITBASE_API ccContextRelatedObject : public Common::FakeUnknown<Serializable>
	{
	public:
		ccContextRelatedObject():context(NULL){}
		inline ccCockpitContext * get_context() const { return context; };
	private:
		ccCockpitContext * context;
		friend class	ccCockpitContext;
	};


	enum ccCptInitConditions
	{
		CPT_INCON_COLD_GROUND,
		CPT_INCON_HOT_GROUND,
		CPT_INCON_HOT_AIR
	};

	class COCKPITBASE_API avInitData 
	{
		wsInitData			* unit_init_data;
		void init_birthplace(unsigned birthplace);
		void copy(wsInitData * unit_data);
		//forbidden copy
		avInitData(const avInitData& that);
		avInitData& operator = (const avInitData& that);
	public:
		avInitData();
		~avInitData();
		void set(wsInitData*);
		void set(wsPlaneJump*);

		ccCptInitConditions cpt_init_condition;
		double			    init_fuel_weight;

		const customUnitProperty * get_custom_unit_property(const ed::string & name) const;
		void				save_in_state(lua_State *L);
	};

	class COCKPITBASE_API ccCockpitContext
	{
		static ccCockpitContext	 contexts[CC_MAX_CONTEXTS];
        static ccCockpitContext* activeHumanContext;


		friend	ccCockpitContext & aircraft();
		friend	ccCockpitContext & independent(); 
		friend	ccCockpitContext & ground_unit();
        friend  ccCockpitContext & human_unit();

		void	 update_active_shift();

		Vector3  active_shift;

		ccCockpitContext();
	public:
		~ccCockpitContext();

		//initialization
		bool					 init_unit		 (MovingObject * _human,const char * custom_name = NULL);
		bool					 create_all_from (Lua::Config & config);
		avDevice   *			 create_device   (unsigned char ID,Lua::Config& config);
		ccDrawable *			 create_indicator(Lua::Config& config);
		//factory method
		ccContextRelatedObject * create(const ed::string & ClassName);

		void					 release();
		void					 start();

		void					 reassign_device(unsigned char ID,Lua::Config & config,SimpleOctTree * elementsTree);

		Position3				 get_cockpit_orientation();
		Position3				 get_cockpit_render_position_local();
		const Vector3 &			 get_dynamic_shift() const { return active_shift; }
		Vector3					 get_dynamic_shift_unprocessed();

		Position3				 get_cockpit_render_position()
		{
			return get_cockpit_render_position_local() * get_cockpit_orientation();
		}

		void	set_argument(int index,float value = 0);
		float	get_argument(int index);

		void	draw_update();
		void	draw();
		void	parse_for_render(Graphics::ModelParser & parser);
		void	draw_screenspace();

	public:
		IwHumanPlane*		human;
		woPointer			human_ptr; 
		devices_keeper		devices;
		indicators_keeper	indicators;
		avInitData			init_conditions;

		ccMainPanel				 *panel;
		avLinkToTargetResponder  *link_to_target;
		AI::wPlayerTaskIndicator *task_handler;
		bool				active;
		bool				initialized;
	};


	inline ccCockpitContext & aircraft()	 { return ccCockpitContext::contexts[CC_GENERAL_AIRCRAFT]; }
	inline ccCockpitContext & independent()  { return ccCockpitContext::contexts[CC_INDEPENDENT]; }
	inline ccCockpitContext & ground_unit()  { return ccCockpitContext::contexts[CC_GROUND_UNIT]; }
    inline ccCockpitContext & human_unit()   { return *ccCockpitContext::activeHumanContext; }
}
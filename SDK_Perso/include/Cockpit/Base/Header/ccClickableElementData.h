#pragma once

#include "Utilities/ccSimpleOctTree.h"
#include "Utilities/ccUtilities.h"

namespace Graphics
{ 
    class Geometry;
    class ModelInstance;
    class ModelConnector;
}

namespace cockpit
{
	enum cockpit_action_number
	{
		PRIMARY_ACTION = 0,
		SECONDARY_ACTION,
		THIRD_ACTION,	// Используется только в сочетании - 1 ось + две других команды
		TYPES_NUMBER
	};

	enum cockpit_control_type
	{
		NULL_TYPE = 0,
		BUTTON_TYPE,
		TUMBLER_TYPE,
		SWINGBUTTON_TYPE,
		AXIS_TYPE,
		MOVABLE_AXIS_TYPE
	};

	class avDevice;
	class clickableElementActionData;

	typedef ed::vector<clickableElementActionData*> ActionDataList;

	class clickableElementData : public SimpleOctTreeLoad
	{
		ed::list<clickableElementData*>  automatically_updated;
	public:		//Common data
		clickableElementData(const char *name = 0);
		~clickableElementData();

		bool l_read_actions(Lua::Config& config);

		void update_with_me(clickableElementData * child)
		{
			automatically_updated.push_back(child);
		}

    public:
        COCKPITBASE_API clickableElementActionData* getActionData ( unsigned char action );

        COCKPITBASE_API const clickableElementActionData* getActionData ( unsigned char action ) const;

		bool is_movable_or_axis(unsigned char action) const;
		bool is_axis(unsigned char action) const;

		COCKPITBASE_API void update();
		void setOrientedBox(const Box & new_OBB,const Position3 & new_position);

		COCKPITBASE_API void setHint (const ed::string & new_hint);

		COCKPITBASE_API const ed::string& getHint() const 
		{
			return hint_text;
		}

		const ed::wstring& getHintUnicode() const 
		{
			return hint_text_wchar;
		}

		void setCallback(avDevice * dev);
		float get_distance_to_OBB(const Vector3& start, const Vector3& end);

		COCKPITBASE_API void   reg_in_script(lua_State * L);
	protected:
		
		bool check_segment(const Vector3 &start,const Vector3 &end);

	public:
		ed::string			 shape_name;
		Vector3              normal; 
		Position3            position;
		bool                 use_OBB;
		Box                  OBB; //oriented bounding box
		bool				 updatable;
		Graphics::Geometry*  shape_interface;
		Graphics::ModelConnector*      shape_helper;
		bool				 is_custom;
		unsigned char        DeviceID;// если МФД или HUD то необходим ID ,если нет тогда DeviceID = 0

		ActionDataList		action_data;

		ed::string          hint_default;
		double              last_time;
		bool                cycle;

	protected:
		ed::string hint_text;
		ed::wstring hint_text_wchar;
	};

	typedef ed::unordered_map<ed::string,clickableElementData> clickMap;
	typedef clickMap::iterator						   clickItem;
	typedef ed::map<int, clickableElementData*>       callbackMap;

}

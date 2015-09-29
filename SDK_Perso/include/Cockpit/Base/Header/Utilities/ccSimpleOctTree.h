#pragma once

#include "CockpitBase.h"

#include "GraphMath\box.h"
#include "ClipVolumes\ClipVolume.h"
#include "GraphMath/position.h"
#include <ed/map.h>
#include <ed/unordered_map.h>
#include <ed/list.h>
#include <ed/string.h>
#include "Mem\Shared.h"

namespace cockpit
{
	class SimpleOctTreeNode;

	class COCKPITBASE_API SimpleOctTreeLoad : public Mem::Shared
	{
	public:
		SimpleOctTreeLoad():node(0),is_hidden(false) {}
		virtual ~SimpleOctTreeLoad();
        const Box& getBoundingBox() const { return AABB;}
		void       setBoundingBox(const Box & new_AABB);
        void       hide(bool value_in = true) { is_hidden = value_in;}
        bool       hidden() const { return is_hidden; }
	protected:
		Box        AABB;//axis allied bounding box
		virtual bool	   check_segment(const Vector3 &start,const Vector3 &end);
		bool	           check_volume (ClipVolume* volume);
		void               remove_from_node();
	private:
		void               change_position();
		friend class SimpleOctTreeNode;
		friend class SimpleOctTree;
		SimpleOctTreeNode   * node;
        bool       is_hidden;
	}; 
	
	struct COCKPITBASE_API SimpleOctTreeNodePreferences
	{	
		SimpleOctTreeNodePreferences()
		{
			tree_min_cell     = 50.0f;
			node_max_objects  = 5;
			node_min_objects  = 2;
			octant			  = true;
			max_subdivision_level = 50;
			root			  = 0;
		};	
		inline int	cell_count() const { return (octant) ? 8 : 4;}
		float tree_min_cell;
		int   node_max_objects;
		int   node_min_objects;
		bool			octant;
		unsigned char   max_subdivision_level;
		const SimpleOctTreeNode  * root;

		static SimpleOctTreeNodePreferences DUMMY;
		static const int tree_per_cell = 2;
	};

	class COCKPITBASE_API SimpleOctTree_check_object
	{
	public:
		SimpleOctTree_check_object():final(false){}
		virtual bool valid(SimpleOctTreeLoad *obj) { return true;}
		void         finish(){ final = true;}
		bool         finished() { return final;};
	private:
		bool         final;
	};
	typedef SimpleOctTreeLoad							Object__;
	typedef Mem::Ptr <Object__>							SimpleOctTreeLoadPtr;
	typedef ed::unordered_map<ed::string,SimpleOctTreeLoadPtr>	SimpleNamedMap;
	typedef cockpit::SimpleOctTreeNode/*<Object__>*/    SimpleNodeInstance;
	typedef ed::list<SimpleOctTreeLoadPtr>				SimpleContentPtrList;
	typedef ed::list<Object__*>						SimpleContentList;


	/*template <class Object__>*/ 
	class COCKPITBASE_API SimpleOctTreeNode 
	{
	public:
		 SimpleOctTreeNode(SimpleOctTreeNode  *_parent, short _ix, short _iy, short _iz);
		 SimpleOctTreeNode(SimpleOctTreeNodePreferences *_preferences,const Vector3 &_min ,const Vector3 &_max);
	    ~SimpleOctTreeNode();
		 void				   GetObjectList(ClipVolume* volume,SimpleContentPtrList &target,SimpleOctTree_check_object *checker = 0);
		 void				   GetObjectList(SimpleContentPtrList &target,SimpleOctTree_check_object *checker = 0);
		 void                  Walk(SimpleOctTree_check_object & checker,ClipVolume* volume = 0);
		 bool				   AddObject(Object__ *obj);
		 bool				   RemoveObject(Object__ *obj);
		 bool				   EraseObject(Object__ *obj); // erases from THIS node objs, returns true if found
		 void				   ChangedPos(Object__ *obj);
		 unsigned			   GetCount() const {return objCount;};
		 SimpleOctTreeLoadPtr IntersectSegment(const Vector3 &start,const Vector3 &end,SimpleOctTree_check_object *checker = 0);
		 void				   DeleteAll();
		 void                  debug_draw_tree(const Position3 & origin,bool nodes);
		 void                  debug_draw_tree_reverse(const Position3 & origin);
	protected:
		 bool NeedsDivision  (void); // many objects and size more than MinCell
		 bool NeedsCollapsing(void); // less than MinObjects
		 void Subdivide(void); // create children and distribute objects among them
		 void Collapse(bool getObjects = false); // destroy children and get their objects 
		 int  GetChild(Object__ *obj);
		 void InsertObject(Object__ *obj); // actually inserts in THIS node objs
		 int  Intersect(ClipVolume *volume);
		 bool IsContain (Object__ *obj);

		 void insert_box(const Box & inserted_box);
	
	public:
		Box	                                   box;
		float                              max_len;	// дополнительный припуск со всех сторон - максимальный радиус объекта в нем
		int                               objCount;	// Полное число объектов
		unsigned char			          ix,iy,iz; // in parent's child array
		SimpleOctTreeNode		           *parent;
		SimpleOctTreeNode                  **child; // NULL=>collapsed, else contains MAP_TREE_PER_CELL^2 children
		SimpleContentPtrList                  objs;
		SimpleOctTreeNodePreferences  *preferences;
		unsigned char            subdivision_level;
	};

	/*template <class Object__>*/ 
	class COCKPITBASE_API SimpleOctTree 
	{
	public:
		SimpleOctTree(Box const& bounds,
					   float world_tree_min_cell  = 50.0f,
					   int   node_max_objects= 5,
					   int   node_min_objects = 2) ; 

		 static SimpleOctTree * create_quad_tree  (Box const& bounds,float world_tree_min_cell  = 50.0f);
		 static SimpleOctTree * create_octant_tree(Box const& bounds,float world_tree_min_cell  = 50.0f);

		 SimpleOctTree();
		 ~SimpleOctTree(void);
		 void				   DeleteAll();
		 bool				   AddObject   (Object__   *obj);
		 bool				   RemoveObject(Object__   *obj);
		 void				   GetObjects  (ClipVolume* volume,SimpleContentPtrList &target,SimpleOctTree_check_object *checker = 0);
		 void				   GetObjects  (SimpleContentPtrList &target,SimpleOctTree_check_object *checker = 0);
		 void				   Walk        (SimpleOctTree_check_object & checker,ClipVolume* volume = 0);
		 unsigned			   GetCount() const;
		 bool				   AddObject   (const ed::string & name,Object__   *obj);
		 bool				   RemoveByName(const ed::string & name);
		 SimpleOctTreeLoadPtr GetByName   (const ed::string & name);
		 void				   Initialize();
		 void				   Initialize(Box const& bounds);
		 SimpleOctTreeLoadPtr  IntersectSegment(const Vector3 &start,const Vector3 &end,SimpleOctTree_check_object *checker = 0);

		 void                  debug_draw_tree(const Position3 & origin,bool nodes);
		 void                  debug_draw_tree_reverse(const SimpleOctTreeLoadPtr   & obj,const Position3 & origin);
	protected:
		Box									  box;
		SimpleNodeInstance*		             root;
		SimpleNamedMap                map_by_name;
		SimpleOctTreeNodePreferences preferences;
	};
}

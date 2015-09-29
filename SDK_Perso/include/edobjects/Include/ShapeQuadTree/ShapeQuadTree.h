#ifndef MapObjectsTree_H
#define MapObjectsTree_H

#include <ed/map.h>

#include "ClipVolumes/IntersectionType.h"

// 2 or 4 => determine later (2x2 or 4x4 branches in each node)
#define MAP_TREE_PER_CELL     2
#define MAP_CELL_COUNT        (MAP_TREE_PER_CELL*MAP_TREE_PER_CELL)
#define MAP_TREE_MIN_CELL     100 // in meters
#define MAP_NODE_MAX_OBJECTS  10 // if more than that => subdivide
#define MAP_NODE_MIN_OBJECTS  2  // if less than that => collapse

class viObjectFlatShape;
class viSearch;

class EDOBJECTS_API ShapeQuadTreeNode 
{
protected:
    bool NeedsDivision(void); // many objects and size more than MinCell
    void Subdivide(void); // create children and distribute objects among them
    
    bool NeedsCollapsing(void); // less than MinObjects
    void Collapse(bool getObjects = true); // destroy children and get their objects 

    IntersectionType IsInCell(viObjectFlatShape *obj);

    // these also must create crossreferences in obj
    void InsertObject(viObjectFlatShape *obj, bool large); // actually inserts in THIS node objs
    bool EraseObject(viObjectFlatShape *obj); // erases from THIS node objs, returns true if found
public:
	float xMin, xMax;
	float zMin, zMax;
	float yMin, yMax;

	// Полное число объектов
	int objCount; // including children 

	unsigned char ix,iz; // in parent's child array

	ShapeQuadTreeNode *parent;
	ShapeQuadTreeNode **child; // NULL=>collapsed, else contains MAP_TREE_PER_CELL^2 children

	struct objDesc
	{
		viObjectFlatShape *obj;
		bool large;
	};

	// objects in this cell
	typedef ed::map<ObjectID, objDesc> objsType;
	objsType objs;

	ShapeQuadTreeNode(float _xMin, float _zMin, float _xMax, float _zMax); // for root only
	ShapeQuadTreeNode(ShapeQuadTreeNode *_parent, short _ix, short _iz);
	~ShapeQuadTreeNode(void);

	unsigned GetCount(void)	{return objCount;}
	void GetStatistics(int *objects, int *nodes, int *references, int *maxLevel, int *maxObjectPerNode, float *minNodeSize);

	// returns object count 
	bool GetObjectList(viSearch *search, IntersectionType it = itIntersects); // возвращает true - можно продолжать поиск

	// per object functions
	void AddObject(viObjectFlatShape *obj, bool large = false); // large => contains cell
	bool RemoveObject(viObjectFlatShape *obj); // returns true if found

	// Распечатать содержимое в файл
	void PrintToFile(FILE *f, int level);
};

#endif // MapObjectsTree

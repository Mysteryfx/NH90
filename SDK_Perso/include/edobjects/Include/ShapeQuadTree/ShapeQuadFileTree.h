#ifndef SHAPEQUADTREEFILE_H
#define SHAPEQUADTREEFILE_H

#include "Registry\Registered.h"
#include "ShapeQuadTree.h"

class ClipVolume;
class viObjectFlatShape;

// для приблизительного поиска
typedef bool (*viApproximateHandler)(ObjectID id, void *data);

class EDOBJECTS_API ShapeQuadFileTreeNode 
{
private:
	
//	bool NeedsDivision(void); // many objects and size more than MinCell
//	void Subdivide(void); // create children and distribute objects among them
	
//	bool NeedsCollapsing(void); // less than MinObjects
//	void Collapse(bool getObjects = true); // destroy children and get their objects 

//	IntersectionType IsInCell(viObjectFlatShape *obj);

	// these also must create crossreferences in obj
//	void InsertObject(viObjectFlatShape *obj, bool large); // actually inserts in THIS node objs
//	bool EraseObject(viObjectFlatShape *obj); // erases from THIS node objs, returns true if found
public:
	float xMin, xMax;
	float zMin, zMax;
	float yMin, yMax;

	unsigned short ix,iz; // in parent's child array

	// Полное число объектов
	int objCount; // including children 

	// смещение на папу
	unsigned int offset_Parent;
	// смешение на массив смещений на детей
	unsigned int offset_ChildsArray;
	// Ссылка на объект
	typedef std::pair<ObjectID, bool> LinkToObject;
	// Число объектов в массиве
	unsigned int local_objectcount;
	// смещение на массив структур объектов
	unsigned int offset_ObjectList;
	

	ShapeQuadFileTreeNode* getParent()
	{
		if(!offset_Parent) return NULL;
		return (ShapeQuadFileTreeNode*)((char*)this + offset_Parent);
	}
	ShapeQuadFileTreeNode* getChild(int index)
	{
		if(!offset_ChildsArray) return NULL;
		if(index>=MAP_CELL_COUNT) return NULL;

		unsigned int* pArray = (unsigned int*)((char*)this + offset_ChildsArray);
		return (ShapeQuadFileTreeNode*)((char*)this + pArray[index]);
	}
	LinkToObject* getObject(int index)
	{
		if(index>=(int)local_objectcount) return NULL;
		LinkToObject* pLinkToObject = (LinkToObject*)((char*)this + offset_ObjectList);
		return pLinkToObject+index;
	}
	


	// objects in this cell
	//typedef ed::map<viObjectFlatShape *, bool> objsType; // map<Object, IsLarge>
//	typedef ed::list< std::pair<viObjectFlatShape *, bool> > objsType;
//	objsType objs;
	
//	ShapeQuadTreeNode(float _xMin, float _zMin, float _xMax, float _zMax); // for root only
//	ShapeQuadTreeNode(ShapeQuadTreeNode *_parent, short _ix, short _iz);
//	~ShapeQuadTreeNode(void);

	unsigned GetCount(void) {return objCount;}
	void GetStatistics(int *objects, int *nodes, int *references, int *maxLevel, int *maxObjectPerNode, float *minNodeSize);

	// returns object count 
	bool GetObjectList(viSearch *search, IntersectionType it = itIntersects); // возвращает true - можно продолжать поиск

	// приблизительный поиск, без создания объектов
	int GetObjectList(ClipVolume *volume, viApproximateHandler handler, void *data, IntersectionType it = itIntersects);
	
	// per object functions
//	void AddObject(viObjectFlatShape *obj, bool large = false); // large => contains cell
//	bool RemoveObject(viObjectFlatShape *obj); // returns true if found
	
	// Распечатать содержимое в файл
	void PrintToFile(FILE *f, int level);
};

#endif // MapObjectsTree

#pragma once

#include "lTerraBasic.h"

#include "Registry/Registered.h"
#include "Registry/RegisterManager.h"
#include "ShapeQuadTree\ShapeQuadFileTree.h"
#include "ShapeQuadTree\ShapeQuadStorage.h"
#include "LandObjectStorage\SceneLinkNode.h"
#include "misc/misc_mmf.h"
#include "misc/misc_mmf_wr.h"
#include "misc/misc_vector.h"
#include "misc/misc_ptr.h"

#pragma pack(1)

class ShapeQuadTreeNode;

namespace BinarySceneAndTree
{
	const unsigned int current_version = 3;
	const char current_filetype[4] = {'b', 's', 'f', ' '};

	class TreeNode;

	// Служебная 
	struct WriteNodeStr
	{
		TreeNode* dst;
		ShapeQuadTreeNode* src;
		WriteNodeStr(TreeNode* dst, ShapeQuadTreeNode* src)
		{
			this->dst = dst;
			this->src = src;
		}
	};
	typedef ed::list< WriteNodeStr> tag_WriteNodeList;

	// Эл.дерева
	class EDOBJECTS_API TreeNode 
	{
	public:
		// Бокс
		float xMin, xMax;
		float zMin, zMax;
		float yMin, yMax;

		// in parent's child array
		unsigned short ix,iz; 

		// Полное число объектов
		int objCount; // including children 

		// смещение на папу
		misc::ptr<TreeNode> Parent;
		// дети
		misc::vector< misc::ptr< TreeNode> > ChildsArray;
		// Ссылка на объект
		typedef std::pair<ObjectID, bool> LinkToObject;
		// Объекты
		misc::vector< LinkToObject> ObjectList;

	// Доступ
	public:
		TreeNode* getParent(){return Parent;}
		TreeNode* getChild(int index)
		{
			if( index<0 || index>=(int)ChildsArray.size())
				return NULL;
			return ChildsArray[index];
		}
		LinkToObject* getObject(int index)
		{
			if( index<0 || index>=(int)ObjectList.size())
				return NULL;
			return &ObjectList[index];
		}
		// Число объектов		
		unsigned GetCount(void) {return ObjectList.size();}
		// ???
		void GetStatistics(int *objects, int *nodes, int *references, int *maxLevel, int *maxObjectPerNode, float *minNodeSize);
		// returns object count 
		bool GetObjectList(viSearch *search, IntersectionType it = itIntersects); // возвращает true - можно продолжать поиск
		// приблизительный поиск, без создания объектов
		int GetObjectList(ClipVolume *volume, viApproximateHandler handler, void *data, IntersectionType it = itIntersects);
		// Распечатать содержимое в файл
		void PrintToFile(FILE *f, int level);

	public:
		TreeNode(
			TreeNode* parent, 
			ShapeQuadTreeNode* src, 
			tag_WriteNodeList& WriteNodeList, 
			misc::mmf_write& outfile);
	};

	// Тип объекта
	struct ObjectType
	{
		char type[32];
	};
	// Объект
	struct Object
	{
		cPosition pos;
		misc::ptr<ObjectType> type;
	};

	// Объект элемент структуры (дб задано либо name, либо id)
	// 
	struct LinkNode
	{
		// Тип элемента
		LinkNodeType type;
		// Имя элемента (мб NULL)
		misc::ptr<char> name;
		// Ид. элемента (мб -1)
		ObjectID id;

		// Исходящие ссылки, типа объект-...
		misc::vector< misc::ptr<LinkNode> > out_links;
		// Входящие ссылки, типа ...-объект
		misc::vector< misc::ptr<LinkNode> > in_links;

		bool IsId(){return type==LNT_OBJECT;}
		bool operator <(const LinkNode& arg)const;
	};



	// Файл
	class EDOBJECTS_API File
	{
		// Тип файла
		char filetype[4];
		// Версия 
		unsigned int version;				
	public:
		// Границы выгр. ид. (ид. первого объекта)
		ObjectID startId;
		ObjectID endId;
		// Бокс
		float xMin, xMax;
		float zMin, zMax;
		// Тип
		viFlag types;

	public:
		// Список типов
		misc::vector<ObjectType> objecttypes;
		// Список объектов
		misc::vector< misc::ptr<Object> > objects;
	public:
		// Корень дерева
		misc::ptr<TreeNode> rootnode;
	public:
		// Ссылочная целостность между объектами
		// Объекты могут ссылаться друг на друга и на именованые закладки
		misc::vector<LinkNode> links; // граф ссылок

	/////////////////////////////////////
	// Ф-кции доступа
	public:
		// Открыть файл
		static File* Open(const char* filename, misc::mmf& file);

		// Интервал ид-ров
		void GetIdInterval(
			ObjectID& startId,			// Границы выгр. ид.
			ObjectID& endId);
		// Объект
		Object* getObject(ObjectID id);

		// Позиция и тип объекта
		cPosition* getObjectById(
			ObjectID id, 
			char*& type);

		// Объект в графе ссылок
		LinkNode* getLinkNode(const LinkKeyNode& key);
		// Объект в графе ссылок
		LinkNode* getLinkNode(ObjectID id);
		// Именованый Объект в графе ссылок
		LinkNode* getLinkNode(LinkNodeType type, const char* name);

	/////////////////////////////////////
	// Ф-кции создания
	public:
		File();

		// Записать в файл
		static File* BuildFile(
			RegisterManager* registry,				// Реестр
			ShapeQuadStorage& theShapeQuadStorage,  // Дерево
			tag_LinkDeclList& Links,				// Список связей
			ObjectID startId,						// Границы выгр. ид.
			ObjectID endId,				
			misc::mmf_write& file);

	protected:
		// Сохранить сл. уровень
		static void WriteNodeChilds( 
			TreeNode* dst, 
			ShapeQuadTreeNode* src, 
			tag_WriteNodeList& WriteNodeList, 
			misc::mmf_write& outfile);
	};
}

#pragma pack()

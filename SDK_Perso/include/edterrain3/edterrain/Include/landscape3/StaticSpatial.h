#ifndef _STATICSPATIAL_H
#define _STATICSPATIAL_H
#include "../landscape3_config.h"
#include "lTerraBasic.h"

namespace scene3
{

// дерево для хранения в mmf
struct StaticSpatial
{
	//////////////////////////////////////////////////
	// дерево:
	struct TreeNode
	{
		float xMin, xMax;
		float zMin, zMax;
		float yMin, yMax;

		// Полное число объектов
		uint32_t objCount; // including children
		// Число объектов в массиве
		uint32_t local_objectcount;

		// индекс в массисе ObjectID
		uint32_t ObjectID_index;

		// смешение на массив смещений на детей, м.б. -1
		int32_t offset_ChildsArray;
	};

	// nodes
	io::mmfvector<TreeNode> nodes;
	// массив child индексов TreeNode, суда ссылается TreeNode::offset_ChildsArray
	io::mmfvector<int32_t> ChildNodes;
	// массив ид, суда ссылается TreeNode::ObjectID_index
	io::mmfvector<uint32_t> ObjectIDs;
};

}

#endif
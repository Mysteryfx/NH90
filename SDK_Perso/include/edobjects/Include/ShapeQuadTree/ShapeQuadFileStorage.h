#ifndef ShapeQuadFileStorage_h
#define ShapeQuadFileStorage_h

#include "cLinear.h"
#include "viObjectStorage.h"
#include "lTerraBasic.h"

#include "ShapeQuadFileTree.h"
#include "ShapeQuadStorage.h"
#include "misc/misc_mmf.h"
#include "misc/misc_mmf_wr.h"

#include "LandObjectStorage\BinarySceneAndTree.h"

#include <ed/string.h>

class ShapeQuadTreeNode;

// coordinates assume x=north, z=east, y=up
class EDOBJECTS_API ShapeQuadFileStorage : public viObjectStorage
{
private:
	struct Header
	{
		// Вернсия файла
		unsigned int version;
		// Версия исходных данных
		ObjectCollection_Version srcdata_version;
		// ???
		float xMin, xMax;
		float zMin, zMax;
		// Тип
		viFlag types;

		// Ссылка на корень
		unsigned int offset_root;
	
		ShapeQuadFileTreeNode* getRoot()
		{
			if(!offset_root) return NULL;
			return (ShapeQuadFileTreeNode*)((char*)this + offset_root);
		}
	};
	// Файл
	misc::mmf theMMF;
	// Заголовок
	Header* pHeader;
	// Корень
	ShapeQuadFileTreeNode* root;
	BinarySceneAndTree::TreeNode* root2;

	// Записать построеноое дерево в файл
	static ShapeQuadFileTreeNode* WriteNodeToFile( 
		misc::mmf_write& outfile, 
		ShapeQuadTreeNode* pnode, 
		ShapeQuadFileTreeNode* parentnode);

public:
	// Записать построеноое дерево в файл
	static bool WriteToFile( 
		ShapeQuadStorage& theShapeQuadStorage, 
		ObjectCollection_Version& collection_ver, 
		const char* filename);

	// Найти в каталоге свободное имя файла
	static ed::string FindUnusedFilename( 
		const char* foldername, 
		const char* extention);

    ShapeQuadFileStorage();
    virtual ~ShapeQuadFileStorage();

	// Открыть файл
	// Поиск файла с нужной версией
	bool Open(
		const char* filename,
		ObjectCollection_Version srcdata_version);
	bool Open(
		BinarySceneAndTree::File* file
		);

	// имя типа хранилища
	virtual const char *Name(void);

	// операции с объектами
	virtual bool AddObject(viObject *object);
	virtual bool RemoveObject(viObject *object);

	// выбросить объекты по маске
	virtual void Purge(viFlag mask);
	
	// поиск по структуре поиска
	virtual void GetObjects(viSearch *search);

	// приблизительный поиск, не требует создания объектов
	virtual int GetObjects(ClipVolume *volume, viApproximateHandler handler, void *data);
	
	// выдать количество объектов в хранилище
	virtual unsigned GetCount(void);

	// выдать состояние (статистику)
	virtual void GetStatistics(char buffer[]);

	// Распечатать содержимое в файл
	virtual void PrintToFile(char *filename);
};

#endif // MapObjectsStorage

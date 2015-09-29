#ifndef LandObjectStorage_H
#define LandObjectStorage_H
#ifndef USE_TERRAIN4


#include "viObjectStorage.h"
#include "LandObjectStorage/SceneLinkNode.h"
#include "LandObjectStorage/BinarySceneAndTree.h"
#include "landscape3/lSceneFile.h"

#include "misc/misc_vector.h"
#include "misc/misc_mmf.h"

class ShapeQuadStorage;
class ShapeQuadFileStorage;

// менеджер загрузки и хранения объектов стандартной сцены
class EDOBJECTS_API LandObjectStorage : public viObjectStorage
{
private:
	bool inited;
	static bool locked;
	
	// габариты мира
	Box box;

	// если используется фиксированное дерево
	bool using_fixed;

	ShapeQuadStorage *mut_tree;

    ObjectID lastId_scenefile;

	// Новый файл сцен
    struct SceneDesc
    {
//	    io::MmfStream mmfstreams_scenefile;
	    scene3::lSceneFile* _scenefile;
	    ObjectID startId_scenefile;
    };
    ed::list<SceneDesc> _scenes;

private:
	struct CacheSceneFiles
	{
		ed::string filename;
		misc::mmf file;
		BinarySceneAndTree::File* scenefile;
	};
	ed::list<CacheSceneFiles> cachscenefiles;

protected:
	typedef ed::list< ed::string > stringList;
	
	// получить путь к файлам сцены с учетом настроек детализации
	char *GetScenePath();
	const char *GetSceneINI();
	stringList GetSceneListFromFolder(const char *folder);
	stringList GetSceneListFromINI(const char *file);
	stringList GetScenesList();

	// создание сцены
	void InitRailwayGirder();
	bool LoadMainScene(const stringList& scenes); 
	bool LoadETL(char *folder);
	int  CreateScene();

	// получить версию файлов
	void GetSourceVersion(const stringList& scenes, FILETIME& time);

public:
	LandObjectStorage(const Box &borders);
    virtual ~LandObjectStorage();

	// загрузить (создать) сцену
	// lodObjects = отложенная загрузка объектов стандартной сцены
	// lodTree = использование предгенерации пространственного дерева
	void InitScene(scene3::lSceneFile* scenefile, bool lodObjects = true, bool lodTree = true);

	// сохранить пространственное дерево в файл
	void SaveScene(
		const char* filename, 
		BinarySceneAndTree::tag_LinkDeclList& LinkDeclList);

	// выгрузить все объекты - подготовить к InitScene
	void FinishScene();

	// имя типа хранилища
	virtual const char *Name(void);

	// операции с объектами
	virtual bool AddObject(viObject *object);
	virtual bool RemoveObject(viObject *object);

	// выбросить объекты по маске
	virtual void Purge(viFlag mask);
	
	// поиск по структуре поиска
	virtual void GetObjects(viSearch *search);

	// поиск по объему
	void QueryObjects(ClipVolume *volume, scene3::IQuerySink* pQuerySink);
	
	// выдать количество объектов в хранилище
	virtual unsigned GetCount(void);

	// выдать состояние (статистику)
	virtual void GetStatistics(char buffer[]);

	// возможность добавлять/удалять объекты из этого дерева
	static void Lock();
	static void Unlock();

protected:
	bool lSceneFile_GetObjectList_rec(scene3::lSceneFile *scene, ObjectID startId, scene3::lSceneFile::TreeNode* treenode, viSearch* search, IntersectionType it = itIntersects);

};

#endif // ifndef USE_TERRAIN4
#endif // LandObjectStorage_H

#ifndef viObjectManager_H
#define viObjectManager_H

#include "edObjects.h"

#include "box.h"
#include "cPosition.h"
#include "viSearch.h"
#include "LandObjectStorage/SceneLinkNode.h"
#include <osg/vec3d>
#include <osg/Polytope>
#include <osg/Matrixd>

#ifdef USE_TERRAIN4
#include "viObjectHandle.h"
#endif 

class viObjectFactory;
class ClipVolume;
class viObjectStorage;
namespace model {class IModel;} 

// Функция опред. процент выполнения
typedef void (ProgressCallback)(char* message, float percent);

// интерфейс подсистемы для выборки объектов по объему
class EDOBJECTS_API viObjectManager
{
public:
	static viObjectManager *instance;
	
	// конструктор и деструктор
	static viObjectManager* Create(const Box &world);
	virtual ~viObjectManager(void) {};

	// операции с хранилищами
	virtual void AddContainer(viObjectStorage *storage)=0;
	virtual void DestroyContainer(viObjectStorage *storage)=0;

	// фабрики объектов (используются в CreateObject)
	virtual void AddObjectFactory(viObjectFactory *factory)=0;
	virtual void DestroyObjectFactory(viObjectFactory *factory)=0;

	// поиск по абстрактным объемам (возвращают количество найденных объектов)
	int GetObjects(ClipVolume *volume, viObject **objs, int objMax, viFlag flags = viAny);
	int GetObjects(ClipVolume *volume, viSearchHandler handler, void *data, viFlag flags = viAny);

	// поиск по конкретным объемам (с обработчиком)
	int GetBoxList(const cPoint &sw, const cPoint &ne, viSearchHandler handler, void *data, viFlag flags = viAny);
	int GetCameraList(cPosition &cam, float len, float angle, viSearchHandler handler, void *data, viFlag flags = viAny, float aspect = float(4.f/3.f));
	int GetSegmentList(const cPoint& a, const cPoint& b, viSearchHandler handler, void *data, viFlag flags = viAny);

	// поиск по конкретным объемам (с массивом возврата)
	int GetBoxList(const cPoint &sw, const cPoint &ne, viObject **objs, int objMax, viFlag flags = viAny);
	int GetCameraList(cPosition &cam, float len, float angle, viObject **objs, int objMax, viFlag flags = viAny);
	int GetSegmentList(const cPoint& a, const cPoint& b, viObject **objs, int objMax, viFlag flags = viAny);
	
	// поиск объектов по структуре поиска
	virtual void GetObjects(viSearch *search) = 0;

	// Сохранить сцены в бинарные файлы
	virtual bool SaveScene(
		const char* filename, 
		ProgressCallback* callback, 
		BinarySceneAndTree::tag_LinkDeclList& LinkDeclList) = 0;
	// Загрузить сцены из бинарных файлов
	virtual int LoadScene(
		ObjectID districtstartId, 
		ProgressCallback* callback
		) = 0;
	// выгрузить все сцены
	virtual int UnloadScene(
		) = 0;

	/*/
	// Загрузить сцену кварталов
	virtual bool LoadDistrictScene(
		const char* filename,			// имя файла кварталов
		const char* blockmodelsdir,		// каталог с моделями кварталов
		ObjectID startId
		) = 0;
	/*/

	// Создать объект соответствующего типа
	// Для выбора нужного класса используются фабрики объектов 
	virtual viObject* CreateObject(
		const char* type, 
		cPosition& pos, 
		viFlag status = viUnknown,
		void* context=NULL)=0;
	// Вернуть id Объекта (нужно для нелинковки с edObject.dll)
	virtual ObjectID GetObjectID(viObject* obj)=0;
	// Найти связанные объекты
	virtual int GetLinkedObjects(
		const BinarySceneAndTree::LinkKeyNode& key,
		viObject **objs, int objMax, bool bInLinks)=0;

	// Найти объект по координатам и типу
	virtual viObject* FindObject(
		const char* type, 
		float x, float z)=0;

	// Найти высоту точки (и нормаль), с учетом находящихся в этой точке объектов (крыш)
	virtual float getHeightWithObjects(float x, float z, viFlag mask, cVector *n = 0,viObject * excludeMe = nullptr) = 0;

	// выбросить объекты из хранилищ по маске
	// хранилища имеют право не выбрасывать объекты - это только для оптимизации
	virtual void Purge(viFlag mask = viAny) = 0;

	// выдать статистику по хранилищам
	virtual void PrintStats(void) = 0;

	// получить специализированное хранилище для данного типа объектов
	virtual viObjectStorage *GetStorage(viFlag type) = 0;

	// вызывается при разрушении любого домика
	virtual void OnObjectDegrade(ObjectID id) = 0;

#ifdef USE_TERRAIN4
	/// Поиск обектов, для нового интерфейса земли TODO replace bool to enum
	/// Запрос объектов сцены
	/// mask - viFlag
	/// usage:
	/// objectManager->QueryObjectsCB(p1, p2, viMapObject, [&](const ISceneObject& sceneObject)
	/// {
	///		sceneObject.getPosition();
	///	    return QOB_CONTINUE;
	/// });
	virtual QueryObjectResult QuerySceneObjectsCB(const osg::Vec3d& pt1, const osg::Vec3d& pt2, viFlag mask, OnQuerySceneObjectCallback func) = 0;
	virtual QueryObjectResult QuerySceneObjectsCB(const osg::Polytope& polytope, viFlag mask, OnQuerySceneObjectCallback func) = 0;
	virtual QueryObjectResult QuerySceneObjectsCB(const osg::BoundingBoxf& bbox, viFlag mask, OnQuerySceneObjectCallback func) = 0;

	/// Возращает viObject* по viObjectHandle
	virtual viObject* getInstance(const viObjectHandle& objId, bool create = false) = 0;

	/// Создать viObject, если обект существует то повторно он не создается а возращается указатель на него
	virtual viObject* createInstance(const viObjectHandle& objId) = 0;

	virtual viObject* getInstance(const ISceneObject& objId, bool create = false) = 0 ;
	virtual viObject* createInstance(const ISceneObject& objId) = 0 ;
#endif 
};

#endif // viObjectManager_H

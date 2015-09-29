#ifndef viObjectStorage_h
#define viObjectStorage_h

#include "viSearch.h"
#include "Common/Link.h"
#include <osg/vec3d>
#include <osg/Polytope>
#include "viObjectHandle.h"

#define STATISTICS_BUFFER_SIZE	1000

// интерфейс для специализированных пространственных хранилищ объектов
class EDOBJECTS_API viObjectStorage : public LinkHost
{
protected:
	viFlag types;
public:
	virtual ~viObjectStorage() {};
	
	// типы объектов, пригодные для хранения
	inline viFlag	CanContain(void)	{return types;}

	// имя типа хранилища
	virtual const char *Name(void) = 0;

	// операции с объектами
	virtual bool AddObject(viObject *object) = 0;
	virtual bool RemoveObject(viObject *object) = 0;

	// выбросить объекты из хранилищ по маске
	// хранилища имеют право не выбрасывать объекты - это только для оптимизации
	virtual void Purge(viFlag mask) = 0;
	
	// поиск по структуре поиска
	virtual void GetObjects(viSearch *search) = 0;

//#ifdef USE_TERRAIN4 
	// TODO тестирование новой земли
	//virtual QueryObjectResult QueryObjectsCB(const osg::Vec3d& pt1, const osg::Vec3d& pt2, OnQueryObjectCallback func){return QueryObjectResult::COMPLETE;} //=0;
	//virtual QueryObjectResult QueryObjectsCB(const osg::Polytope& polytope, OnQueryObjectCallback func){return QueryObjectResult::COMPLETE;} //=0;
//#endif 

	// выдать количество объектов в хранилище
	virtual unsigned GetCount(void) = 0;

	// выдать состояние (статистику)
	virtual void GetStatistics(char buffer[]) = 0;
};

#endif // viObjectStorage_h

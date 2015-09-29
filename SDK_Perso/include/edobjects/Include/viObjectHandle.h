
#pragma once
#include <functional>


// класс временно введен дл€ того чтобы запретить не€вную ковертацию из SceneObjectId, ObjectID тк они могу не совпадать
class viObjectHandle 
{
public:
	friend unsigned int convert_viObjectHandleToObjectID(const viObjectHandle&);
	friend bool operator < (const viObjectHandle& lhs, const viObjectHandle& rhs);
	unsigned int id;

	// INVALID_HANDLE не имеет смысла
	// viObjectHandle не может быть невалдным, если его вернул QueryObject он всегда валидный по этому нет deafault конструктора
	// ≈сли есть необходимость создать пустой viObjectHandle используйте boost::optional

};

class ObjectHandle{};

namespace model {
	class IModel;
	class IModelParams;
}
// ќбщий интерфейс дл€ viObject и обектов земли
class ISceneObject {
public:
	virtual unsigned int               getObjectID() const = 0;
	virtual osg::Matrixd               getObjectPosition() const = 0;
	virtual model::IModel*             getObjectType() const = 0;
	virtual const model::IModelParams* getModelParams() const = 0;
};


enum class QueryObjectBehavior {
	QOB_CONTINUE,
	QOB_FINISH
};

enum class QueryObjectResult {
	COMPLETE, 
	CANCELED
};

typedef std::function<QueryObjectBehavior (viObjectHandle objectId)> OnQueryObjectCallback;
typedef std::function<QueryObjectBehavior(const ISceneObject& objectId)> OnQuerySceneObjectCallback;

inline bool operator < (const viObjectHandle& lhs, const viObjectHandle& rhs){
	return lhs.id < rhs.id;
}

inline unsigned int convert_viObjectHandleToObjectID(const viObjectHandle& objId){
	return objId.id;
}

inline viObjectHandle convert_ObjectIDToviObjectHandle(unsigned int id){
		viObjectHandle hnd = {id};		
		return hnd;	 
}
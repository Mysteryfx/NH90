#ifndef ModelConnector_H
#define ModelConnector_H

#include "position.h"
#include "dPosition.h"

namespace osg{
	class Vec2f;
	class Vec3f;
	class Vec4f;
	class Matrixf;
}

namespace Graphics
{

class ModelInstance;

// Интерфейс для разъемов модели
class ModelConnector
{
public:
	virtual ~ModelConnector(){}

	// получить имя соединения
	virtual const char* GetConnectorName() = 0;

	// получить, присутствует ли разъем в модели в текущей конфигурации параметров
	virtual bool IsConnectorVisible(ModelInstance &object) = 0;

	// получить координаты соединения
	// камеру можно передать любую
	virtual const Position3& GetConnectorPos(ModelInstance &object) = 0;
    virtual const Position3& GetConnectorPosLocal(ModelInstance &object) = 0;
	
	virtual const dPosition& GetConnectorPosd(ModelInstance &object) = 0;
    virtual const dPosition& GetConnectorPosdLocal(ModelInstance &object) = 0;

	/// Returns value of property with given \param name name.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,Graphics::ModelInstance &mi,float &val)const{return false;}
	/// Returns value of property with given \param name name.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,Graphics::ModelInstance &mi,unsigned int &val)const{return false;}
	/// Returns value of property with given \param name name.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,Graphics::ModelInstance &mi,osg::Vec2f &val)const{return false;}
	/// Returns value of property with given \param name name.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,Graphics::ModelInstance &mi,osg::Vec3f &val)const{return false;}
	/// Returns value of property with given \param name name.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,Graphics::ModelInstance &mi,osg::Vec4f &val)const{return false;}
	/// Returns value of property with given \param name name.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,Graphics::ModelInstance &mi,osg::Matrixf &val)const{return false;}
	/// Returns pointer to string value of property with given \param name name.
	/// Warning: Do not delete given pointer.
	/// Returns false if there is no property with given name, or property type coincides with desired type,
	/// otherwise returns true.
	virtual bool getProperty(const char *name,char **val)const{return false;}
};

} // namespace Graphics

#endif // ModelConnector_H

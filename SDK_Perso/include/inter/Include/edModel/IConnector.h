#ifndef ED_MODEL_ICONNECTOR_H
#define ED_MODEL_ICONNECTOR_H

#include "osg/Matrixd"
#include "Arguments.h"

namespace model
{

class IConnector
{
protected:
	virtual ~IConnector() {}
public:
	/// Returns position of connector.
	/// \param modelPos holds position of model.
	virtual osg::Matrixd getPosition(const osg::Matrixd& modelPos,const Arguments& args,const osg::Matrixd *bones,unsigned int nBones)const=0;
	/// Returns position of connector.
	/// \param modelPos holds position of model.
	virtual void getPosition(const osg::Matrixd& modelPos,const Arguments& args,const osg::Matrixd *bones,unsigned int nBones,osg::Matrixd& pos)const=0;

	/// Returns visibility of connector.
	virtual bool getVisibility(const Arguments& args)const=0;

	/// Returns name of connector.
	virtual const char* getName()const=0;

	virtual bool getProperty(const char* name,const Arguments& args,float& val)const=0;
	virtual bool getProperty(const char* name,const Arguments& args,unsigned int& val)const=0;
	virtual bool getProperty(const char* name,const Arguments& args,osg::Vec2f& val)const=0;
	virtual bool getProperty(const char* name,const Arguments& args,osg::Vec3f& val)const=0;
	virtual bool getProperty(const char* name,const Arguments& args,osg::Vec4f& val)const=0;
	virtual bool getProperty(const char* name,const Arguments& args,osg::Matrixf& val)const=0;
	virtual bool getProperty(const char* name,char** val)const=0;
};
}
#endif

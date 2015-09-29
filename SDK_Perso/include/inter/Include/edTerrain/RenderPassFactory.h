#ifndef inter_edterrain_PassFactory_h__
#define inter_edterrain_PassFactory_h__


#include "Factory.h"
#include "inter.h"
#include "IRenderPass.h"

namespace render {

class RenderPassFactory
{
public:
	INTER_API static RenderPassFactory& instance();
	INTER_API void add(ed::Factory* factory);
	INTER_API void RenderPassFactory::remove(ed::Factory* factory);
	INTER_API IRenderPass* create(const char* id);

private:
	ed::FactoryManager factoryManager;

	RenderPassFactory(){}
};


/*
 * Usage:
 * class YourRenderPass : public render::IRenderPass {};
 * render::RegRenderPass<YourRenderPass> reg("YourRenderPassName");
 */
template <class T>
class RegRenderPass : public ed::Factory
{
public:
	RegRenderPass(const char* name)
	{
		id = name;
		RenderPassFactory::instance().add(this);
	}

	virtual ~RegRenderPass() 
	{
		RenderPassFactory::instance().remove(this);
	}

	virtual int getCount()
	{
		return 1;
	}
	virtual const char* getIdentifier(int index)
	{
		return id;
	}
	virtual const char*	getSystemId(int index)
	{
		return typeid(T).name();
	}
	virtual const char* identify(const char* sysId)
	{
		return id;
	}
	virtual void* create(const char* id)
	{
		return new T;
	}
	virtual void* create(const char* id,void* p)
	{
		return new(p) T;
	}
	virtual size_t getSize(const char* id)
	{
		return sizeof(T);
	}

private:
	const char* id;
};


} // namespace render

#endif // inter_edterrain_PassFactory_h__

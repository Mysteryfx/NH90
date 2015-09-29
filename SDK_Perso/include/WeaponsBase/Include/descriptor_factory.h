#ifndef __DESCRIPTOR_FACTORY_H__
#define __DESCRIPTOR_FACTORY_H__

#include "WeaponsBase.h"
#include <ed/map.h>

class wDescriptorBase;

class WEAPONSBASE_API wDescriptorFactory
{
public:
	typedef wDescriptorBase* (*descriptorCreateFuncPtr)(const ed::string& name);

private:
	wDescriptorFactory();
	~wDescriptorFactory();

public:
	static wDescriptorFactory* instance();

	void registerDescriptorType(const ed::string& name, descriptorCreateFuncPtr func);
	wDescriptorBase* createDescriptor(const ed::string& typeName, const ed::string& name) const;

private:
	// Descriptor types
	typedef ed::map<ed::string, descriptorCreateFuncPtr> DescFactoryMap_;
	DescFactoryMap_ descFactoryMap_;

};

#endif

#ifndef __AMMUNITION_NAME_H__
#define __AMMUNITION_NAME_H__

#include "Registry/Registered.h"

class wDatabaseLoader;

class wINamedAmmunitionDescriptor;

class wINamedAmmunition
{
public:
	virtual const wINamedAmmunitionDescriptor* getNameDescriptor() const = 0;
    
    virtual ObjectID getLauncherId() const = 0;
};

class wINamedAmmunitionDescriptor
{
public:
    virtual const ed::string & getOriginalName() const  = 0;
	virtual const ed::string & getDisplayName() const  = 0;
	virtual const ed::string & getTypeDisplayName() const  = 0;
};

class wNamedAmmunitionDescriptor : public wINamedAmmunitionDescriptor
{
public:
    void load(wDatabaseLoader* loader);

	virtual const ed::string & getOriginalName() const { return originalName_; }
	virtual const ed::string & getDisplayName() const { return displayName_; }
	virtual const ed::string & getTypeDisplayName() const  { return typeName_; }

protected:
    ed::string displayName_, typeName_, originalName_;
};

#endif

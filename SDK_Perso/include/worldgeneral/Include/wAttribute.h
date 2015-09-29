#ifndef __OBJECT_ATTRIBUTE_H__
#define __OBJECT_ATTRIBUTE_H__

#include <ed/string.h>
#include <ed/unordered_map.h>
#include <ed/unordered_set.h>

#include "WorldGeneral.h"

typedef ed::unordered_set<ed::string> AttributesString;

class WORLDGENERAL_API wAttributeSet
{
public:
    wAttributeSet(const ed::string& unit_type);
    bool has_attribute(const ed::string& attribute) const;
    bool has_attribute(const int attribute) const;
    void add_attribute(const ed::string& attribute); // for compatibility purposes, do not use it in new code!
	inline const AttributesString & getAttributesString() { return sCache_; }
protected:
    AttributesString sCache_;
    ed::unordered_set<int> iCache_;
};

class WORLDGENERAL_API wAttributeSetManager
{
public:
    ~wAttributeSetManager();
    static wAttributeSet* get_attribute_set(const ed::string& unit_type);
protected:
    static ed::unordered_map<ed::string,wAttributeSet*> cache_;
};

class WORLDGENERAL_API wAttributeOwner
{
public:
    wAttributeOwner():set(0){};
    bool has_attribute(const ed::string& attribute) const;
    bool has_attribute(const int attribute) const;
    void add_attribute(const ed::string& attribute); // for compatibility purposes, do not use it in new code!
    ed::string unit_type;

	inline const AttributesString & getAttributesString() { return set->getAttributesString(); }

private:
    mutable wAttributeSet* set;
};

#endif

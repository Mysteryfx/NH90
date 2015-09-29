#ifndef __SIMULATION_WIRE_TYPES_H__
#define __SIMULATION_WIRE_TYPES_H__

#include <ed/string.h>

enum eLeadType
{
	LEAD_INTEGER = 0,
	LEAD_BOOLEAN,
	LEAD_DOUBLE,
	LEAD_VECTOR,
	LEAD_QUATERNION,
	LEAD_OBJECT_PTR,
    LEAD_INTEGER_UNSIGNED,
	LEADTYPE_LAST
};

struct wLeadDescription
{
public:
	wLeadDescription(const ed::string& name, eLeadType type) : name_(name), type_(type) {}

	const ed::string& name() const { return name_; }
	const eLeadType type() const { return type_; }

protected:
	ed::string name_;
	eLeadType type_;
};


#endif
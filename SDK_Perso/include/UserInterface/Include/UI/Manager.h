/**
 * @file GenUI/Manager.h
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_MANAGER_H
#define GENUI_MANAGER_H


#include "./Element.h"
#include "./Group.h"

#include <ed/string.h>


namespace UI {


class GENUI_API Manager : virtual public Element {
public:
	/**
	 * Create control by its resource indentifier
	 */
	virtual Element* create(const ed::string& name, const ed::string& params=0, Group* parent=0, int id=0) = 0;

	/**
	 * Destroy control window
	 */
	virtual void destroy(Element* window) = 0;

}; // class Manager

/* COM-like ptr typedef */
typedef Ptr<Manager> IManagerPtr;


} // namespace UI

#endif /* GENUI_MANAGER_H */

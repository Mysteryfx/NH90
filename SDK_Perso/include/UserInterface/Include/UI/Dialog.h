#ifndef GENUI_DIALOG_H
#define GENUI_DIALOG_H


#include "./Element.h"
#include "./Group.h"

namespace UI {

/**
 * base class for all dialogs
 */
class Dialog : private Ptr<Group> {
	typedef Ptr<Group> Base;
public:
		Dialog( UI::Element* dlg ) : Ptr<Group>(dlg) {}
		GENUI_API Dialog( const char* name );
		GENUI_API ~Dialog();
		
	Element* window() const { return Base::ptr(); }

	Element* child( int id ) const { return ptr()->child(id); }

	void show() { ptr()->enable(); }
	void hide() { ptr()->disable(); }

}; // class Dialog


}; // namespace UI

#endif /* GENUI_DIALOG_H */

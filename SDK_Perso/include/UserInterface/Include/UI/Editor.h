/**
 * @file GenUI/Editor.h
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_EDITOR_H
#define GENUI_EDITOR_H


#include "./Element.h"


namespace UI {


/**
 * Text input interface
 */
class GENUI_API Editor : virtual public Element {
public:
	typedef Signal::Notifier<Editor*> Event;

	typedef ed::string Text;

	/**
	 * Get whole text
	 */
	virtual const Text getText() const = 0;

	/**
	 * Set whole text
	 */
	virtual void setText(const Text&) = 0;

	/**
	 * User interaction callback.
	 * (change event raises on user interaction ONLY)
	 */
	Event& changed() { return sig_changed; }
	
	virtual bool checkText() { return true; }

private:
	Event sig_changed;

}; // class Editor

/* COM-like ptr typedef */
typedef Ptr<Editor> IEditorPtr;


} // namespace UI

#endif /* GENUI_EDITOR_H */

/**
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_SWITCH_H
#define GENUI_SWITCH_H


#include "./Element.h"


namespace UI {


/**
 * Abstract switch class.
 * Can be in two states: On & Off.
 */
class GENUI_API Switch : virtual public Element {
public:
	typedef Signal::Notifier<Switch*> Event;

	/**
	 * set current state
	 */
	virtual bool setState(bool state) = 0;

	/**
	 * get current state
	 */
	virtual bool getState() const = 0;

	/**
	 * User interaction callback.
	 */
	Event& changed() { return sig_changed; }

private:
	Event sig_changed;

}; // class Switch

/* COM-like ptr typedef */
typedef Ptr<Switch> ISwitchPtr;


} // namespace UI

#endif /* GENUI_SWITCH_H */

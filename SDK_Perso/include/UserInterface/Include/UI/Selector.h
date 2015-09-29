/**
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_SELECTOR_H
#define GENUI_SELECTOR_H


#include "./Element.h"


namespace UI {


/**
 * Abstract switch class.
 * Can have N states [0, N-1] and at one time be in one of them.
 */
class GENUI_API Selector : virtual public Element {
public:
	typedef Signal::Notifier<Selector*> Event;

	/**
	 * Set selector to specified position.
	 * @return success of the operation
	 */
	virtual bool setPosition(int pos) = 0;

	/**
	 * Get current position of selector.
	 */
	virtual int getPosition() const = 0;

	/**
	 * Set number of possible states to count.
	 */
	virtual bool setPositionRange(int count) = 0;

	/**
	 * Get number of possible states.
	 */
	virtual int getPositionRange() const = 0;

	/**
	 * Allow/disallow specific state(s).
	 * @param allow access status
	 * @param fromState number of first state
	 * @param count number of states to apply status to
	 */
	virtual void allowPosition(bool allow, int from, int count = 1) = 0;

	/**
	 * User interaction callback.
	 */
	Event& changed() { return sig_changed; }

private:
	Event sig_changed;

}; // class Selector

/* COM-like ptr typedef */
typedef Ptr<Selector> ISelectorPtr;


} // namespace UI

#endif /* GENUI_SELECTOR_H */

/**
 *
 * Generic User Interface
 *
 *
 * @file GenUI/Element.h
 *
 * @author Dmitry S. Baikov
 *
 */
#ifndef GENUI_CONTROL_H
#define GENUI_CONTROL_H


#include "./config.h"
#include "./Pointer.h"

#include "Signal/Notifier.h"

#pragma warning (disable: 4250)

namespace UI {


/**
 * The base class for all controls.
 * never use delete on Element* (though you can't do this),
 * since your pointer can refer stub or child object.
 * Element sends a signal when it gets destroyed.
 */
class GENUI_API Element
{
protected:
		Element();
	virtual ~Element();

public:
	typedef Signal::Notifier<Element*> Event;

/* ---=== Events ===--- */
	/**
	 * destruction event notificator
	 */
	Event& destroyed() { return sig_destroyed; }

/* ---=== Methods ===--- */
	/**
	 * Get parent control.
	 */
	virtual Element* parent() const = 0;

	/**
	 * Get id of control
	 */
	virtual int id() const = 0;

	/**
	 * Get child control by id.
	 */
	virtual Element* child(int id) const = 0;

	/**
	 * Bring element into input focus
	 */
	virtual void activate() = 0;

	/**
	 * Flags.
	 */
	enum Flag {
		ReadOnly	= 1,
		Disabled	= 2,
		Visible		= 4
	};

	/**
	 * Set flag's value.
	 */
	virtual void setFlag(Flag flag, bool value) = 0;

	/**
	 * Get flag's value
	 */
	virtual bool getFlag(Flag flag) const = 0;

	/**
	 * Obsolete.
	 */
	void freeze(bool val=true) { setFlag(ReadOnly, val); }
	void unfreeze() { setFlag(ReadOnly, false); }

	void disable(bool val=true) { setFlag(Disabled, val); }
	void enable() { setFlag(Disabled, false); }

	virtual void show(bool val=true) { setFlag(Visible, val); }
	virtual void hide() { setFlag(Visible, false); }

	/**
	 * Set profile (visual settings etc.)
	 */
	virtual void setProfile(const char* name) = 0;

protected:
		Element(const Element&) {}
	void operator = (const Element&) {}

private:
	Event sig_destroyed;

}; // class Element

/* COM-like ptr typedef */
typedef Ptr<Element> IElementPtr;


} // namespace UI

#endif /* GENUI_CONTROL_H */

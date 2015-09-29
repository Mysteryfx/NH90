/**
 * @file GenUI/Button.h
 * @author Dmitry S. Baikov
 */
#ifndef GENUI_BUTTON_H
#define GENUI_BUTTON_H


#include "./Element.h"


namespace UI {


/**
 * Abstract button control
 */
class GENUI_API Button : virtual public Element {
public:
	typedef Signal::Notifier<Button*> Event;

	/* signals */
	Event& clicked() { return sig_clicked; }

private:
	/* signals */
//	Event	sig_pressed,
//			sig_unpressed,
	Event	sig_clicked;

}; // class Button

} // namespace UI

#endif /* GENUI_BUTTON_H */

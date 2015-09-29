/**
 *
 * Generic User Interface
 *
 *
 * @file GenUI/Tuner.h
 *
 * @author Dmitry S. Baikov
 *
 */
#ifndef GENUI_TUNER_H
#define GENUI_TUNER_H


#include "./Element.h"
//#include "./Property.h"

namespace UI {


/**
 * This control implements continious value selector (a knob).
 */
class GENUI_API Tuner : virtual public Element {
public:
	typedef Signal::Notifier<Tuner*> Event;

	/* none of get/set functions raises change event */
	struct Range {
		double min, max;

			Range() {}
			Range(double v) : min(v), max(v) {}
			Range(double _min, double _max) : min(_min), max(_max) {}
	};

	virtual void setRange(const Range&) = 0;
	virtual const Range getRange() const = 0;

	virtual void setValue(double value) = 0;
	virtual double getValue() const = 0;

	virtual void setStep(double step) = 0;
	virtual double getStep() const = 0;

	/**
	 * User interaction callback.
	 */
	Event& changed() { return sig_changed; }

private:
	Event	sig_changed; /*< the state has changed via user interaction */

}; // class Tuner

/* COM-like ptr typedef */
typedef Ptr<Tuner> ITunerPtr;


} // namespace UI

#endif /* GENUI_TUNER_H */

#ifndef USERINTERFACE_PROGRESSBAR_H
#define USERINTERFACE_PROGRESSBAR_H

#include "ControlWithVisibleAreas.h"
#include "SliderButton.h"
#include "Static.h"
#include "UI/Tuner.h"

#include <ed/string.h>

typedef UI::Tuner::Range Range;

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ProgressBarParameters
												: public ControlWithVisibleAreasParameters
{
public:
	ProgressBarParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ProgressBarParameters)
	virtual void    serialize(Serializer &);

	typedef ControlWithVisibleAreasParameters Base;

	ControlParameters progress;
	StaticParameters textPrms;

	void read(const char* configText, int configSize);

	Range			range;
	float			value;
	ed::string		caption;
};


class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ProgressBar
	: public ControlWithVisibleAreas,
	  public UI::Tuner
{
public:
	typedef ControlWithVisibleAreas Base;
	typedef ProgressBarParameters Parameters;

private:	
	
	DECLARE_MESSAGE_TABLE()

	//Оформление
	void					thisArrange(const Parameters& prms);

protected:
    Control*				progress;
	Static*					staticText;
	
	float rate;

	void					init();	
	void					updateProgress();
	void					updateText();
	void					update();

public:
							ProgressBar(const Parameters& params);

							ProgressBar(int tag,
								   float x, float y, 
								   float X, float Y);
	virtual					~ProgressBar();

	virtual void			setCoordinates(float x, float y, float X, float Y);

	// вот он интерфейс:
	// default range is [0,1]
	virtual void			setRange(const Range&);
	virtual const Range		getRange() const;

	// default value is 0, set what you need
	virtual void			setValue(double value);
	virtual double			getValue() const;

	virtual void			setCaption(const ed::string text);
	virtual const char*		getCaption() const;

	// unused.
	virtual void			setStep(double step);
	virtual double			getStep() const;

	//Оформление
	void					arrange(const Parameters& prms);
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)

	Control&				getProgress() {return *progress;}
	Static&					getStaticText() {return *staticText;}

	virtual void			inflateChildren();
private:
	Range	m_range;
	double	m_value;

}; // class ProgressBar

inline Serializer& operator<<(Serializer& serializer, Range& prms)
{
	return serializer << prms.min << prms.max;
}

#endif /* USERINTERFACE_PROGRESSBAR_H */ 

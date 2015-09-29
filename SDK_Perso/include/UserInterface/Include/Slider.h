#ifndef __USERINTERFACE_SLIDER_H__
#define __USERINTERFACE_SLIDER_H__

#include "ui_Control.h"
#include "SliderButton.h"
#include "Static.h"

#include "UI/Tuner.h"


struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SliderParameters : public ControlParameters 
{
protected:
	static ed::string ms_DefaultResourceFile;
public:
	SliderParameters(int preset = 0); 
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(SliderParameters)
	virtual void serialize(Serializer &);

	typedef ControlParameters Base;

	void					read(const char* text, int size);

	SliderButtonParameters	m_SliderBtnParams;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Slider :	public UI::Tuner, public Control
{
public:
	typedef Control			 Base;	
	typedef SliderParameters Parameters;
private:

	DECLARE_MESSAGE_TABLE()

	void					thisArrange(const Parameters&);
protected:

    SliderButton*			m_SliderBtn;
    Static*					m_Progress;

	void					onLButtonDown(int keys, int x, int y);	
	void					onPaint();
	void					onClose();
	void					onStyleChanging(bool deselect, int newStyle);
	void					onSliderMove();
	void					onProgressClick();

	void					init();
	
public:
							Slider(const Parameters&);

							Slider(int tag,
								    float x = 0.f, float y = 0.f, 
									float X = 0.f, float Y = 0.f);
	virtual					~Slider();

    virtual bool			canAcceptPoint(const Point& point) const;

	void					setPositionRange(double minPos = 0.0, double maxPos = 100.0);
	double					getMinPosition() const;
	double					getMaxPosition() const;

	void					setPosition(double pos);
	double					getPosition() const;

	void					arrange(const Parameters& params)
							{ 
								Control::arrange(params);
								thisArrange(params);
							}
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)

	//============================ UI::Tuner ============================//
	void					setRange(const UI::Tuner::Range& range);
	const UI::Tuner::Range	getRange() const;

	void					setValue(double value);
	double					getValue() const;

	void					setStep(double step);
	double					getStep() const;
};

#endif // __USERINTERFACE_SLIDER_H__
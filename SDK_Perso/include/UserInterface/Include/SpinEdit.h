#ifndef __USERINTERFACE_SPINEDIT_H__
#define __USERINTERFACE_SPINEDIT_H__

#include "ControlWithVisibleAreas.h"
#include "EditBox.h"
#include "SwingButton.h"
#include "UI/Tuner.h"

class Static;

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SpinEditParameters : public ControlWithVisibleAreasParameters
{
public:
	SpinEditParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(SpinEditParameters)
	virtual void serialize(Serializer &);

	typedef ControlWithVisibleAreasParameters Base;	

	void							read(const char*, int);

	float							minimum;
	float							maximum;
	float							step;
	float							value;
	int								precision;

	bool							editBoxReadOnly;

	bool							maximumShow;
	bool							minimumShow;
	int								alignment;	// выравнивание для редакторов

	SwingButtonParameters			decreaseButton;
	SwingButtonParameters			increaseButton;
	EditBoxParameters				editBox[3]; // 0 - для показа value, 1 - для показа maximum'а
												// 2 - для показа minimum'a
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SpinEdit :
			public ControlWithVisibleAreas,
			public UI::Tuner
{
public:
	typedef ControlWithVisibleAreas Base;
	typedef SpinEditParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()

	void thisArrange(const Parameters&);

protected:

	float					value;				// Значение, не выше максимума и не ниже минимума.
    float                   step;				// Величина приращения
    float                   min;
    float                   max;
	int						theLastIncrementSign;// Знак последнего приращения

    int                     precision;			// Сколько знаков после запятой в переменной step

	bool					showSign;			// показывать/скрывать знак '+' 

    void			        init(const SpinEditParameters& inParams);	

	void					onPaint();
	void					onEditKillFocus();
	void					onEditChange();
	void					onEditEndSession();

    void                    fitToActualValue(bool inEditUpdate); // Синхронизировать value и значение
																 // в EditBox с actualValue
	EditBox*				editBox[3];
	SwingButton*			increaseButton;
	SwingButton*			decreaseButton;
	Static*					background;

public:
	SpinEdit(const Parameters params) : Base(params)
	{ 
		init(params);
		thisArrange(params);
	}
	SpinEdit(int tag, float x = 0.f, float y = 0.f, 
			 float X = 0.f, float Y = 0.f);

	virtual ~SpinEdit();

	virtual	bool			canAcceptPoint(const Point& point) const;			

	void					setValueRange(float min = 0, float max = 0);
	void					setMinValue(float min = 0);
	void					setMaxValue(float max = 0);
	float					getMinValue() const	{return min;}
	float					getMaxValue() const	{return max;}

    void					changeValue(float newValue, int lastSign = 0); //Сменить значение и послать сообщение BN_CLICK

    void					setFontColor(unsigned int	color = COLOR_MAKE(255, 255, 255, 255),
										 int			editBoxIndex = 0);
   
    void                    increaseValue();
    void                    decreaseValue();

	int						lastIncrementSign() {return theLastIncrementSign;}//Знак последнего приращения			

    void                    onStyleChanging(bool deselect, int newStyle);
    void                    calculatePrecision();

	void					arrange(const Parameters& params)
								{ Base::arrange(params); thisArrange(params); }
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)

	void					showPosSign(bool in_show = false) { showSign = in_show; }

//===================== UI::Tuner interface ========================
	void					setRange(const UI::Tuner::Range& range);
	const UI::Tuner::Range	getRange() const;

	void					setValue(double value);
	double					getValue() const;

	void					setStep(double step);
	double					getStep() const;
};

#endif // __USERINTERFACE_SPINEDIT_H__ 
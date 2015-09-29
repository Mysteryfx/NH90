#ifndef __USERINTERFACE_SCROLLBAR_H__
#define __USERINTERFACE_SCROLLBAR_H__

#include "ui_Control.h"
#include "SliderButton.h"
#include "Static.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ScrollBarParameters : public ControlWithVisibleAreasParameters
{
public:
	ScrollBarParameters(int preset = 0);
	typedef ControlWithVisibleAreasParameters Base;	
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ScrollBarParameters)
	virtual void serialize(Serializer &);

	void read(const char*, int);

	//Относительные расположения контролей
	int						decreaseButtonXShift;
	int						increaseButtonXShift;
	int						decreaseButtonYShift;
	int						increaseButtonYShift;
	int						scrollerXShift;
	int						decreaseToScroller;
	int						increaseToScroller;

	int						pageSize;

	StaticParameters		decreaseButton;
	StaticParameters		increaseButton;
	SliderButtonParameters	scroller;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ScrollBar : public ControlWithVisibleAreas
{
public:
	typedef ControlWithVisibleAreas Base;
	typedef ScrollBarParameters Parameters;

private:

	DECLARE_MESSAGE_TABLE()

	//Оформление
	void					thisArrange(const ScrollBarParameters& prms);
	
protected:

    //Составные части
	Static*					decreaseButton;
    Static*					increaseButton;
    SliderButton*			scroller;
    
    //Параметры 
	//float					buttonYShift; //Насколько scroller отодвинут от кнопок

	//Состояние
	int						onePageSize;
	int						lastPosition;

	//Инициализация
	void					init();	

	//Обработка событий
	void					onLButtonDown(int keys, int x, int y);	
	void					onSetFocus(Control* losingFocus);
	void					onPaint();
	void					onCreate(ScrollBarParameters* prms);
	void					onClose();
	void					onStyleChanging(bool deselect, int newStyle);
	void					onScrollerMove();
	void					onDecreasePosition();
	void					onIncreasePosition();
	void					onGetKeyboardFocus(int id);


public:
							ScrollBar(const ScrollBarParameters&);

							ScrollBar(int tag,
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);

	virtual					~ScrollBar();

    //Координаты и размеры
	virtual bool			canAcceptPoint(const Point& point) const;

	//Настройка параметров
	void					setScrollRange(float fromCoord, float toCoord);
	float					getFromCoordinate(){return scroller->getFromCoordinate();}
	float					getToCoordinate(){return scroller->getToCoordinate();}
	void					setPositionRange(int minPos = 0,
											 int maxPos = 100);
	int						getMinPosition() const;
	int						getMaxPosition() const;

	//Управление состоянием
    void					setPageSize(int size) {onePageSize = size;};
    int						getPageSize() const {return onePageSize;};
	void					setPosition(int pos, bool notify = true);
	int						getPosition() const;
	void					increasePosition(unsigned int num = 1);
	void					decreasePosition(unsigned int num = 1);
	void					pageUp(unsigned int pageNumber = 1);
	void					pageDown(unsigned int pageNumber = 1);

	//Служебные функции
	void					fitSliderSize();
	void					fitFromToCoord();

	//Оформление
	void					arrange(const ScrollBarParameters& prms);
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)

    //Составные части
	Static*					getDecreaseButton() const {return decreaseButton;}
    Static*					getIncreaseButton() const {return increaseButton;}
    SliderButton*			getScroller()		const {return scroller;}
};

#endif // __USERINTERFACE_SCROLLBAR_H__
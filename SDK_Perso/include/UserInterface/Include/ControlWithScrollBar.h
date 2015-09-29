#ifndef __USERINTERFACE_CONTROLWITHSCROLLBAR_H__
#define __USERINTERFACE_CONTROLWITHSCROLLBAR_H__

#include "ControlWithVisibleAreas.h" 
#include "ScrollBar.h" 

#include "ExpImpSpec.h" 
#include "ui_Point.h"


struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlWithScrollBarParameters 
											: public ControlWithVisibleAreasParameters
{
public:
	ControlWithScrollBarParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ControlWithScrollBarParameters)
	virtual void serialize(Serializer &);

	typedef ControlWithVisibleAreasParameters Base;	

	void read(const char* configText, int configSize);

	ScrollBarParameters		scrollBar;
	
	bool					isExternal;
	int						shiftFromRightBorder;
	int						shiftFromTop;
	int						shiftFromBottom;

	bool					visibleScrollBar;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlWithScrollBar : public ControlWithVisibleAreas
{
public:
	typedef ControlWithVisibleAreas Base;
	typedef ControlWithScrollBarParameters Parameters;

private:
	
	DECLARE_MESSAGE_TABLE()

private:

	ScrollBarParameters		scrollBarParams;
    
    //Параметры ScrollBar-а
	bool					visibleScrollBar;	
	bool					theHasScrollBar;
	float					headerHeight;
	float					shiftFromRightBorder;
	float					shiftFromTop;
	float					shiftFromBottom;

	//Выравнивание ScrollBar-а
	void					fitScrollBar();

	//Оформление
	void					thisArrange(const ControlWithScrollBarParameters& prms);

protected:

    //Составные части
	ScrollBar*              scrollBar;
    
	//Параметры
	float					scrollBarWidth;
    
    void                    init();

	//Обработка событий
	void					onStyleChanging(bool deselect, int newStyle);
	void					onCreate();

	virtual void			updateSelfPaintCoordinates();
	void					correctVisibleArea();

public:
	//Конструкторы
							ControlWithScrollBar(const Parameters& params);

							ControlWithScrollBar(int tag,   
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);

	virtual					~ControlWithScrollBar();
    
	
	//Координаты
	virtual void			setCoordinates(float rx,
										   float ry,
										   float rX,
										   float rY);
	virtual void			fitVisibleArea();
	virtual void			setBorderThickness(bool thin);
    virtual bool			canAcceptPoint(const Point& point) const;

	//Работа со ScrollBar-ом

    //Эта функция умеет не только создавать ScrollBar, 
	//но и адаптировать его к изменившимся параметрам Control-а
	void                    createScrollBar(int fromPos = 0, int toPos = 0, int pageSize = 0,
                                            float headerHeight = 0.0f, int pos = 0);
	void					recreateScrollBar();
    void                    deleteScrollBar();
    bool                    hasScrollBar() const{return theHasScrollBar;}
	float					getHeaderHeight(){return headerHeight;}
	void					setVisibleScrollBar(bool visible);
	bool					getVisibleScrollBar(){return visibleScrollBar;}

	//Параметры
	//Получить информацию о минимальном возможном размере контроля
	float					getMinXSize();

	//Оформление
	void					arrange(const Parameters& prms);
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)

	//Составные части
	ScrollBar*              getScrollBar() const {return scrollBar;}
};

#endif // __USERINTERFACE_CONTROLWITHSCROLLBAR_H__
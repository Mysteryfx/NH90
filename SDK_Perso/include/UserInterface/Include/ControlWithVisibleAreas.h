#ifndef __USERINTERFACE_CONTROLWITHVISIBLEAREAS_H__
#define __USERINTERFACE_CONTROLWITHVISIBLEAREAS_H__

#include "ui_Control.h" 

#include "ExpImpSpec.h" 
#include "ui_Point.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlWithVisibleAreasParameters
															: public ControlParameters
{
public:
	ControlWithVisibleAreasParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ControlWithVisibleAreasParameters)
	virtual void    serialize(Serializer &);

	typedef ControlParameters Base;

	void read(const char* configText, int configSize);

	//Отступы от края контроля до области, в которой находится текст
	int				xLeftShift;
	int				xRightShift;
	int				yTopShift;
	int				yBottomShift;

	//Выравнивать ли видимую область так, чтобы в ней целиком отображался текст
	bool		theAutoVisibleArea;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlWithVisibleAreas : public Control
{
public:
	typedef Control Base;
	typedef ControlWithVisibleAreasParameters Parameters;

private:	
	
	DECLARE_MESSAGE_TABLE()

	//Оформление
	void					thisArrange(const ControlWithVisibleAreasParameters& prms);

protected:

//	ControlWithVisibleAreasParameters parameters;

	//Параметры
	//Текущая видимая область
	int						visibleAreaWidth;
	int						visibleAreaHeight;
	Point					visibleAreaBegin;

	int						visibleAreaPWidth;
	int						visibleAreaPHeight;
	Point					visibleAreaPBegin;

	//Параметры
	//Сдвиги до видимой области
	float					xLeftShift;
	float					xRightShift;
	float					yTopShift;
	float					yBottomShift;

	float					xPLeftShift;
	float					xPRightShift;
	float					yPTopShift;
	float					yPBottomShift;


	//Параметры
	//Будет подстраивать VisibleArea под caption
	bool					theAutoVisibleArea;

	//Обработка событий
	void					onMove(int dx, int dy);
	void					changeSizeMessage();

protected:
	virtual void			updateSelfPaintCoordinates();
	virtual void			inflateSelf();
	void					init();

public:
	//Конструкторы							
							ControlWithVisibleAreas(const Parameters& params);

							ControlWithVisibleAreas(int tag,   
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);
	virtual					~ControlWithVisibleAreas();

	//Настройка видимой области
	void					setAutoVisibleArea(bool newAutoVisibleArea){theAutoVisibleArea = newAutoVisibleArea;}
	bool					autoVisibleArea(){return theAutoVisibleArea;}
	void					setVisibleAreaBegin(float newX, float newY);
	void					setVisibleAreaBegin(Point newBegin);
	void					setVisibleAreaWidth(int newWidth);
	void					setVisibleAreaHeight(int newHeight);
	Point					getVisibleAreaBegin(){return visibleAreaBegin;}
	int						getVisibleAreaHeight(){return visibleAreaHeight;}
	int						getVisibleAreaWidth(){return visibleAreaWidth;}
	Point					getVisibleAreaPBegin(){return visibleAreaPBegin;}
	int						getVisibleAreaPHeight(){return visibleAreaPHeight;}
	int						getVisibleAreaPWidth(){return visibleAreaPWidth;}
	void					fitXVisibleArea(bool update = true);
	void					fitYVisibleArea(bool update = true);
	virtual void			fitVisibleArea();
	void					fitPXVisibleArea(bool update = true);
	void					fitPYVisibleArea(bool update = true);
	virtual void			fitPVisibleArea();
	void					fitPShifts();
	
	//Настройка отступов
	void					setXShift(float newShift);
	void					setXLeftShift(float newShift);
	void					setXRightShift(float newShift);
	void					setYShift(float newShift);
	void					setYTopShift(float newShift);
	void					setYBottomShift(float newShift);
	void					setBorderSize(float borderSize);
	void					setShifts(	float newXLeftShift, float newYTopShift, 
										float newXRightShift, float newYBottomShift);
	int						getXLeftShift(){return (int)xLeftShift;}
	int						getXRightShift(){return (int)xRightShift;}
	int						getYTopShift(){return (int)yTopShift;}
	int						getYBottomShift(){return (int)yBottomShift;}
	
	//Координаты
	void					shift(float dx, float dy);
	virtual void			setCoordinates(float x, float y, float X, float Y);

	//Параметры
	//Рамка
	virtual void			setBorderThickness(bool thin);

	//Параметры
	//Получить информацию о минимальном возможном размере контроля
	float					getMinXSize();
	float					getMinYSize();

	//Оформление
	void					arrange(const Parameters& prms);
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)
};

#endif // __USERINTERFACE_CONTROLWITHVISIBLEAREAS_H__
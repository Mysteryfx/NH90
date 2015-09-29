#ifndef __USERINTERFACE_SLIDERBUTTON_H__
#define __USERINTERFACE_SLIDERBUTTON_H__

#include "Static.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SliderButtonParameters : public StaticParameters
{
public:
	SliderButtonParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(SliderButtonParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;	

	void read(const char*, int);
	
	bool					useFromTo;
	float					fromCoordinate;
	float					toCoordinate;

	bool					usePositions;
	double					minPosition;
	double					maxPosition;

	bool					usePosition;
	int						position;

	int						minSize;
	bool					verticalDir;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SliderButton : public Static
{
public:
	typedef Static Base;
	typedef SliderButtonParameters Parameters;

private:	

	DECLARE_MESSAGE_TABLE()

private:
	
	//Служебные переменные
	double                  pointsPerPos;    //На сколько точек должен сместиться курсор, чтобы сместить слайдер на одну позицию
    float                   posOnSlider;     //На каком расстоянии от края слайдера нажали

	//Оформление
	void					thisArrange(const SliderButtonParameters& prms);

protected:


	//Параметры
	//Настраиваемые
	float					fromCoordinate;
	float					toCoordinate;
	double					minPosition;
	double					maxPosition;

	//Параметры
	//Ненастраиваемые
	float                   minSize;         //минимальный размер 
	float					fixedSize;		 //Если больше нуля, то имеет фиксированный размер, если нет, то изменяет размер
	float                   pixelsPerString; //на сколько пикселей уменьшается размер при увеличинии количества невидимых строк на 1
	
	//Состояние
	double					position;

	//Инициализация
	void					init();	

	//Реакция на события
	void					onLButtonDown(int keys, int x, int y);	
	void					onLButtonUp(int keys, int x, int y);
	void					onMouseMove(int keys, int x, int y);	
	void					onPaint();
	void					onMove(int dx, int dy);
	void					onSetFocus(Control* losingFocus);
	void					onKillFocus(Control* reseivingFocus);
	void					onCreate(SliderButtonParameters* prms);
	void					onClose();
	void					onStyleChanging(bool deselect, int newStyle);
public:
							SliderButton(const Parameters&);
	//Конструкторы
							SliderButton(int tag,
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);
	virtual					~SliderButton();

	//Настройка параметров
	void					setPositionRange(double minPos = 0,
											 double maxPos = 0,
											 bool	onFitSliderSize = true);
	double					getMinPosition() const					{return minPosition;}
	double					getMaxPosition() const					{return maxPosition;}
	void					setScrollRange(float	fromCoord,
										   float	toCoord,
										   bool		onFitSliderSize = true);
	float					getFromCoordinate(){return fromCoordinate;}
	float					getToCoordinate(){return toCoordinate;}
    void                    getXYofPos(double pos, float *x, float *y);

	virtual void			setCoordinates(float rx, float ry, float rX, float rY);
	virtual void			shift(float dx, float dy);
	void					shiftFromToCoordinates(float dx, float dy);

	//Управление состоянием
	void					setPosition(double inPos, bool inParentNotify = true);
	double					getPosition() const						{return position;}
    void                    fitSliderSize();
	int						getFixedSize() const {return (int)fixedSize;}
	void					setFixedSize(int size = 0);

	//Оформление
	void					arrange(const Parameters& prms);
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)
};

#endif // __USERINTERFACE_SLIDERBUTTON_H__
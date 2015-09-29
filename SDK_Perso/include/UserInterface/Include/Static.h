#ifndef __USERINTERFACE_STATIC_H__
#define __USERINTERFACE_STATIC_H__

#include <ed/map.h>

#include "ControlWithVisibleAreas.h"
#include "EditString.h"
#include "Styles.h"

#include "UI/Button.h"
#include "UI/Switch.h"

class DrawUIText;
class DrawPrimitive;

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR StaticParameters: public ControlWithVisibleAreasParameters
{
protected:
	bool&						initialized();//Заполнены ли имена файлов ресурсов по умолчанию
	ed::map<int, ed::string>& defaultResourceFiles();

public:
	void init();

	StaticParameters(int _preset = 0);
	const ed::string& defaultResourceFile();
	virtual void serialize(Serializer &);

	typedef ControlWithVisibleAreasParameters Base;

	void	read(const char* configText, int configSize);

	//Caption
	ed::string				caption;

	//Параметры
	
//--------------- Старая графика --------------------------------------		
	int						font;
//---------------------------------------------------------------------
	ed::string				fontType;
	float					fontSize;
	unsigned				fontFlags;

	ed::vector<unsigned int> stateFontColors;	// координаты начала текстуры для всех сосотяний
	bool					activating;
	bool					pressing;
	int						alignment;
	bool					elevated;
	bool					isSwitch;
	bool					upclick;
	bool					isText;
	bool					is3dText;
	bool					is3dTextDown;
	bool					autoStyle;
	bool					repeatAction;
	bool					useCaption;
	unsigned int			activationColor;
	bool					dottail;

	bool					autoSize;
	//Хранитель ресурсов
	RESOURCEKEEPER_H(StaticParameters)
};

class EditString;

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Static :
			public ControlWithVisibleAreas,
			public UI::Button,
			public UI::Switch
{
public:
	typedef ControlWithVisibleAreas Base;
	typedef StaticParameters Parameters;

private:
	
	DECLARE_MESSAGE_TABLE()

	//Оформление
	void					thisArrange(const StaticParameters& prms);

protected:

	//Состояние 
	bool					isPressed;
    bool                    isActive;
	int						lastChangingTime;
	bool					wasLButtonDown;
	bool					autoSize;
		
	int						alignment; // caption alignment

	//Параметры
	//Настраеваемые в конфигах

	ed::vector<unsigned int> stateFontColors; // координаты начала текстуры для всех сосотяний
	EditString*				caption;
	bool					autoStyle; //Выберет автоматически, должен ли контроль иметь стиль bs_text

	//Параметры

	//Информация для отображения
	//Активация
//--------------- Старая графика --------------------------------------		
	USERINTERFACEVERTEX*	activationVertices;
//---------------------------------------------------------------------
	DrawUIText*				text;
	DrawPrimitive*			activationBackground;

	virtual void			updateBorders();
	void					updateSelfBorders();

	//Отображение
	virtual void            setOrder(bool deselect, int newStyle); //Устанавливает по-умолчанию Static с картинкой ниже всех других объектов, а Static с надписью - выше
	void					setActivationVertices();
	
	//Инициализация
	void					init();	
    
	//Обработка событий
	void					onLButtonUp(int keys, int x, int y);
    void					onLButtonDown(int keys, int x, int y);
	void					onLButtonDblclk(int keys, int x, int y);
	void					onMouseWheel(short keys, short zDelta, int x, int y);
	void					onKeyDown(int virtKey, int keyData);
	void					onActivate(int state, Control* previous);
	void					onPaint();
	void					onMove(int dx, int dy);
	void					onKillFocus(Control* reseivingFocus);
	void					onClose();
	void					onStyleChanging(bool deselect, int newStyle);
	void					onSize();

	/* send notificastion to parent */
	void					raiseEvent( int );

	virtual void			inflateSelf();

	virtual void			showCursor();

	virtual void			calculateCaptionParameters();
	void					calculateFontPSize();
public:
	//Конструкторы
							Static(const Parameters&);

							Static(int tag,
								     float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);

	virtual					~Static();

	//Обработка событий
	void					onSetFocus(Control* losingFocus);	
   	
	//Параметры
	//Координаты и размеры
	virtual void		    setCoordinates(float rx, float ry,
										   float rX, float rY);
	virtual void			shift(float dx, float dy);
	virtual float			getFullWidth();
	virtual float			getFullHeight();

	bool					canAcceptPoint(const Point& point) const;
	
	//Параметры
	//Состояние
    void                    setIsActive(bool newIsActive = false){isActive = newIsActive;}
    bool                    getIsActive(void) const {return isActive;}

    void                    changePressedState  (bool newState, bool mustNotify = true);
	void                    changeActivatedState(bool newState);

	//Параметры
	//Надписи и выравнивание
	void					setCaption(const char* caption, int pos);
	virtual void			setCaption(const char* caption);
	virtual const char*		getCaption() const;	

//	подгонка размеров контрола под его сaption 
//	indentSize - количество пикселей отступа, fromLeft - какая координата не меняется (left/right)
	virtual void			AdjustmentSize(int indentSize, bool fromLeft);	

	void					setCaptionAlignment(int pos);
	int						getCaptionAlignment(void) const;
	virtual void			fitPCaption();
	void					selfInflate();
    //Если color = 1, то будет использован цвет, задаваемый переменной fontColor!!!!!
    virtual void			setFontColor(unsigned int color = 1);
	virtual unsigned int	getFontColor(int fontNumber = -1) const;
	void					calculateAutoSizeCoords(float& x, float& y, float& X, float& Y);

//--------------- Старая графика --------------------------------------		
	int 					getFont() const {return caption->font;}
	void					setFont(int font = font_normal){caption->font = font; calculateCaptionParameters();}
//---------------------------------------------------------------------

	void					getFontParams(ed::string& fontType, 
										  float& fontSize, 
										  unsigned& flags);
	const ed::string&		getFontType();
	float					getFontSize();
	float					getFontPSize();
	unsigned				getFontFlags();
	
	void					setFontParams(const ed::string& fontType,
										  float fontSize = -1, 
										  unsigned flags = 0);
	void					setFontType(ed::string& fontType);
	void					setFontSize(float fontSize = -1);
	void					setFontFlags(unsigned flags = 0);
	void					setItalic(bool set);
	void					setBold(bool set);
	void					setUnderlined(bool set);
	void					setStrikeOut(bool set);

	int						getTextLength(){return caption->getTextLength();}
	int						getTextPixelLength(){return caption->pixelLength;}
	void					setAutoStyle(bool newStyle){autoStyle = newStyle;}
	bool					getAutoStyle(){return autoStyle;}

	//Отображение
	void					setActivationColor(int r, int g, int b, int a);
	void					setActivationColor(unsigned int color);
	
	//Отображение
	//Служебные функции
	virtual void			fitVisibleArea(	float x = -1, float y = -1, 
											float X = -1, float Y = -1);

	//Переопределенные функции установки параметров Control-а
	virtual void			setFace(int face, bool force = false);
	virtual int				findFace();
	virtual void			setNumberOfFaces(int num);

	//Установка цвета контрола для каждого состояния
	virtual void			setStateFontColor(int face, unsigned int color);

	//Оформление
	virtual void			arrange(const Parameters& prms);
	virtual void			arrange(const ed::string& name) NAMEARRAGNGE(Parameters)

	/* UI::Switch:: */
	virtual bool setState(bool);
	virtual bool getState() const { return isPressed; }
};

#endif // __USERINTERFACE_STATIC_H__

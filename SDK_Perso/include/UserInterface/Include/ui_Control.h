#ifndef __USERINTERFACE_CONTROL_H__
#define __USERINTERFACE_CONTROL_H__

#include <ed/vector.h>
#include "Windows.h" 
#include "Message.h"
#include "UserInterfaceVertex.h" 

#include "ExpImpSpec.h" 
#include "ui_Point.h"
#include "MessageTable.h"
#include "ResourceKeeper.h"

#include "UI/Element.h"

#include "Macros.h"
#include "PaintList.h"
#include "Serializer\Serializable.h"
#include "Serializer/STLSerializer.h"

class DrawPrimitive;
class Vector3;
class DescriptorTable;
class MailBox;
class Dialog;
class ControlManager;
template <class TParameters> class ResourceKeeper;
struct Message;

namespace UI
{
	typedef ed::vector<Vector3> DrawPrimitiveVector;
}

inline Point scaleTexCoord(const Point& pt, const Point& scale)
{ return (pt + Point(0.5)) / scale; }

// Correction made 11.01.2002 by Eugene
// Problem : symbol Unknown is ambiguous with enum _MEDIA_TYPE::Unknown from system file file winioctl.h 
// Solution : explicit specify namespace of Unknown 

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlParameters : 
									public Common::FakeUnknown<Serializable>
{
public:
	ControlParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ControlParameters)
	virtual void serialize(Serializer &);

	void read(const char* configText, int configSize);

	int						tag;

	int						face;

	//Координаты и размеры
	bool					useCoordinates;
	Point					origin;
	Point					size;
	int						originPos;

	//Цвет
	unsigned int			color;

	//Текстура
	ed::string				textureFileName;
	Point					textureFileSize;
	bool					alphaChannelPresence;
	bool					textureFilterPresence;

	Point					textureSize;	// размеры текстуры
	ed::vector<Point>		textureCoords;	// координаты начала текстуры для всех сосотяний
	Point					textureCover;

	bool					disabled;
	bool					readOnly;
	
	//Стиль
	bool					drawItself;
	bool					transparent;
	bool					sendParent;
	int						orderStyle;//Смотри Styles.h
	bool					fixedOrder;//Если включено, то контроль не будет всплывать при нажатии

	//Рамка
	int						borderShape;
	bool					thickBorder;
	bool					hasBorder;
	unsigned int			borderColor;
	unsigned int			pressedColor;

	//Порядок отрисовки
	int						layer;
	
	// текст всплывающей подсказки
	ed::string				hint_text;

	int						m_preset;

	bool					acceptMouse;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Control :
		virtual public UI::Element
{
	friend class			ControlManager;
	friend class			PaintList;
	friend class			Dialog;
	friend class			DescriptorTable;

public:

  // FIXME: это временный костыль
  // должен умереть со старым GUI
  static void setCockpitVisible(bool visible) 
  {
    cockpitVisible = visible;
  }

	typedef Control Base;//Так надо для инвариантности контролей
	typedef ControlParameters Parameters;
private:
    static bool cockpitVisible;
	const char*				controlType;

private:
	//Список событий	
	static const MSGTABLE_ENTRY 
							messageEntries[];
	void					dispatchMessage(Control* control, 
											Message* message,	
											const MSGTABLE_ENTRY* entry);
	const MSGTABLE_ENTRY*	findMessageEntry(const MSGTABLE_ENTRY* entry, 
											 int messageID, 
											 int controlID,
											 int notifyCode = 0);

protected:	
	virtual void			increaseDialogCounter(){};
	virtual void			decreaseDialogCounter(){};

//Таблица событий	
	static const MSGTABLE	messageTable;
	virtual const MSGTABLE* getMessageTable() const;
//Дети и родители
	Control*				m_parent;
	DescriptorTable*		children;

	PaintList				paintList;					//Список отрисовки детей
	int						layer;						//Слой, в котором находится control
	PaintListIterator		paintListPosition;
	PaintListIterator		setPaintListPosition(PaintListIterator iter){return paintListPosition = iter;}

//Почтовый ящик
	MailBox*				mailBox;

//Информация о контроле
	//Тэг
	int						tag;						// for identification this control among siblings
	int						initialTag;

	//Стиль	
	int						style;
	int						originPos;

	//Рамка
	int						borderShape;
	bool					isThinBorder;

	//Точка клика мышкой
	Point					dragPoint;

	//Координаты углов
	float					controlx, controlX, controly, controlY;
	float					controlPx, controlPX, controlPy, controlPY; //Координаты, по которым контроль будет рисоваться (зависят от разрешения экрана)
	static Point			inflateCoeffs;
	static Point			inverseInflateCoeffs;

	//Текстура
//--------------- Старая графика --------------------------------------
	bool					alphaChannelPresence;
	bool					textureFilterPresence;
	int						textureIndex;
//---------------------------------------------------------------------
	ed::string				textureFileName;
	float					tx, ty, tX, tY;
	Point					textureSize;
	ed::vector<Point>		textureCoords; // координаты начала текстуры для всех сосотяний
	int						m_CurFace;
	int						m_RefFace;
	Point					textureCover;
    float                   textureRotation;

//Информация для отображения
	//Заливка

//--------------- Старая графика --------------------------------------
	USERINTERFACEVERTEX*	vertices;
	int						numberOfVertices;

	//Рамка
	USERINTERFACEVERTEX*	borders[5];
	int						numberOfBorderVertices[5];
	unsigned short*			borderIndices[5];
	int						numberOfBorderIndices[5];
//---------------------------------------------------------------------
	unsigned int			borderColor;
	unsigned int			pressedColor;

	DrawPrimitive*			background;
	DrawPrimitive*			backgroundTexture;
	DrawPrimitive*			ltOuterBorder;
	DrawPrimitive*			ltInnerBorder;
	DrawPrimitive*			rbOuterBorder;
	DrawPrimitive*			rbInnerBorder;

	ed::string				hint_text;

	//Оформление
	void					thisArrange(const ControlParameters&);
	bool					destroyChildrenOnExit;

	//Только по умолчанию или програмно
	int						delayBeforeRepeatAction;

	bool					acceptMouse;

protected:
	virtual void			updateSelfPaintCoordinates();
	virtual void			updatePaintCoordinates();
	void					updateSelfBorders();
	virtual void			updateBorders();
	
	//Обработка событий
	virtual	void			onMove(int dx, int dy);
	virtual	void			DoPaint();
	virtual	void			onCreate();
	virtual void			paintChildren();
	virtual	void			onStyleChanging(bool deselect, int newStyle);
	virtual	void			onLButtonDown(int keys, int x, int y);
	virtual	void			onLButtonDblclk(int keys, int x, int y);
	virtual	void			onMouseMove(int keys, int x, int y);
	virtual	void			onActivate(int state, Control* previous);

	virtual void			showCursor();

	void					init();

	virtual					~Control();
	void					clearPtrs();

	virtual	void			onPaint();
	virtual					Control* findChildByCoordAskChildren(const Point& p, bool makeTop);

	//Родители
	void					setParent(Control* parent)		{m_parent = parent;}
public:
	//Конструкторы							
							Control(const Parameters&);
							Control(int tag,   
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);

	static ControlManager*	getControlManager();

	// FIXME: временный костыль до удаления старого gui
	virtual bool isNeedInputDialog() { return false; }

	//Оформление
	void					setParameters(const Parameters& p) { thisArrange(p); }
	void					getParameters(Parameters&);

	void					arrange(const Parameters& prms)	{ setParameters(prms) ;}
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)

	//Обработка сообщений
	void					processMessages();
	virtual void			processMessage(Message* message); 
	void					sendMessage(int messageID, DWord lParam, DWord wParam);
	void					postMessage(int messageID, DWord lParam, DWord wParam);
	void					sendCommandToParent(int command);	// send WM_COMMAND to parent
	void					sendMessageToAllChildren(int messageID, DWord lParam, DWord wParam); //Рекурсивно разослать всем детям событие

	//Родители
	Control*				getParent() const				{return m_parent;}
	Dialog*					getOwnerDialog();
	//Проверка, не является ли probableParent одним из цепочки владельцев this
	bool					belong(const Control* probableParent) const; 

	//Дети
	bool					hasChildren() const;			
	Control*				getChild(int tag) const;				
	bool					hasChild(Control* child) const;

	virtual int				registerControlAsChild(Control* child);
	virtual Control*		unregisterControlAsChild(int tag, bool autoDelete = true);
	virtual Control*		unregisterControlAsChild(Control* control, bool autoDelete = true);
	void					clearChildrenList(bool autoDelete);
	
	Control*				findChildByCoord(const Point& pnt, bool makeTop = false);

	//Порядок отрисовки	
	void					makeChildTopMost   (int tag);
	void					makeChildBottomMost(int tag);
	void					makeChildTopMost   (Control* control);
	void					makeChildBottomMost(Control* control);

	void					setLayer(int layer);
	int						getLayer(){return layer;}
	PaintListIterator		addToPaintList(Control* control);

	PaintListIterator		getPaintListPosition();
	PaintListIterator		paintListEnd(){return paintList.end();}

	//Параметры Control-а
	//Тэг
	int						getTag() const	{return tag;}
	void					setTag(int tag);

	void					setInitialTag() {tag = initialTag;}

	//Координаты и размеры
	void					setOrigin(const Point& org, int mode = 0);
	void					setOrigin(float newX, float newY, int mode = 0);
	void					setSize(const Point& sz);
	void					setOriginPos(int newPos = 0){originPos = newPos;}
	int						getOriginPos(){return originPos;}
	virtual void			setCoordinates(float rx, float ry, float rX, float rY);
    virtual void            onRescale(float rx, float ry, float rX, float rY) { setCoordinates(rx,ry,rX,rY); } 
	void					setXSize(float newSize = 18.0f); //Задать фиксированный размер Control-а
	void					setYSize(float newSize = 18.0f); //Задать фиксированный размер Control-а
	void					setSize(float newXSize = 18.0f, float newYSize = 18.0f);//Задать фиксированный размер Control-а
	Point					getLTPoint() const;
	Point					getRBPoint() const;
	float					getHeight() const;
	float					getWidth() const;
	Point					getPLTPoint() const;
	Point					getPRBPoint() const;
	float					getPHeight() const;
	float					getPWidth() const;
	virtual void			shift(float dx, float dy);
	void					shiftBorder(float dx, float dy);
    virtual bool			canAcceptPoint(const Point& point) const;
	int						getMaxWidth();
	int						getMostLeftPoint() const;
	int						getMostRightPoint() const;

    virtual	void			fitByParent(Control* parent);
	virtual	void			unfitByParent(Control* control);

	//Смена разрешения
	const Point&			getInflateCoeffs() const {return inflateCoeffs;}
	const Point&			getInverseInflateCoeffs() const {return inverseInflateCoeffs;}

	virtual void			inflate();
protected:
	virtual void			inflateSelf();
	virtual void			inflateChildren();
public:
	float					inflateValue(float value, float k, 
										 int type = 0);
	float					inflateX(float value, int type = 0);
	float					inflateIX(float value, int type = 0);
	float					inflateY(float value, int type = 0);
	float					inflateIY(float value, int type = 0);

	float					fInflateX(float value, int type = 0);
	float					fInflateIX(float value, int type = 0);
	float					fInflateY(float value, int type = 0);
	float					fInflateIY(float value, int type = 0);

	//Параметры Control-а
	virtual int				getFace() const { return m_CurFace; }
	virtual void			setFace(int face, bool force = false);
	virtual int				findFace();
	virtual int				getRefFace() const { return m_RefFace; }
	virtual void			setRefFace(int inRefFace);

	virtual void			setNumberOfFaces(int num);
	virtual unsigned int	getNumberOfFaces();
	virtual void			setFaceTexCoords(int face, const Point& texOrigin);

	//Текстура
	void					setTextureCoordinates(float tx,
												  float ty,
												  float tX,
												  float tY);
	void					getTextureCoordinates(float& tx,
												  float& ty,
												  float& tX,
												  float& tY);
	bool					getAlphaChannelPresence() const	{return alphaChannelPresence;}
	void					setAlphaChannelPresence(bool acp)	{alphaChannelPresence = acp;}

	bool					getTextureFilterPresence() const	{return textureFilterPresence;}
	void					setTextureFilterPresence(bool tfp);

	int						getTextureIndex() const			{return textureIndex;}
	bool					setTexture(const char* fileName);
	bool					setVideoTexture(const char* fileName);
	ed::string				getTexture() const;
	void					deleteTexture();
    void                    setTextureRotation(float angle);


//Параметры Control-а
	//Цвет заливки
	unsigned int			getColor() const				{return vertices[0].color;}
	void					setColor(int r, int g, int b, int a);
	void					setColor(unsigned int col);
	void					setTextureColor(int r, int g, int b, int a);
	void					setTextureColor(unsigned int col);

	//Рамка
	bool					getBorderThickness() const		{return isThinBorder;}
	virtual void			setBorderThickness(bool thin);

//--------------- Старая графика --------------------------------------	
	USERINTERFACEVERTEX*	getBorder(int i)				{return borders[i];}			
//---------------------------------------------------------------------

	int						getBorderShape()				{return borderShape;}
	int						setBorderShape( int shape, 
											unsigned int inBorderColor = COLOR_MAKE(0, 0, 0, 255));

	//Работа со стилем
	int						getStyle() const	{return style;}
	void					setStyle(int style)	{this->style = style;}
	void					changeStyle(int newStyle, bool deselect = true);
	bool					isInStyle(int standardStyle) const	{return (style & standardStyle) == standardStyle;}
	void					setVisible(bool visible);
	bool					isVisible();

	bool					hasFocus() const;

	bool					getAcceptMouse() const {return acceptMouse;}
	void					setAcceptMouse(bool _acceptMouse){acceptMouse = _acceptMouse;}

	//Отображение
	// vertices is the array that represents the stripe of triangles, where first triangle is described by points in
	// counterclockwise direction in the coordinate system where x is directed to right and y is directed to bottom

//--------------- Старая графика --------------------------------------
	USERINTERFACEVERTEX*	getVertices() 					{return vertices;}
	int						getNumberOfVertices() const		{return numberOfVertices;}
//---------------------------------------------------------------------

	void					setHint(const char* in_text);

	const char*				getControlType() const {return controlType;}
	/* -------------------- generic UI --------------------------- */
	/* UI::Control:: */
	virtual UI::Element*	parent() const;
	virtual int				id() const;
	virtual void			setFlag(UI::Element::Flag, bool);
	virtual bool			getFlag(UI::Element::Flag) const;
	virtual void			activate();
	virtual void			setProfile(const char*);

	static int getStyle(UI::Element::Flag);

	/* UI::Group:: */
	virtual UI::Element* child(int) const;

	/* -------------- auto registration stuff --------------------- */

	/**
	 * Create from name
	 */
	static Control* create( const ed::string& resourceName, const char* name, 
							const char* config, int cfg_size);


	class Creator {
	public:
			Creator(const char* name);
		virtual ~Creator() {}

		/**
		 * Create from file
		 */
		virtual Control* create(const ed::string& resourceName, const char* config, int cfg_size)=0;

		/**
		 * Create with param
		 */
		virtual Control* create(const char* params) = 0;

	}; // class Creator


	template<class Ctl, class Base=Control>
	class BaseRegister : public Creator {
	public:
			BaseRegister(const char* classname) : Creator(classname) {}
		virtual Control* create(const ed::string& resourceName, const char* config, int cfg_size);
		virtual Control* create(const char* params);

	}; // class Register<Ctl, Base>

	template<class Ctl>
	class Register : public BaseRegister<Ctl> {
	public:
		Register(const char* classname) : BaseRegister<Ctl>(classname) {}
	};


	template<class Ctl, class Base=Control>
	class BaseRegisterPreset : public Creator {
	private:
		typedef typename Ctl::Parameters Parameters;
		//Parameters m_preset;
		int m_preset;

	public:
			//BaseRegisterPreset(const char* classname, const Parameters& preset)
			BaseRegisterPreset(const char* classname, const int preset)
				: Creator(classname), m_preset(preset) {}
		virtual Control* create(const ed::string& resourceName, const char* config, int cfg_size);
		virtual Control* create(const char* params);

	}; // class RegisterPreset<Ctl, Base>

	template<class Ctl>
	class RegisterPreset : public BaseRegisterPreset<Ctl> {
	public:
		//RegisterPreset(const char* classname, const Parameters& preset) :
		RegisterPreset(const char* classname, const int preset) :
		  BaseRegisterPreset<Ctl>(classname, preset) {}
	};

}; // class Control


template<class Ctl, class Base>
Control* Control::BaseRegister<Ctl,Base>::create(const ed::string& resourceName, 
												 const char* config, int cfg_size)
{
	const Ctl::Parameters& params = *Ctl::Parameters::resourceKeeper().getResource(resourceName, config, 
																		cfg_size, 0, true);
	return new Ctl( params );
}

template<class Ctl, class Base>
Control* Control::BaseRegister<Ctl,Base>::create(const char* params)
{
	return new Ctl( *params );
}


template<class Ctl, class Base>
Control* Control::BaseRegisterPreset<Ctl,Base>::create(const ed::string& resourceName, 
														const char* config, int cfg_size)
{
	ed::string name(config, cfg_size);
	const Ctl::Parameters& params = *params.resourceKeeper().getResource(resourceName, config, 
																		 cfg_size, m_preset, true);
	return new Ctl( params );
}

template<class Ctl, class Base>
Control* Control::BaseRegisterPreset<Ctl,Base>::create(const char* params)
{
	return new Ctl( *params );
}


#endif // __USERINTERFACE_CONTROL_H__

#ifndef __USERINTERFACE_STATICWITHICON_H__
#define __USERINTERFACE_STATICWITHICON_H__

#include "ui_Control.h"
#include "Static.h"
#include "EditBox.h"

template <class TParameters> class ResourceKeeper;

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR StaticWithIconParameters : StaticParameters
{
public:
	StaticWithIconParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(StaticWithIconParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;	

	void					read(const char* text, int size);
	
	int						m_IconXShift;		// Сдвиг иконки от начала контрола по Х
	int						m_TextXShift;		// Сдвиг static'a с надписью от конца иконки по Х
	ed::string				m_Caption;			// надпись
	int						m_CaptionAlignment;	// её выравнивание

	bool					m_ShowIcon;			// показать/скрыть иконку

	bool					m_TextLeftOn;		// сначала редактор, потом иконка 
	bool					m_Rubbe;			// размер редактора всегда = длине его строки

	StaticParameters		m_Icon;				// Параметры иконки
	EditBoxParameters		m_Text;				// Параметры edit'a с надписью
};

class StaticWithIcon : public Static
{
public:
	typedef Static					 Base;
	typedef StaticWithIconParameters Parameters;
private:
	DECLARE_MESSAGE_TABLE()

	bool					m_ShowIcon;
	int						m_IconXShift;	// Сдвиг иконки от начала контрола по Х 
	int						m_TextXShift;	// Сдвиг static'a с надписью от конца иконки по Х
	bool					m_Rubbe;

	// Инициализация/размещение 
	void					Init();
	void					thisArrange(const StaticWithIconParameters& inParams);
protected:
	// Составные части
	Static*					m_Icon;
	EditBox*				m_Text;
    
	// Обработка событий
	virtual void			processMessage(Message* message);
	virtual void			onPaint();
	void					onSetFocus(Control* losingFocus);
	void					onStyleChanging(bool deselect, int newStyle);
	void					onSize();
	void					onLButtonDown(int keys, int x, int y);

	void					onEditBoxChanged();
public:
	StaticWithIcon(const StaticWithIconParameters& inParams);
	StaticWithIcon(int tag, float x = 0.f, float y = 0.f, float X = 0.f, float Y = 0.f);
	virtual					~StaticWithIcon();

	// Координаты и размеры
	virtual	bool			canAcceptPoint(const Point& point) const;			
	void					setCoordinates(float x, float y, float X, float Y);

    // Pазместить всех деток относительно нынешнего положения на экране и размера
	virtual void			arrange(const StaticWithIconParameters& inParams);
	virtual void			arrange(const ed::string& name) NAMEARRANGE(Parameters)
		
	//Установить цвет заливки для m_Text
	virtual void			setColor(unsigned int color = COLOR_MAKE(145, 115, 120, 255)); 
	//Установить цвет шрифта для m_Text
    virtual void			setFontColor(unsigned int color = COLOR_MAKE(255, 255, 255, 255));
    //Сменить стиль деток
	void                    changeChildrenStyle(int newStyle, bool deselect = true); 

	virtual const char*		getCaption() const;	
	virtual void			setCaption(const char* inCaption);

	virtual float			getFullWidth();

	virtual void			setFace(int face, bool force = false);
	virtual void			setRefFace(int face);
	virtual int				getFace() const { return m_Icon->getFace(); }
	virtual int				getRefFace() const { return m_Icon->getRefFace(); }

	virtual void			empty();

	// Составные части
    Static*					getIcon		() const {return m_Icon;}
    EditBox*				getEdit		() const {return m_Text;}
	operator				EditBox*	() const {return m_Text;}	

	//------------- UI::Element------------
	virtual void			setFlag(UI::Element::Flag flag, bool value);
	virtual void			freeze(bool = true);
	virtual void			activate();
	//------------- UI::Switch ------------
	virtual bool					setState	(bool);
	virtual bool					getState	() const;
	//------------- UI::Switch ------------
	virtual const UI::Editor::Text	getText		() const;
	virtual void					setText		(const UI::Editor::Text&);
};

#endif // __USERINTERFACE_STATICWITHICON_H__


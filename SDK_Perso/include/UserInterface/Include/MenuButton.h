#ifndef __USERINTERFACE_MENUBUTTON_H__
#define __USERINTERFACE_MENUBUTTON_H__

#include "Static.h"
#include "MultiColumnListBox.h"

class MainMenu;

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MenuButtonParameters : StaticParameters
{
public:
	MenuButtonParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(MenuButtonParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;	

	void read(const char* text, int size);
	
	int						listYShift; //Насколько выпадающий список отодвинут от EditBox-а
	int						listYTopShift;
	int						minDistFromTop;
	int						minDistFromBottom;
	int						minDistToBottom;		//Если расстояние до низа экрана меньше, то ComboBox откроется вверх

	//Параметры выпадающего списка
	MultiColumnListBoxParameters	list;
};


class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MenuButton : public Static
{
public:
	typedef Static Base;
	typedef MenuButtonParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()

	void thisArrange(const Parameters&);
public:
	bool losingFocus;
protected:
    MultiColumnListBox*		list;

	//Параметры
	float					listYShift;
	float					listYTopShift;
	int						minDistToBottom;
	int						minDistFromTop;
	int						minDistFromBottom;

	//Составные части
	MainMenu*				menu;
	
	//Инициализация
	void					init();	

	//Обработка событий
	void					onPaint();
	void					onClose();
    void					onLButtonDown(int keys, int x, int y);
	void					onLButtonUp(int keys, int x, int y);
	void					onLButtonDblclk(int keys, int x, int y);
	void					onSelectionChanged();
	void					onKeyDown(int virtKey, int keyData);

	//Служебные функции
	void					rememberFocus(Control* losingFocus);
public:
	//Конструкторы
							MenuButton(const Parameters& params);

							MenuButton(int tag,
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);

	virtual					~MenuButton();

	//Обработка событий
	void					onDrop();
	void					onProbablyCloseList();
	void					onActivate(int state, Control* previous);

	//Служебные функции
	virtual	bool			canAcceptPoint(const Point& point) const;
	bool					isMenuChild(){return menu != 0;};
	virtual void			setCoordinates(float x, float y, float X, float Y);
	void					setMenu(MainMenu* menu);
	void					setList();
	MultiColumnListBox*		getList() { return list; }
	void					iAmDropped();
	void					iAmClosed();

    //Оформление
	void					arrange(const Parameters& prms);//Разместить всех чайлдов, относительно нынешнего положения на экране и размера
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)
};

#endif // __USERINTERFACE_MENUBUTTON_H__
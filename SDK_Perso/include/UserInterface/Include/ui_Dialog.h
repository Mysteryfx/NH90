#ifndef __USERINTERFACE_DIALOG_H__
#define __USERINTERFACE_DIALOG_H__

#include "ui_Control.h"
#include <ed/set.h>

enum keep_size_method
{ 
    NO_KEEP,
    KEEP_CENTER,
    KEEP_LEFTTOP,
    KEEP_CENTERTOP,
    KEEP_RIGHTTOP,
    KEEP_RIGHTCENTER,
    KEEP_RIGHTBOTTOM,
    KEEP_CENTERBOTTOM,
    KEEP_LEFTBOTTOM,
    KEEP_LEFTCENTER,
    KEEP_ALL
};

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR DialogParameters : public ControlParameters
{
public:
	DialogParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(DialogParameters)
	virtual void serialize(Serializer &);

	typedef ControlParameters Base;

	void read(const char* configText, int configSize);

	bool						fixedPosition;//Можно ли диалог двигать мышкой
	bool						mustHaveCursor;
    int                         keep_size;//размер в пикселях постоянный 
    int                         repositionToMainView;
};

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlList : public Common::FakeUnknown<Serializable>
{
public:
	typedef ed::list<ed::string> ControlNames;
public:
	ControlList(int preset = 0){};
	ControlNames controlNames;
	RESOURCEKEEPER_H(ControlList)
public:
	virtual void serialize(Serializer &);
	void read(const char* configText, int configSize){};
};


class Static;
class MainMenu;
class Trace;
class CursorOwner;

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Dialog : public Control
{
public:
	typedef Control Base;
	typedef DialogParameters Parameters;

private:
	
	DECLARE_MESSAGE_TABLE()

	static ed::set<ed::string> dialogsWithCursor;

	void createFromFile(const char* fileName);
	//Эта функция введена исключительно для сокращения кода 
	//и может быть вызвана только из метода CreateFromFile
	//Попытка вызвать ее откуда-нибудь окончится неудачей
	void createControl(const ed::string& resourceName,
					   const ed::string& controlName,
					   char const* filePos,
					   int beginPos,
					   int configSize,
					   MainMenu*& mainMenu);
	ed::string       createResourceName(const char* fileName, int num, 
										 MapOfFile* mapOfFile);
	
	//Оформление
	void			  thisArrange(const Parameters& prms);
	
	Point			  initCoords;	
	bool			  mustHaveCursor;
    int               keep_size;
	CursorOwner* cursorOwner;

    void              doKeepSize(int how_to_keep);
protected:

	virtual void	  increaseDialogCounter();
	virtual void	  decreaseDialogCounter();
	
	bool					fastPointAcceptCheck;	
	bool					hasMenu;
	ed::string				name;
	bool					coordsMemorized;
    int                     repositionToMainView;

	void					init();	

	//Обработка событий
 	void					onCreate();
	void					onClose();
	void					onLButtonDown(int keys, int x, int y);	
	void					onLButtonUp(int keys, int x, int y);	
	void					onMouseMove(int keys, int x, int y);	
	void					onActivate(int state, Control* previous){}
	void					onPaint();
	void					onWindowPosChanging(int xRes, int yRes);
	void					onCaption();
	void					onCloseButton();
	void					onStyleChanging(bool deselect, int newStyle);

	bool					isMustHaveCursor() const { return mustHaveCursor; }

public:
	//Конструкторы
							Dialog(const Parameters&);

							Dialog(const char* fileName);

							Dialog(int tag, 
								    float x = 0.f, float y = 0.f, 
									float X = 0.f, float Y = 0.f);
	virtual					~Dialog();

	//Параметры
	bool					getFastPointAcceptCheck() const			{return fastPointAcceptCheck;}
	void					setFastPointAcceptCheck(bool need)		{fastPointAcceptCheck = need;}

	//Служебные функции
	virtual bool			canAcceptPoint(const Point& point) const;
	virtual	void			fitByParent(Control* parent);
	
	//Закрытие
	virtual	void			whenCloseDialog();

	//Оформление
	void					arrange(const Parameters& prms);
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)
    void                    setKeepSize(int how_to_keep);

	// UI::Element
	virtual void			enable(bool what) { disable(!what); }
	virtual void			disable(bool what);
	virtual const ed::string&		getName() const {return name;}
}; // class Dialog

#endif // __USERINTERFACE_DIALOG_H__

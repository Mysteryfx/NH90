#ifndef __USERINTERFACE_COMBOBOX_H__
#define __USERINTERFACE_COMBOBOX_H__

#include "ui_Control.h"
#include "MultiColumnListBox.h"
#include "StaticWithIcon.h"
#include "Static.h"

#include "UI/Chart.h"

template <class TParameters> class ResourceKeeper;

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ComboBoxParameters : ControlWithVisibleAreasParameters
{
public:
	ComboBoxParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ComboBoxParameters)
	virtual void    serialize(Serializer &);

	typedef ControlWithVisibleAreasParameters Base;	

	void read(const char* text, int size);
	
	int						buttonXShift;			//Насколько кнопка отодвинута от EditBox
	int						listYShift;				//Насколько выпадающий список отодвинут от EditBox-а
	int						listYTopShift;
	int						minDistFromTop;
	int						minDistFromBottom;
	int						minDistToBottom;		//Если расстояние до низа экрана меньше, то ComboBox откроется вверх

	bool					editBoxreadOnly;		//Вообще-то это есть и в editbox-е, но очень уж часто таким режимом пользуются
	bool					showIcon;				//в строках списка и в редакторе есть иконки 
    bool                    copyTextureToEdit;      //копировать текстуру с выбранного элемента листа в едитбокс
    bool                    copyFontColorToEdit;    //копировать цвет шрифта с выбранного элемента листа в едитбокс


	
	//Параметры кнопки
	StaticParameters				button;

	//Параметры выпадающего списка
	MultiColumnListBoxParameters	list;

	//Параметры EditBox-а
	StaticWithIconParameters		edit;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ComboBox :
	public ControlWithVisibleAreas,
	public UI::Chart
{
public:
	typedef ControlWithVisibleAreas Base;
	typedef ComboBoxParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()
	
	void					thisArrange(const ComboBoxParameters& prms);
	void					showCursor();
protected:
	float					listYShift;
	float					listYTopShift;
	int						minDistToBottom;
	int						minDistFromTop;
	int						minDistFromBottom;
    bool                    copyTextureToEdit;
    bool                    copyFontColorToEdit;


	// Составные части
    Static*					edit;
    Static*					button;
	MultiColumnListBox*		list;
	bool					noEdit;
    
	
	void					killFocusMessage();			// Рассылка сообщений
	void					init();						// Инициализация
	void					setEditParamsOnSelect();	// Установка параметров редактора по 	
														// выделенному элементу
	// Обработка событий
	void					onPaint();
	void					onCreate(ComboBoxParameters& prms);
	void					onClose();
	void					onSelectionChanged();
	void					onDrop();
	void					onEditClick();
	void					onProbablyCloseList();
	void					onStyleChanging(bool deselect, int newStyle);
	void					onEditKillFocus();
	void					onEditEndSession();
	void					onSize();
	void					onChar(int charCode, int keyData);
	void					onKeyDown(int virtKey, int keyData);
	void					onActivate(int state, Control* previous);
public:
	// Конструкторы
							ComboBox( const Parameters& );
							ComboBox(int tag, float x = 0.f, float y = 0.f, 
												float X = 0.f, float Y = 0.f);
	virtual					~ComboBox();


	// Координаты и размеры
	virtual	bool			canAcceptPoint(const Point& point) const;			

	// Очистка списка и редактора
	void					empty();
    void					clear() { empty(); }

	// Работа с List-ом
	// строки
//	для отображения иконок необходимо:
//	1. Указать в ресурсном файле, что данный ComboBox отображает иконки
//	\showicon{1}
//	2. Прописать там же - откуда будут браться иконки
//	\updateeditwithicon{country_edit.res}
//	\updatelist{country_list.res}
//	3. При вызове функции addString указать ID иконки для данной строки. 
	virtual void			addString(const char* inCaption, int inIconNumber = -1);

	void					delString(int index);
	const char*				getString(int index) const; 
    const char*				getString() const { return getString(getIndexOfSelectedString()); } 
	int						getNumberOfStrings() const;
	void					setItemCaption(const char* caption, int column, int row);
	const char*				getItemCaption(int column, int row) const;
	void					setItemCaption(const char* caption, int row);
	const char*				getItemCaption(int row) const;
	void					setItemFace(int face, int column, int row);
	int						getItemFace(int column, int row);
	void					emptyList();

	// Работа с List-ом
	// Маркер
	int						getIndexOfSelectedString() const; //Синоним getSelectedRow()
	int						getSelectedRow() const;
	void					changeSelectedRow(int row); //Сменить выбранную строку и послать сообщение СBN_SELECTIONCHANGED
	void					setSelectedRow(int row);	//Сменить выбранную строку и не послать сообщения
	void					changeSelectedItem(int column, int row = 0); //Сменить выбранный элемент и послать сообщение СBN_SELECTIONCHANGED
	void					setSelectedItem(int column, int row = 0);    //Сменить выбранный элемент и не послать сообщения
	void					setSelectedItem(const char* inCaption);	    //Сменить выбранный элемент и не послать сообщения

	// Работа с Edit-ом
	const char*				getEditString() const;
	void					setEditString(const char* text, bool force = false);
	void					setEditReadOnly(bool readOnlyState);

    // Оформление
	void					arrange(const Parameters& prms);//Разместить всех чайлдов, относительно нынешнего положения на экране и размера
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters)
		
	void                    setChildrenColor(unsigned int color 
                                = COLOR_MAKE(145, 115, 120, 255)); //Установить цвет заливки всех контролей
    void					setChildrenColor(unsigned int colorEdit,
                                             unsigned int colorList);
    void                    changeChildrenStyle(int newStyle, 
										bool deselect = true); //Сменить стиль List и EditBar
    void					setChildrenFontColor(unsigned int color = COLOR_MAKE(255, 255, 255, 255));
    void                    setCursorColor(unsigned int color = COLOR_MAKE(0, 0, 180, 255));

	void					showList(bool show);

	// Составные части
    Static*					getEditBox() const {return edit;}
    Static*					getButton()  const {return button;}
	MultiColumnListBox*		getList()	 const {return list;}


	//======================= UI::Chart:: interface =============================

	virtual int		newRecord(int before=-1, int count=1);
	virtual int		newField(int before=-1, int count=1);
	virtual bool	delRecord(int row, int count=1);
	virtual bool	delField(int field, int count=1);

	virtual bool	setValue(int record, int field, const ed::string& val);
	virtual bool	getValue(int record, int field, ed::string* val) const;

	virtual int		allRecords() const;
	virtual int		allFields() const;

	virtual bool	setSelection(int record, int field=0, bool extend=false);
	virtual bool	getSelection(int* record, int* field=0, bool next=false) const;

	virtual void	onRescale(float x,float y,float X,float Y);

}; // ComboBox

#endif // __USERINTERFACE_COMBOBOX_H__

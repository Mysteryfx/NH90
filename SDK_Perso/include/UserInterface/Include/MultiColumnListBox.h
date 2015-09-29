#ifndef __USERINTERFACE_MULTICOLUMNLISTBOX_H__
#define __USERINTERFACE_MULTICOLUMNLISTBOX_H__

#include <ed/vector.h>
#include <ed/list.h>
#include "ControlWithScrollBar.h"
#include "UI/Chart.h"

#include "MultiColumnListBoxTypes.h"
#include "ListItemsSortVector.h"

#include "DefaultFileNames.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MultiColumnListBoxParameters : public ControlWithScrollBarParameters
{
public:
	MultiColumnListBoxParameters(int preset = 0);
	typedef ControlWithScrollBarParameters Base;	
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(MultiColumnListBoxParameters)
	virtual void serialize(Serializer &);

	void addColumns(int num, int width = -1, 
					int dist = -1, ed::string itemSet = ed::string(""));
	void addRows(int num, int height = -1, int dist = -1);
	void deleteColumns(int from, int to = -1);
	void deleteRows(int from, int to = -1);
    void resize(int count) { setRowsNum(count); }
	void setColumnsNum(int newColumnsNum);
	void setRowsNum(int newRowsNum);

	void read(const char* text, int size);

	int					headerHeight;
	int					headerDist;
	ed::string			headerSet;
	
	//Параметры
	//Вид
	bool				hasMarker1;
	bool				hasMarker2;
	bool				hasVerticalLines;
	bool				hasHorizontalLines;

	//Параметры
	//Цвета
	//unsigned int		itemFontColor;
	//unsigned int		headerFontColor;

	//Параметры
	//Прочие
	int					minYSize;	 //Вертикальный рамер не может быть меньше этой величины

	//Параметры курсоров
	StaticParameters	marker1;
	StaticParameters	marker2;

	//Параметры размещения курсоров
	Marker				marker1Params;
	Marker				marker2Params;

	//Параметры по умолчанию для добавляемых элементов
	int					defaultColumnWidth;
	int					defaultRowHeight;
	int					defaultColumnDist;
	int					defaultRowDist;
	ed::string			defaultItemSet;
	int					defaultColumnWidthFitType;
	int					defaultLineColor;
	int					m_ItemType;			// тип элемента списка 0 - Static, 1 - StaticWithIcon
	bool				keepXRegion;
	int					minXLeftBorderShift;
	int					minXRightBorderShift;

	bool				mouseMoveSensitive;

	bool				hasHeader;

	int					m_SearchTimeInterval;

	ed::vector<ColumnsPrms>	columnsParameters;
	ed::vector<RowsPrms>		rowsParameters;

	ed::list<ItemsNames> columnsItems;
	ed::list<ItemsNames> rowsItems;
};
//	Замечание:
//	для отображения иконок необходимо:
//	1. Указать в ресурсном файле, что данный список отображает иконки
//	\showicon{1}
//	2. Прописать там же - откуда будут браться иконки
//	\defaultitem{country_list_item.res}
//	3. При вызове функции addString указать ID иконки для данной строки. 

class Static;
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MultiColumnListBox :
			public ControlWithScrollBar,
			public UI::Chart
{
public:
	typedef ControlWithScrollBar Base;
	typedef MultiColumnListBoxParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()
	
	void					setItemParams(const ed::string &inItemString, int inColumn, int inRow);
	void					thisArrange(const MultiColumnListBoxParameters& prms);
protected:
	
	//Параметры
	//Параметры составных частей
	ColumnsParameters	columnsParameters;
	RowsParameters		rowsParameters;
	Marker				marker1Params;
	Marker				marker2Params;

	int					headerHeight;
	int					headerDist;
	ed::string			headerSet;			//Это надо превратить в arrange
	
	//Параметры
	//Вид
	bool				hasMarker1;
	bool				hasMarker2;
	bool				hasVerticalLines;
	bool				hasHorizontalLines;

	//Параметры
	//Цвета
	//unsigned int		itemFontColor;
	//unsigned int		headerFontColor;

	//Параметры
	//Прочие
	int					minYSize;	 //Вертикальный рамер не может быть меньше этой величины

	//Параметры по умолчанию для добавляемых элементов
	float				defaultColumnWidth;
	float				defaultRowHeight;
	float				defaultColumnDist;
	float				defaultRowDist;
	ed::string			defaultItemSet;
	int					defaultColumnWidthFitType;
	int					defaultLineColor;
	bool				keepXRegion;
	int					minXLeftBorderShift;
	int					minXRightBorderShift;

	//Состояние
	bool				firstRowIsHeader;
	int					columnsNum;
	int					rowsNum;

	//Состояние
	//Выделение
	int					selectedRow;
	int					selectedColumn;
	int					markerPosition;

	// поиск строк
	ListItemsSortVector	m_ItemsStrings;			// отсортированный вектор указателей на строки списка
	DWORD				m_CurrentTime;			// 
	int					m_SearchTimeInterval;	// max интервал между нажатиями клавиш во время поиска
	
	//Составные части
	Static*				marker1;
	Static*				marker2;
	Table				table;			// указатели на элементы, из которых состоит ListBox
	int					m_ItemType;		// тип элементов (производный от Static'a)

	//Разметка
//--------------- Старая графика --------------------------------------		
	USERINTERFACEVERTEX*	linesVertices;
	unsigned short *		linesVerticesIndices;
	unsigned int			linesNum;
//---------------------------------------------------------------------

	//Служебные переменные
	bool				needToCalculate;
	bool				theColumnXSizeCalculated;
	bool				columnsFitted;

	int					firstVisibleRow;
	int					firstVisibleColumn;
	int					lastVisibleRow;
	int					lastVisibleColumn;
	
	//Выравнивание колонок
	int					equealComponent;		//Эти штуки нужны, чтобы сделать undo выравнивания колонок
	int					notEquealFitColumnNum;
	int					notEquealComponent;
	bool				canUndoFit;
	int					lastFitType; //Использованный последний раз тип выравнивания колонок по ширине ListBox-а

	//Инициализация
	void				init();	

	//Обработка событий
	void				onPaint();
	void				onCreate();
	virtual void		paintChildren();
	void				onCreate(MultiColumnListBoxParameters* prms);
	void				onClose();
	void				onMouseMove(int keys, int x, int y);
	void				onMouseWheel(short keys, short zDelta, int x, int y);
	void				onLButtonDown(int keys, int x, int y);
	void				onLButtonDblclk(int keys, int x, int y);
	void				onKeyDown(int virtKey, int keyData);
	void				onChar(int charCode, int keyData);
	void				onSetFocus(Control* losingFocus);
	void				onKillFocus(Control* receivingFocus);
//	void				onVScroll(int scrollCode,
//								  int scrollBoxPos,
//								  Control* scrollBar);
	void				onMove(int dx, int dy);
	void				onSize();
	void				onVScroll(int, int, Control*);
	void				onStyleChanging(bool deselect, int newStyle);

	//Манипуляции с ячейками
	void				initializeCell(Static* tempStatic);

	float				getFirstColumnLTX();
	float				getFirstRowLTY();
	float				calculateNextLTX(int column, float previousRBX);
	float				calculateNextLTY(int row,    float previousRBY);			
	float				calculateRBX    (int column, float LTX);		
	float 				calculateRBY    (int row,    float LTY);
	void				calculateCellsPositions();
	
	//Служебные функции
	void				setItemsCoordinates();//Располагает все видимые Item-ы в ListBox-е

	//Определение видимых элементов
	virtual Static*		CreateItem(int inWidth, int inHeight);
	void				findVisibleItems();//Заполняет значения VisibleRows/Columns
	void				setVisibleItems(); //Делает Item-ы видимыми/невидимыми

	//Выделение и прокрутка
	void				tuneScrollBar();
	void				adaptMarkerPosition();//Установить ScrollBar так, чтобы выделенная строчка была видна

	//Разметка
	void				setLinesNumber(int linesNumber); //Установить нужное количество Vertices и VerticesIndices для отрисовки разбиения на ячейки
	void				calculateLinesCoordinates();

	virtual				Control* findChildByCoordAskChildren
											(const Point& p, bool makeTop);
	
	virtual void		inflateSelf();
	virtual void		inflateChildren();

	//Выравнивание
	//Выравнивание в списке
	void				alignStatic(int column, int row, int align = caption_used);
	void				alignColumn(int column, int align = caption_used);
	void				alignRow   (int row   , int align = caption_used);
	void				alignTable(int align = caption_used);// Если нужно выравнять все видимые Static-и, то нужно применять эту функцию, а не ставить в цикл предыдущую!!! Эта функция работает много быстрее.

	//Преобразование координат в колонки/строки
	//Преобразовать экранную координату x в номер колонки, который ей соответствует
	int					xToColumn(float x);
	//Преобразовать экранную координату y в номер строки, который ей соответствует
	int					yToRow(float y);
	//Преобразовать экранные координаты точки в номер соответствущих им строки и колонки
	void				coordToItem(float x, float y, int* column, int* row);	

	//Курсоры
	void				setMarker1Position();
	void				setMarker2Position();
public:
						MultiColumnListBox(const Parameters& params)
							: Base(params) { init(); thisArrange(params); }
	//Конструкторы
						MultiColumnListBox(int tag, 
									 float x = 0.f, float y = 0.f, 
									 float X = 0.f, float Y = 0.f);

	virtual				~MultiColumnListBox();

	//Предустановки колонок и строк
	void				arrangeItem(ed::string set, int column, int row);//если set == -1, то будет использован defaultItemSet
	void				arrangeColumn(ed::string set, int column);
	void				arrangeRow    (ed::string set, int row);
	void				arrangeHeader (ed::string set = DEF_STATIC);
	
	//Манипуляции с колонками и строками
	//Добавление/удаление
	
	//Добавить колонки (количество добавляемых колонок, 
	//					их ширина в пикселях, 
	//					расстояние между колонками, 
	//					имя ресурсного файла с описанием ячейки - Static-а)
	//Если ширина или расстояние < 0, то будут использованы значения по умолчанию
	void				addColumns( int columnsNum = 1, int columnWidth = -1, 
									int dist = -1, ed::string itemSet = ed::string(""));
	//Аналогично для строк
	void				addRows	  (int rowsNum = 1, int rowsHeight = -1, int dist = -1);
	//Удалить строки от from до to. Параметр DestroyOn - позволяет ускорить очищение 
	//списка за счет отказа от пересчета параметров ScrollBar-а
	void				deleteRows    (int from, int to = -1, bool inDestroyOn = false); //-1 означает, что to = from
	//Аналогично для колонок
	void				deleteColumns (int from, int to = -1);
	//Добавить одну строку с текстом = text и иконкой = inIconNum
	//Удобно, когда в списке всего одна колонка
	virtual bool		addString(const char* text, int inIconNum = -1);
	//Взять текст из нулевой колонки, строки row
	//Удобно, когда в списке всего одна колонка
	const char*			getString(int row);
	//Задать количество колонок. Лишние колонки будут удалены. 
	//Недостающие будут созданы с дефолтными параметрами
	void				setColumnsNum(int columnsNum = 1);
	//Аналогично для строк
	void				setRowsNum	 (int rowsNum = 1);
	//Задать число колонок и строк
	void				setTableSize(int columnsNum = 1, int rowsNum = 1);
	//Запросить число колонок
	int					getColumnsNum() const {return columnsNum;}
	//Запросить число строк
	int					getRowsNum   () const {return rowsNum;   }
	//Удалить все строки и колонки
	void				empty();
	//Удалить все строки и колонки
    void				clear() { empty(); }

	//Манипуляции с колонками и строками
	//Заголовок списка
	
	//Есть ли у списка заголовок - фиксированная нулевая строка
	bool				hasHeader(){return firstRowIsHeader && (rowsNum > 0);}
	//Включить/выключить заголовок
	void				setHeader(bool hasHeader = true);
	//Задать высоту заголовка
	void				setHeaderHeight(int height);
	//Запросить высоту заголовка
	int					getHeaderHeight(){return headerHeight;}
	//Задать расстояние от заголовка до следующей строки
	void				setHeaderDist(int dist);
	//Запросить расстояние от заголовка до следующей строки
	int					getHeaderDist(){return headerDist;}
	//Запросить номер первой незаголовочной строки
	int					firstDataRow(){return int(firstRowIsHeader);}

	//Манипуляции с колонками и строками
	//Задание размеров

	//Задать ширину колонки column в пикселях
	void				setColumnWidth(int column, float width  = -1);
	//Задать высоту строки row в пикселях
	void				setRowHeight  (int row   , float height = -1);
	//Запросить ширину колонки column
	float				getColumnWidth(int column = 0);
	//Запросить высоту строки row
	float				getRowHeight  (int row = 0);
	//Задать цвет шрифта в строке row
	void				setRowFontColor(int color, int row);

	// Манипуляции с ячейками
	// определяет тип item 0 - Static, 1 - StaticWithIcon
	void				setItemType(int inType) { m_ItemType = inType; }

	//Получить указатель на элемент таблицы. Им будет, в зависимости от типа ячеек,
	//либо Static, либо StaticWithIcon. Возвращается всегда указатель на Static.
	Static* const		item(int column, int row);
	//Задать текст ячейки колонки column, строки row
	virtual void		setItemCaption(const char* caption, int column, int row);
	//Запросить текст ячейки колонки column, строки row
	const char*			getItemCaption(int column, int row) const ;
	//Задать текст ячейки нулевой колонки, строки row
	//Удобно, когда в списке всего одна колонка
	void				setItemCaption(const char* caption, int row);
	//Запросить текст ячейки нулевой колонки, строки row
	//Удобно, когда в списке всего одна колонка
	const char*			getItemCaption(int row);
	//Задать Face (заранее заданную текстуру) ячейки колонки column, строки row
	void				setItemFace(int face, int column, int row);
	//Запросить Face (заранее заданную текстуру)
	int					getItemFace(int column, int row);
	//Задать цвет шрифта ячейки колонки column, строки row
	void				setItemFontColor(int color, int column, int row);
	//Задать текст колонки column заголовка
	void				setHeaderCaption(const char* caption, int column);

	//Настройка параметров по умолчанию. 
	//Эти параметры будут истользоваться при добавлении новых элементов
	//Задать дефолтную ширину колонки
	void				setDefaultColumnWidth(int newWidth)	{defaultColumnWidth = newWidth;}
	//Запросить дефолтную ширину колонки
	int					getDefaultColumnWidth()				{return defaultColumnWidth;}
	//Задать дефолтную высоту строки
	void				setDefaultRowHeight(int newHeight)	{defaultRowHeight = newHeight;}
	//Запросить дефолтную высоту строки
	int					getDefaultRowHeight()				{return defaultRowHeight;}
	//Задать дефолтный промежуток между колонками
	void				setDefaultColumnDist(int newDist)	{defaultColumnDist = newDist;}
	//Запросить дефолтный промежуток между колонками
	int					getDefaultColumnDist()				{return defaultColumnDist;}
	//Задать дефолтный промежуток между строками
	void				setDefaultRowDist(int newDist)		{defaultRowDist = newDist;}
	//Запросить дефолтный промежуток между строками
	int					getDefaultRowDist()					{return defaultRowDist;}
	//Задать имя дефолтного ресурстного файла с описанием формата ячейки
	//Этот файл является обычным файлом для описания Static-а или StaticWithIcon,
	//в зависимости от типа item-а. 
	void				setDefaultItemSet(ed::string newSet){defaultItemSet = newSet;}
	//Запросить имя дефолтного ресурсного файла с описанием формата ячейки
	ed::string			getDefaultItemSet()					{return defaultItemSet;}
	//Задать имя дефолтного ресурстного файла с описанием формата ячейки заголовка
	//Этот файл является обычным файлом для описания Static-а или StaticWithIcon,
	//в зависимости от типа item-а. 
	void				setHeaderSet(ed::string newSet)	{headerSet = newSet;}
	//Запросить имя дефолтного ресурсного файла с описанием формата ячейки заголовка
	ed::string			getHeaderSet()						{return headerSet;}
	//Задать способ автоматического выравнивания ширины колонок (cм. Styles.h)
	void				setDefaultColumnWidthFitType(int newColumnWidthFitType)	
										{defaultColumnWidthFitType = newColumnWidthFitType;}
	//Запросить способ выравнивания ширины колонок 
	int					getDefaultColumnWidthFitType()				{return defaultColumnWidthFitType;}
	//Эти две функции в данный момент не поддерживаются, но оставлены на будущее
	void				setDefaultLineColor(int newLineColor)	
										{defaultLineColor = newLineColor;}
	int					getDefaultLineColor()				{return defaultLineColor;}

	//Видимые строки/колонки
	//Запросить номер первой видимой строки
	int					getFirstVisibleRow	 (){return firstVisibleRow;   }
	//Запросить номер первой видимой колонки
	int					getFirstVisibleColumn(){return firstVisibleColumn;}
	//Запросить номер последней видимой строки
	int					getLastVisibleRow	 (){return lastVisibleRow;    }
	//Запросить номер последней видимой колонки
	int					getLastVisibleColumn (){return lastVisibleColumn; }

	//Состояние
	//Нажать/отжать item
	//Проверить, нажат ли Static в ячейке колонки column и строки row
	bool				isItemPressed(int column, int row);
	//Нажать или отжать Static в ячейке колонки column и строки row
	void				setItemOnOff(bool on, int column, int row);

	//Выделение
	
	//Запросить последнюю выбранную строку (ту, в которую последний раз кликнул мышью пользователь)
	int					getSelectedRow() const {return selectedRow;}
	//Запросить последнюю выбранную колонку (ту, в которую последний раз кликнул мышью пользователь)
	int					getSelectedColumn() const {return selectedColumn;}
	//Запросить последнюю выбранную ячейку (ту, в которую последний раз кликнул мышью пользователь)
	Point				getLastItemPressed(){return Point((float)selectedRow, (float)selectedColumn);}
	//Выбрать ячейку и послать нотификацию о том, что ячейка выбрана
	void				changeSelectedItem(int column,
										   int row = 0,
										   int inMessage = LBN_SELCHANGE);
	//Выбрать ячейку и не посылать нотификацию о том, что ячейка выбрана
	void				setSelectedItem(int column, int row = 0);

	//Выбрать ячейку и не посылать нотификацию о том, что ячейка выбрана
	void				setSelectedItem(const char* inCaption);

	//Выбрать строку (при этом будет выбранна нулевая колонка) 
	//и послать нотификацию о том, что строка выбрана
	void				changeSelectedRow (int row){changeSelectedItem(getSelectedColumn(), row);}
	//Выбрать строку (при этом будет выбранна нулевая колонка) 
	//и не посылать нотификацию о том, что строка выбрана
	void				setSelectedRow (int row){setSelectedItem(getSelectedColumn(), row);}
	//Найти ячейку с данным текстом
	Point				findItem(const char* inCaption);
	//Сделать строки readonly. 
	//(У них можно задать другой цвет и они не будут выбираться/нажиматься)
	void				freezeRows(int from, int to = -1);
	//Сделать строки не readonly
	void				unfreezeRows(int from, int to = -1);

	//Положение маркера
	
	//Задать положение курсора
	void				setMarkerPosition(int row);
	//Запросить пололжение курсора
	int					getMarkerPosition(){return markerPosition;}
	//Установить курсор на строку -1 (список будт выглядеть так, будто курсора нет)
	void				markerOff();
	//Включить/выключить первый курсор (прямоугольник размером во всю строку)
	void				setMarker1(bool hasMarker){hasMarker1 = hasMarker;}
	//Включить/выключить второй курсор (в меню это - стрелочка в левой части большого курсора)
	void				setMarker2(bool hasMarker){hasMarker2 = hasMarker;}
	//Установить цвет первого курсора
	void				setCursorColor(unsigned int color = COLOR_MAKE(0, 0, 180, 255));

	// для прокрутки (inScrollPage=true - постраничной, false - построчной; + Down, - Up)
	void				scroll(int inNumber, bool inScrollPage = true); 
	//Запросить количество страниц списка
	int					getPagesNumber(); 
	
	//Размеры колонок/строк

	//Рассчитать минимальную ширину списка, при которой видны все его элементы
	//Эта величина будет не меньше, чем minSize
	void				calculateFullXSize(float * xSize, float minSize = -1);
	//Рассчитать минимальную высоту списка, при которой видны все его элементы
	//Эта величина будет не больше, чем maxSize
	void				calculateFullYSize(float* ySize, float maxSize = -1);
	//Рассчитать ширину колонки column, при которой видны все еe элементы
	void				calculateColumnFullSize(float* columnSize, int column = 0);
	//Рассчитать ширину всех колонок, при которой видны все их элементы
	//Эта величина будет не меньше, чем minSize
	void				calculateColumnsFullSize(float* columnsSize, float minSize = -1);
	//Установить минимальную ширину списка, при которой видны все его элементы
	//Эта величина будет не меньше, чем minSize
	void				setFullXSize(float minSize = -1);
	//Установить минимальную высоту списка, при которой видны все его элементы
	//Эта величина будет не больше, чем maxSize
	void				setFullYSize(float maxSize = -1);
	//Установить ширину всех колонок, при которой видны все их элементы
	//Эта величина будет не меньше, чем minSize
	void				setColumnFullSize(int column = 0);

	//Координаты
	//Установить координаты списка
	void				setCoordinates(float x, float y, float X, float Y);
    void                onRescale(float x, float y, float X, float Y);
	//Сдвинуть список
	virtual void		shift(float dx, float dy);

	//Служебные функции
	//Запросить была ли уже рассчитана минимальная ширина колонок
	bool				columnXSizeCalculated(){return theColumnXSizeCalculated;}
    //Находится ли данная точка в пределах списка
	virtual bool		canAcceptPoint(const Point& point) const;
	//Запросить нужен ли ScrollBar списку, т. е. все ли элементы помещаются в видимую область
	bool				scrollBarNeed();

	//Выравнивание колонок по ширине
	
	//Выровнять колонки по ширине способом fitType (см. Styles.h)
	void				fitColumns(int fitType = fit_default);
	//Убрать выравнивание колонок по ширине
	void				undoFitColumns();
	//Запросить тип последнего выравнивания
	int					getLastFitType(){return lastFitType;}

	//Разметка
	//Функции разметки в данный момент не поддерживаются, но оставлены на будущее
	void				clearLines(); // Очистить список координат разметки
	void				setVerticalLines(bool has);
	void				setHorizontalLines(bool has);
	void				setLines(bool has);
	void				setLines();
	bool				getHasVerticalLines(){return hasVerticalLines;}
	bool				getHasHorizontalLines(){return hasVerticalLines;}
	int					getHasLines(){return hasVerticalLines + hasHorizontalLines + hasHorizontalLines;}
	// 0 - не имеет, 1 - вертикальные, 2 - горизонтальные, 3 - и те и другие (см. Styles.h)

	//Оформление
	void				arrange(const MultiColumnListBoxParameters& params)
							{ Base::arrange(params); thisArrange(params); }
	void				arrange(const ed::string& name) NAMEARRANGE(Parameters)

	//Составные части
	//Получить указатель на первый курсор
	Static*             getMarker1() const {return marker1;}
	//Получить указатель на второй курсор
	Static*             getMarker2() const {return marker2;}
	//Запросить параметры первого курсора
	Marker&				getMarker1Parameters(){return marker1Params;}
	//Запросить параметры второго курсора
	Marker&				getMarker2Parameters(){return marker2Params;}

	//Включить/выключить свойство списка не уходить за край экрана
	void				setKeepXRegion(bool set){keepXRegion = set;}
	//Запросить, включено ли свойство списка не уходить за край экрана
	void				correctXRegion();
/*	void				setMarker1Size();
	void				setMarker2Size(int width, int height);
	void				setMarker1Texture(const char* filename);
	void				setMarker1TextureCoordinates(float tx,
													float ty,
													float tX,
													float tY);

	void				setMarker2Texture(const char* filename);
	void				setMarker2TextureCoordinates(float tx,
													float ty,
													float tX,
													float tY);*/

	//----------------- UI::Chart:: interface ---------------------

	virtual int newRecord(int before=-1, int count=1);
	virtual int newField(int before=-1, int count=1);
	virtual bool delRecord(int row, int count=1);
	virtual bool delField(int field, int count=1);

	virtual bool setValue(int record, int field, const ed::string& val);
//	virtual bool setValue(int, int, int val);
//	virtual bool setValue(int, int, double);

	virtual bool getValue(int record, int field, ed::string* val) const;
//	virtual bool getValue(int, int, int* value) const;
//	virtual bool getValue(int, int, double* val) const;

	virtual int allRecords() const;
	virtual int allFields() const;

	virtual bool setSelection(int record, int field=-1, bool extend=false);
	virtual bool getSelection(int* record, int* field=0, bool next=false) const;

};

typedef MultiColumnListBox ListBox;

#endif // __USERINTERFACE_MULTICOLUMNLISTBOX_H__

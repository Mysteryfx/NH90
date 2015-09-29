#ifndef __USERINTERFACE_STYLES_H__
#define __USERINTERFACE_STYLES_H__

// types of font
enum
{
	font_small = 0,
	font_normal,
	font_normal_bold,
	font_heavy
};

// shapes of frame
enum
{
	frame_inactive = 0,
	frame_active,
	frame_pressed,
	frame_monochrome
};

// different directions
enum
{
	direction_left = 0,
	direction_right,
	direction_down,
	direction_up,
};

// caption alignment
enum
{
	caption_left = 0,
	caption_center,
	caption_right,
	caption_used		//Применяется в служебных целях, чтобы функция не меняла alignment
};

// Button's presets
// Обязательно добавлять новые preset-ы в функцию getPresetNumber!!!!
enum
{
	button_static = 0,
	button_whitestatic,
	button_radiobutton,
	button_mapstatic,
	button_button,
	button_elevatedbutton,
	button_switchbutton,
	button_graphicbutton,
	button_graphicswitchbutton,
	button_checkbox,
	button_iconstatic,
	button_none
};

// EditBox's styles
// edit_string - редактор содержит произвольную стоку - все символы допустимы
// edit_int_number - редактор содержит целое число - допустимы цифры и символы "+-"
// edit_real_number - редактор содержит действительное число - допустимы цифры  и символы "+-.eEdD"
enum
{
	edit_string = 0,
	edit_int_number,
	edit_real_number,
	edit_none
};

//MultiColumnListBox presets
enum
{
	list_list = 0,
	list_checkboxlist,
	list_menulistbox,
	list_multicolumnlistbox,
	list_combobox
};

//fitColumnWidth Types (in MultiColumnListBox)
enum
{
	fit_default = -1,
	fit_none = 0,
	fit_equeal,   //колонки будут увеличены/уменьшены на равные значения
	//колонки будут увеличены с тем рассчетом, чтобы занять всю ширину ListBox-а
	fit_full_begin, //остаток будет распределен между первыми колонками
	fit_full_end    //остаток будет распределяться между последними колонками
};

//Возможные варианты разметки MultiColumnListBox
enum
{
	lines_none = 0,
	lines_vertical,
	lines_horizontal,
	lines_both
};

//OrderStyle property
//Порядок вывода
enum
{
	order_none = 0,
	order_bottommost,
	order_topmost,
};

enum
{
	origin_up_left = 0,
	origin_up_right,
	origin_down_right,
	origin_down_left
};

//Типы преобразования величин при смене разрешения
enum
{
	inflate_normal = 0, //Будет прибавлена величина INFLATE_SHIFT вне зависимости от знака. Это нужно, чтобы работать с абсолютными координатами.
	inflate_sign        //INFLATE_SHIFT будет прибавлен только к положительным числам. Это нужно, чтобы работать со смещениями.
};

// common control's styles
const int cs_visible					= 0x00000001;
const int cs_disabled					= 0x00000002;
const int cs_transparent				= 0x00000004;
const int cs_topmost					= 0x00000008;
const int cs_bottommost					= 0x00000010;
const int cs_thickframe					= 0x00000020;
const int cs_border						= 0x00000040;
const int cs_noparentnotify				= 0x00000080;
const int cs_fixedorder					= 0x00000100;
const int cs_readonly                   = 0x00000200;
const int cs_sendparent                 = 0x00000400;//Пересылать паренту сообщение о левом клике мыши 
const int cs_drawitself			    	= 0x00000800;
const int cs_icon						= 0x00001000;
const int cs_externalscrollbar          = 0x00002000;//ScrollBar будет рисоваться снаружи Control-а

// button and static's styles
const int bs_text						= 0x20000000;
//const int bs_left						= 0x40000000;
//const int bs_center						= 0x80000000;
//const int bs_right						= 0x01000000;
const int bs_3dtext						= 0x02000000;//Вдавленная надпись
const int bs_verticaldir				= 0x04000000;
const int bs_horizontaldir				= 0x08000000;
const int bs_repeataction				= 0x00100000;
const int bs_3dtextdown                 = 0x00200000;//Приподнятая надпись
const int bs_activating					= 0x00400000;//Может быть активирована
const int bs_pressing					= 0x00800000;//Может быть нажата
const int bs_switch						= 0x00010000;//Будет залипать
const int bs_upclick					= 0x00020000;//Послыает сообщение BN_CLICK, когда отпустили кнопку мыши
const int bs_elevated					= 0x00040000;//В неактивном состоянии приподнята
const int bs_dottail					= 0x00080000;//Если установлено, то если строка не умещается, то обрезаем дополняем "..."

// dialog's styles
const int ds_fixedposition				= 0x10000000;	

// list box's styles
const int lbs_mousemovesensitive		= 0x20000000;

//radiogroup's styles

const int rgs_canunpress				= 0x10000000;//Если установлено, то выбранный в данный момент элемент RadioGroup можно будет отжать

#endif // __USERINTERFACE_STYLES_H__
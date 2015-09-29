#pragma once

#include "viObjectNode.h"
#include "IwoLA.h"
#include "cPosition.h"
#include "wsPilonLoad.h"
#include "wsInitData.h"
#include "cAlgorithm.h"

typedef ed::vector<Vector3> coords_vec;

struct lua_State;

enum pilon_unload_type
{
	UNLOAD_NONE = 0,
	UNLOAD_SEQUENTALLY,// Идет ли сброс бомб последовательно (по всем пилонам)
	UNLOAD_TIMED,	   // Идет ли сброс с задержками (МБД и контейнеры)
};

class IwoAIPilon : public viObjectNode
{  
public:
	enum enum_Pilon_Axis
	{
		PILON_ROT_NULL = 0,
		PILON_ROT_X_AXIS = PILON_ROT_NULL,
		PILON_ROT_Y_AXIS,
		PILON_ROT_Z_AXIS,
		PILON_ROT_AXIS_MAX
	};

	virtual ~IwoAIPilon() {};

	virtual void	serialize(Serializer &serializer) = 0;
	virtual void	Init() = 0;			// Инициализация подвески
    virtual void	init_children() = 0;
	virtual void	Set_DrawArgument(int i, float value = 0.0f) = 0;// Задать аргументу i значение value
	virtual float	Get_DrawArgument(int i) = 0;

	// поддержка интерфейса viObject
	virtual cPosition& Position(wModelTime t) = 0;
	const Graphics::DParamList& GetDrawArguments() { return dr_a; }
	virtual Graphics::Model		*GetModel() = 0;
	virtual const	Position3&	GetPosition() = 0;
	virtual bool	UseChildren(wModelTime t, viArgumentPurpose purpose) = 0;

	float			getTime_()			{	return Time_;}
	void			setTime_(float a)	{	Time_ = a;}
	unsigned char	getNodeNum()		{	return NodeNum;}

	IwoAIPilon*	getchild_pilon(int i)
	{
		if (i > -1 && i < child_pilon.size()) 
			return child_pilon[i];
		return NULL;
	}
	virtual void	init_connector_position(bool force = false) = 0;
	void			clean_install()
	{
		Init();
		init_connector_position(true);
	}

	const Graphics::DParamList& GetSubstationArgs(int substation)
	{
		IwoAIPilon * child = getchild_pilon(substation);
		if (child)
			return child->GetDrawArguments();
		return GetDrawArguments(); 
	}

	
	bool            get_state() const { return isready; }
	void            set_state(bool state) { isready = state; }
	float           get_curr_state() const { return curr_state; }
	void            set_curr_state(float state) { curr_state = state; }
	bool			get_need_to_change() const { return need_to_change; }
	void			set_need_to_change(bool state) { need_to_change = state; }
	
	virtual void    change_state(uint8_t axis, float angle) = 0;
	virtual float   set_angle(uint8_t axis, float angle) = 0;

	float			get_angle(uint8_t axis) { assert(axis >= 0 && axis < PILON_ROT_AXIS_MAX); return Fi_add[axis]; }

	virtual void	stow() = 0;

	virtual Vector3	get_element_pos(int) = 0;  //rocket tube position if rocket is not a child in child_pilon
	virtual bool    get_position_by_connector_local(const char* connector_name,Position3 & pos) = 0;
    virtual bool    get_position_by_connector      (const char* connector_name,Position3 & pos) = 0;

	bool			has_launch_animation() {return launch_animation;}
	void			set_launch_state(float state) {launch_state = state;}
	float			get_launch_state() {return launch_state;}
	
	virtual void    free_substation(int substation) = 0;
	virtual bool	check_restore_substations() = 0;
	inline  void	mark_for_restore() 
	{
		if (!marked_for_restore)
		{
			marked_for_restore   = true;
			restore_time_counter = wTime::GetModelTime();
		}
	};

	virtual void    clear() = 0;

    virtual void    set_args_as_tube_launched(int substation, bool launched = true) = 0;

	inline bool		is_rotatable() const
	{
		for(int i = PILON_ROT_NULL; i < PILON_ROT_AXIS_MAX; ++i)	
			if(changing_arg_rot[i] != 0)
				return true;
		return false;
	}

	virtual cPosition get_pilon_directed_position_l(const cVector& vec) = 0;
	virtual void	  onShot(wModelTime time) = 0;
protected:
	bool					marked_for_restore;
	double					restore_time_counter;
protected:
	cPosition				pos_local;
	Position3				pos3;		
	bool					ChildFlag;	// true - подвеска на балке.
	Graphics::DParamList	dr_a;		// Хранилище для аргументов рисования
	float					Time_;      // время последнего пуска
	unsigned char			NodeNum;	// Количество узлов на подвеске-балке
	ed::vector<IwoAIPilon*> child_pilon;// Cписок дочерних пилонов

	bool					isready;	// признак готовности к работе
	
	// управляющий сигнал текущего состояния пилона или подвески
	float					curr_state; // 0.0f  - стоит в одном из крайних положений
									    // 1.0f  - движется на открытие
										// -1.0f - движется на закрытие
	bool					need_to_change;

	// управляющий сигнал текущего состояния пилона в процессе пуска/сброса
	float					launch_state; // 0.0f - стоит в одном из крайних положений
										  // 1.0f - движется на открытие
										  // -1.0f - движется на закрытие
	bool					launch_animation;
// KA: здесь всякая дребедень, которая используется снаружи пилона
// Вообще все управление этими полями идет не из пилона, он тут выступает исключительно в роли структуры
public:
	// Скорости изменения аргументов
	float					arg_rate[PILON_ROT_AXIS_MAX];

	// Требуемые углы поворота (для каждого оружия означают свое, видимо)
	float					needed_Fi_add[PILON_ROT_AXIS_MAX]; // заданный доп. угол поворота (град) по оси x // заданный доп. угол поворота (град) по оси y// заданный доп. угол поворота (град) по оси z

	//////////////////////////////////////////////////////////////////////////
	// Диапазоны углов поворота
	// Диапазоны изменения аргументов
	r_to_r_f				rot_a_to_arg[PILON_ROT_AXIS_MAX]; //rot angle to model argument

	//////////////////////////////////////////////////////////////////////////
	// Номера аргументов для этих углов
	int						changing_arg_rot[PILON_ROT_AXIS_MAX];

	// Это все добро про аргумент стартовой анимации
	int						changing_arg;
	float					arg_ch_range[2];
	float					curr_arg_value;
	float					arg_rate_out;
	float					arg_rate_in;
	
	//rockets launcher draw arguments
	ed::vector<int>		rockets_lau_args_;

	// Тоже используется исключительно снаружи пилона
	// Дополнительные (текущие) углы поворота отдельных частей подвески (град/доли аргумента).
	// Для конкретного вида оружия можно использовать по своему усмотрению.
	//
	// сейчас Fi_z_add:
	// 1) угол поворота ствола для СППУ-22 (ППУ)
	// 2) угол поворота крышки для Меркурия (подвесной контейнер)

	float					Fi_add[PILON_ROT_AXIS_MAX];

	bool					is_dumb_unload;					// сброс на "НЕВЗРЫВ"

	unsigned char			current_unloading_task;
	wModelTime				last_item_dropped_time;			// Когда в последний раз сбросили что-то (для timed_unloading)
	float					drop_delta;						// Через какие промежутки сбрасывать (для timed_unloading)
	bool					unload_started;					// Началась ли разгрузка по timed_unloading


	float					block_unload_time;				// Время разгрузки одного блока КМГУ
	int						blocks_count;					// Количество блоков в КМГУ (хотя вроде нигде не используется)
	int						elements_in_block;				// Количество элементов в одном блоке	
    coords_vec          	coords; 						// координаты подвески суббоеприпасов (КМГУ-2)


	wModelTime				last_draw_time;					// Когда в последний раз рисовали

	bool					to_change_state;				// К какому состоянию надо прийти

	// Главная гадость - информация об оружии на пилоне
	wsPilonLoad		weapon;				

	virtual   void  push_to_state(lua_State * L) const = 0;
};

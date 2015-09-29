//                    file woFreeFall.h
#pragma once

#include "WorldGeneral.h"
#include "wMovingObject.h"
#include "wTime.h"
#include "Randomizer\RandomObject.h"

class woFreeFall;

class WORLDGENERAL_API wDownfall : public wTime
{
public:
	
	wDownfall(woFreeFall *p, wModelTime dt);
	virtual ~wDownfall();	
	
	void NextEvent();

	woFreeFall	*boss;
};

#define IID_IwoFreeFall Common::identify<woFreeFall>()

class WORLDGENERAL_API woFreeFall: public MovingObject  // Свободно падающие объекты.
{         
public:
	static RandomObject	randomEvenly;	// Собственный итератор в нормальной последовательности
	static unsigned long   Create(unsigned long,const cPosition &, const wsType &,ModelInstance * donor = nullptr ); 

	woFreeFall::woFreeFall();
    ~woFreeFall();

	void			setFreeFall(unsigned long,const cPosition & pos);
    int				Class(void) { return woClass_FreeFall;}
	virtual void	serialize(Serializer &serializer); 

	cPosition&		Position(wModelTime t);
	const Graphics::DParamList& GetDrawArguments() { return drawArgs; }
	void			Set_DrawArgument(int i, float value = 0.0f); // Задать аргументу i значение value

	const gBox&		LocalBox(wModelTime t)	{	return box;}
	void			resetActivityPointer()	{	downfall = 0;}

	dVector			GetPoint();
	dVector			GetPoint(wModelTime t);
// Restore
	wtPointer		downfall;	// деятельность падения в модельном времени
private:

    void			Init(void);             // инициализация
	void			Status_Quo(wModelTime);	// Текущее состояние 
	void			SpaceRegister(wModelTime dt); 

	               // Переменные. Variables. Save
	cVector         Ordme;      // вектор угловой скорости.
    float           vy_hold;    // Установившаяся вертикальная скорость
	float           Ay;         // Вертикальное ускорение
	float           Ag;         // Горизонтальное ускорение 
	cVector         nv;         // единичный вектор заданного направления			
	
	cPosition		pos0;		// начальное положение
	float			vg0;		// начальная горизонтальная скорость
	float			vh0;		// начальная вертикальная скорость 
	float			tg;			// время ускоренного движения в горизонте 
	float			th;			// время ускоренного движения по вертикали 
	float			H_hold;		// высота начала движения с vy_hold
	wModelTime		Life_Time;	// время жизни
	wModelTime		LTime0;		// мировое время в начале жизни.
	gBox			box;
	unsigned long	Gun;         // номер создавшего списке мира
	// Restore

	dVector			pos_p0;
protected:
	void					SetShape(const wsType & type,ModelInstance * donor = nullptr);	// Установить внешний вид
	Graphics::DParamList	drawArgs;	// Хранилище для аргументов рисования
	int						dr_a_Num;
};
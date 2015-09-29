#ifndef __ControlRudder_h__
#define __ControlRudder_h__

#include "Base.h"

namespace EagleFM
{

//Модель педального узла в кабине лётчика,
//содержащая описание основных характеристик поста управления,
//таких как: ход педалей, триммеров и пр. 
//Используется как настраиваемое хранилище данных.
class FMBASE_API Pedals //ControlRudder
{
public:
	Pedals();

	double		control;			// Величина воздействия пилота на педали

	double		pos;				// Положение педалей
	double		posMin;				// Минимальное положение педалей
	double		posMax;				// Максимальное положение педалей

	
	double		trimMin;			// Минимальное положение триммера РН
	double		trimMax;			// Максимальное положение триммера РН
	double		trim;				// Положение триммера РН

};
}

#endif
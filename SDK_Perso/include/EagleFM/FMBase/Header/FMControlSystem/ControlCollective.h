#ifndef __ControlCollective_h__
#define __ControlCollective_h__

#include "Base.h"

namespace EagleFM
{

//Модель рычага управления общим шагом винта вертолета,
//содержащая описание основных характеристик поста управления,
//таких как: ход рычага, триммеров и пр. 
//Используется как настраиваемое хранилище данных.
class FMBASE_API Collective
{
public:
	Collective(); //(init)

	double		control;		// Величина воздействия пилота на рычаг


	double		pos;			// Положение РОШ ??????
	double		posMin;			// Минимальное положение РОШ
	double		posMax;			// Максимальное положение РОШ


	double		trimMin;		// Минимальное положение триммера 
	double		trimMax;		// Максимальное положение триммера 
	double		trim;			// Положение триммера 
};

}

#endif
#ifndef _BIN_EFFECTS_H_
#define _BIN_EFFECTS_H_

#include <ed/string.h>
#include <ed/list.h>

struct binEffects
{
	binEffects():uiParent(0)
		, dX(0.0)
		, dY(0.0)
		, dZ(0.0)
	{};

	//id родителя(0 если родителя нет)
	unsigned int uiParent;

	//номер эффекта
	int iCount;

	//положение в пространстве
	double dX;
	double dY;
	double dZ;

	//имя эффекта
	ed::string sEffect;
};

//список эффектов подлежащих передачи
typedef ed::list<binEffects> tListEffects;

#endif	//_BIN_EFFECTS_H_
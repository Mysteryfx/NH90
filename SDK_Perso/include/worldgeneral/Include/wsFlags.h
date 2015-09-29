#pragma once

#include "WorldGeneral.h"

// Далее кусок про wsFlag
const __int32 wsFlag_Active				= 1<<1;	// РЛС включена
const __int32 wsFlag_HumanPlane			= 1<<2;	// Гуманоидный самолет
const __int32 wsFlag_Disturbance		= 1<<3;	// Постановщик радиоэлектронных помех включен
const __int32 wsFlag_IRDisturbance		= 1<<4;	// Постановщик инфракрасных помех включен
const __int32 wsFlag_Born				= 1<<5;	// Активен (едет, стрелляет, облучает и т.д.)
const __int32 wsFlag_AI_ON				= 1<<6;	// Флаг для отключения\включения активностей юнита.
const __int32 wsFlag_Invisible			= 1<<7;	// Флаг невидимости юнита для врагов
const __int32 wsFlag_Immortal			= 1<<8;	// Флаг неуязвимости юнита от огня противника
const __int32 wsFlag_Static			    = 1<<9;	// Флаг активных (наземка-морские) статиков

class WORLDGENERAL_API wsFlags {
	__int32		flags;
	
	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wsFlags& fl)	
	{		return (serializer << (int&)fl);
	}
	
public:
	wsFlags (void){ flags = 0; }
	wsFlags (__int32 d){ flags = d; }
	const bool	IsSet(__int32 f) const	{ return ((flags & f) == 0) ? false : true; }
	void		Set(__int32 f)			{ flags |= f; }
	void		Reset(__int32 f)		{ flags &= ~f; }
	void		Reset(void)				{ flags = 0; }
	__int32		Get()					{ return flags; }
};


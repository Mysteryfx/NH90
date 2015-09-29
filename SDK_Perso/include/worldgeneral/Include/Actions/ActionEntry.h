#ifndef __ACTION_ENTRY_H__
#define __ACTION_ENTRY_H__

#include "WorldGeneral.h"

/*
    Как все это работает.
    Имеются следующие классы:
        ActionSource,   который забирает действия из инпута, проставляет им метки 
                        времени и засылает их в TimedActionBuffer.
                        Если мы стоим на паузе, то действия засылаются сразу в 
                        ActionDispatcher
                    
        TimedActionBuffer, в который сыпятся действия с метками времени из трека 
                        и ActionSource. Далее он их выплевывет в подходящие моменты
                        времени в ActionDispatcher

        ActionDispatcher, который прогоняет поступающие действия через систему фильтров
                        и в соответствии с результатами пишет или не пишет действия 
                        в трек. Содержательную обработку действий осуществляют сами
                        фильтры (они щас сосредоточены в dActionProcessing.cpp)
        ActionTratis,   вспомогательный класс, определяет тип действия и прочую фигню
    То есть получается следующая схема

    Input --> ActionSource --> TimedActionBuffer --> ActionDispatcher <--> IActionFilter
    TrackController ------------------/\                    \---------------> TrackController

    ЗАМЕЧАНИЕ: в этой схеме есть проблема. Действие с меткой времени t обычно выполняется
    перед всеми активностями в момент t. Этого требует логика работы мировых объектов.
    Однако, если мир находится на паузе, то команды ввода с временем t будут выполнены 
    __после__ активностей с временем t. В треке же это будет не так: они будут выполнены, как
    и требуется, перед активностями. Как решить эту проблему, пока непонятно.
*/

typedef unsigned int Action;

namespace Mail { class Stream; }

enum ActionOrigin
{
    AS_INPUT,
    AS_TRACK,
    AS_WORLD	// Такие действия сгенерированы миром
};

struct ActionEntry
{
	enum Kind { ANALOG, DIGITAL } kind;
	Action action;
	double val;
};

struct TimedActionEntry : public ActionEntry
{
	double time;
};

WORLDGENERAL_API Mail::Stream& operator >> (Mail::Stream& stream, ActionEntry& ae);


#endif
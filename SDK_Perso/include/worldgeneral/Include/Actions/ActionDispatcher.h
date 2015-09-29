#ifndef __ACTION_ROUTER_H__
#define __ACTION_ROUTER_H__

#include <ed/vector.h>

#include "ActionEntry.h"
#include "WorldGeneral.h"


class ActionDispatcher;
class TrackController;

// Флаги решения фильтра
typedef unsigned int ActionFilterDecision;

const ActionFilterDecision AFD_CANCEL	= 1;	// Отменить дальнейшую обработку
const ActionFilterDecision AFD_NO_TRACK = 2;	// Идти дальше, в трек не писать
const ActionFilterDecision AFD_PROCEED	= 3;	// Идти дальше
const ActionFilterDecision AFD_FINISH	= 256;	// Завершить обход (выставляется совместно с другими флагами)

class WORLDGENERAL_API IActionFilter
{
public:
	virtual ~IActionFilter();

	// Запись о действии можно изменять
	virtual ActionFilterDecision filter(ActionEntry& entry, ActionOrigin source, ActionDispatcher* router) = 0;
};

// Фильтрует действия через систему фильтров
class WORLDGENERAL_API ActionDispatcher
{
public:
	ActionDispatcher(TrackController* controller);
	~ActionDispatcher();

    // ВНИМАНИЕ: Этими функциями пользоваться не следует, поскольку действия,
    //           записанные в трек таким образом, при воспроизведении будут
    //           проиграны в другом порядке по отношению к активностям.
    //           Если надо послать какое-то действие из мира, следует сделать это
    //           через соответствующие функции TimedActionBuffer.
    //           В идеале эти функции следует убрать, однако система видов 
    //           пользуется ими для проталкивания каких-то действий в паузе.
	void dispatch(const ActionEntry& action,  ActionOrigin source, ActionFilterDecision flag = AFD_PROCEED);
	// Эти действия имеют source == AS_WORLD
	void dispatchDigitalAction(Action act);
	void dispatchAnalogAction(Action act, double val);

	// Фильтры
    // Добавленные фильтры уничтожаются самим диспетчером
    void prependFilter(IActionFilter* filter);  // добавить фильтр в начало списка
	void addFilter(IActionFilter* filter);		// добавить фильтр в конец списка
	void removeFilter(IActionFilter* filter);

private:
	void process_(ActionEntry ae,  ActionOrigin source, ActionFilterDecision route);
	void writeToTrack_(const ActionEntry& act);

private:
	ed::vector<IActionFilter*> filters_;

	TrackController* trackController_;
};

#endif 
#ifndef IAnimScript_H__
#define IAnimScript_H__

#include "AnimPose.h"

#include "ed/string.h"

namespace model
{
class IModel;
struct ModelParams;

// Анимационный сценарий
class IAnimScript
{
public:
	virtual ~IAnimScript() {}
public:
	// имя
	virtual const char* getName()const=0;

	// Запустить сценарий
	virtual void start()=0;
	// Evaluate script. 
	// Время от начала сценария
	virtual void play(float time)=0;
	// Сценарий завершен?
	virtual bool isFinished()=0;

	// Ждем события (интерфейс не определен)
	virtual bool isWaiting()const=0;

	// Количество актеров
	virtual int getActorCount()const=0;

	// текущее состояние
	// Вернет Модель и ее параметры
	virtual model::IModel* getModel(int actor, osg::Matrixd& pos, model::IModelParams& params)const=0;

	// статистика для отладки
	virtual ed::string getStatistic(int actor)const=0;
};
}

#endif

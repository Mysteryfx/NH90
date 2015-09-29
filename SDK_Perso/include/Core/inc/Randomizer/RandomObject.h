#pragma once

#include "_config.h"
#include "Serializer/Serializer.h"
#include <ed/vector.h>

// Каждый объект данного класса гарантирует свою собственную выборку из
// заданной последовательности. Индивидуальность объекта задается смещением.
// Пример использования:
// Создание равномерной последовательности делается один раз
// при старте симулятора (в RandomManager.cpp): 
// globalUniformRandom = globalRandomManager->createUniform(78954602, 2000, 20, 6.0f);
// ...
// Это подготовка к последующим выборкам
// RandomObject r;
// r.init(globalUniformRandom, 793);
//...
// А это собственно выборка
// float val = r.get();
//
// Все датчики, используемые в модели мира, должны инициализироваться в функции
// wWorld::initRandomObjects() и сериализоваться в wWorld::serializeRandomObjects()!
class ED_CORE_EXTERN RandomObject
{
public:
	typedef const ed::vector<float> const_sequence_t;

private:
	unsigned int type;	// 0 - Равномерная, 1 - Нормальная, 2 и более - Заказные
	const_sequence_t* seq;
	const_sequence_t::const_iterator iseq;
public:
	RandomObject();	// По умолчанию, настраивается на начало globalUniformRandom
	RandomObject(RandomObject &r);
	// Для стандартных последовательностей
	RandomObject(unsigned int tp, unsigned int off = 0);
	// Для нестандартных (заказных) последовательностей
	RandomObject(const_sequence_t &seq, unsigned int tp, unsigned int off = 0);
	virtual ~RandomObject() {};

	void init(unsigned int tp, unsigned int off);
	void setOffset(unsigned int off);
	unsigned int getOffset() const;
	void setType(unsigned int tp);
	unsigned int getType() const;
	float get();	// Функция выборки
    float get(unsigned int offset) const;	// Функция выборки
    virtual void serialize(Serializer &s);
};

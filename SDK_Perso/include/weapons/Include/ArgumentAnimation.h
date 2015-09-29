//				file ArgumentAnimation.h
#pragma once

#include "Weapons.h"

#include "WorldMail.h"

#include "wTime.h"
#include "wFloat.h"

// класс для анимации аргументов моделей наземки
// SFINX

class WEAPONS_API ArgumentAnimation
{
public:
	ArgumentAnimation();
	void					setAnimation(float startVal, float endVal, float defVal, const wModelTime duration);
    float                   playForward();
    float                   playBackward();
	float 					stop();
    float                   GetDefVal() const { return _defVal;};
	float					stopToDefault();
	virtual float 			GetValue() const;
    float                   GetSpeed() const { return _v;}; // скорость изменения значения
    void                    getDiffInitStatus(Mail::Writer& stream);
    void	                setDiffInitStatus(Mail::Reader& stream);

	bool					isPlaying(){return _v != 0.0f;}; // проигрываем анимацию или нет

protected:
	
	wModelTime				_duration; // длительность анимации
    float                   _v; // скорость изменения значения аргумента
    float					_defVal;
    float					_minVal, _maxVal; // упорядоченные крайние значения анимации


	wModelTime				_startModelTime; // время начала анимации
	float					_startVal; // значение, с которого началась анимация
	
};

// циклическая анимация
class WEAPONS_API ArgumentAnimationCyclic: public ArgumentAnimation
{
public:
    virtual float 			GetValue() const;
};
#ifndef ObjectLight_H
#define ObjectLight_H

#include "viLight.h"

namespace Effects
{

// Базовый класс для источников света, прикрепленных к объектам
// координаты задаются в ЛСК
class ObjectLight : public viLight
{
public:

	// включение/выключение
	virtual bool IsOn() = 0;
	virtual void TurnOn() = 0;
	virtual void TurnOff() = 0;

	// установка параметров
	virtual void SetColor(const Vector3& color) = 0;
	virtual void SetAttenuation(float distance) = 0;
	virtual void SetProjectorAngle(float angle) = 0;

	// перерегистрация в пространственном хранилище
	// velocity - скорость host в МСК
	// dt - время до следующего UpdatePos
	virtual void UpdatePos(const Vector3& velocity, float dt) = 0;

	// размер с учетом припуска
	virtual float GetSize() = 0;
	
};

} // namespace Effects

#endif // ObjectLight_H

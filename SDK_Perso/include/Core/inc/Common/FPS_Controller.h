#ifndef FPS_Controller_H
#define FPS_Controller_H

#include "_config.h"

// Контроль за плавностью симуляции
// Задачи:
// - сброс отстающих кадров
// - задержка при превышении FPS
class FPS_Controller
{
public:
	// maxFPS - максимальный FPS, если 0 - не отслеживается
	// threshold - во сколько раз может запаздать кадр чтобы все-таки нарисоваться, должен быть > 1
	ED_CORE_EXTERN FPS_Controller(float maxFPS = 0.f, float threshold = 2.f);

	// должно вызваться в начале обработки кадра
	ED_CORE_EXTERN void StartFrame();

	// проверка - укладывается ли текущий кадр в сроки
	ED_CORE_EXTERN bool IsFrameInTime();

	// распечатка отладочной информации
	ED_CORE_EXTERN void PrintStats();

	// установка параметров
	ED_CORE_EXTERN void SetThreshold(float threshold);
	ED_CORE_EXTERN void SetMaxFPS(float maxFPS);

protected:
	// рабочие переменные
	float thres;
	double min_period;
	double last_rt, cur_rt, last_dt;

	// статистика
	int frames, skiped;
	double sum_time;
};

extern ED_CORE_EXTERN FPS_Controller* globalFpsController;

#endif // FPS_Controller_H




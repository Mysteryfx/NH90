#pragma once
#include "ed/vector.h"
#include <functional>

//ведет статистику по таймерам, усредн€€ значени€ на каждом интервале timeDelta
class FrameStatistics
{
	typedef std::function<float()> GetterFunc;

	struct Item
	{
		ed::string	name;
		ed::vector<float> statistics;
		float		min, max;
		GetterFunc	getValue;
		Item(): min(99999999.f), max(-99999999.f) {}
	};

	long long startTime;
	long long lastTime;
	double time;//итоговое врем€ с момента начала сбора статистики
	double curTime;//текущее врем€ интервала
	double timeDelta;//длина интервала
	ed::vector<Item> items;//текущие счетчики
	ed::vector<unsigned> framesPerInterval;//количество кадров в каждом интервале

public:
			FrameStatistics();
	void	setStartTime();
	//установить шаг сбора и усреднени€ статистики, по умолчанию 1 секудна
	void	setTimeDelta(double delta);

	//подключение произвольного счетчика
	void	addItem(const char *name, GetterFunc getter);
	//подключаем ED_watch
	void	addWatch(const char *name);

	//ќбновить внутренее врем€ и состо€ние по счетчикам, при необходимости закрыть текущие интервалы и начать новые
	//‘ункци€ вызываетс€ один раз за кадр.
	void	gatherFrame();

	void	clear();

	void	saveAsCSV(const char *filename);
};
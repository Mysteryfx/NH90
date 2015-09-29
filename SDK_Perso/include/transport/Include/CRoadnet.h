#pragma once
#include "Transport.h"
#include "wTime.h"

// Экспериментальные классы для поиска оптимального пути
// в дорожной сети с использованием штатного будильника.
// Пока что в проекте не используются.

class CSpan;

class TRANSPORT_API CCross		// Перекресток, в т.ч. - тупик
{
public:
	static void Reset(CCross *c);	// сброс данных о прохождении
	float x, y;						// координаты
	unsigned int num;				// номер (служит только для идентификации)
	ed::string name;				// название (служит только для идентификации)
	bool bPassed;					// признак прохождения
	ed::vector<CSpan *> spans;		// примыкающие перегоны

	CCross();
	~CCross();
	void Print();
};

class TRANSPORT_API CPath		// Траектория в дорожной сети, передаваемая тараканами по эстафете
{
public:
	float lSum;						// суммарная длина
	double tSum;					// суммарное время прохождения
	ed::vector<CCross *> crosses;	// последовательность проходимых перекрестков

	CPath();
	CPath(CPath &);
	~CPath();
};

class TRANSPORT_API CSpan : public wTime		// Перегон между двумя перекрестками с тараканом
{
public:
	static CCross *start;			// стартовый перекресток забега
	static CCross *finish;			// финишный перекресток забега
	static float speed;				// скорость таракана
	static void Reset(CSpan *s);	// сброс данных о прохождении
	static void Pass(CSpan *from, CSpan *to);		// передача эстафеты

	CCross *cross1;					// начальный перекресток
	CCross *cross2;					// конечный перекресток
	float length;					// длина
	float condition;				// проходимость
	bool bPassed;					// признак прохождения
	CPath path;						// унаследованная по эстафете траектория

	CCross *curCross;				// текущий перекресток (куда бежать таракану)
	CSpan();
	~CSpan();
	void Print();
	void NextEvent();				// событие пробега перегона тараканом
};

class TRANSPORT_API CRoadnet	// Дорожная сеть из перекрестков и перегонов
{
public:
	static CPath *champion;			// маршрут-победитель
	static CSpan dummy;				// фиктивный стартовый перегон

	static void Test();

	ed::vector<CCross *> crosses;	// перекрестки
	ed::vector<CSpan *> spans;		// перегоны

	CRoadnet();
	~CRoadnet();
	void Build(int n, int con);		// Построение тестовой сети
	void Clear();					// Удаление перекрестков и перегонов перед новым постороением
	void Reset();					// Сброс данных о прохождении
	void Print();
	CPath *FindShortestWay(CCross *start, CCross *finish);
};

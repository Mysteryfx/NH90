// Если кому нужно вывести на экран свою отладочную информацию
#pragma once

class IDebugVisualizer
{
public:
	// функция для возврата отладочной информации
	typedef const char *funcDebugString(void *ud);

	// регистрация/разрегистрация
	virtual void reg(funcDebugString *f, void *ud) = 0;
	virtual void unreg(funcDebugString *f, void *ud) = 0;
};

extern INTER_API IDebugVisualizer *dbgVisualizer;

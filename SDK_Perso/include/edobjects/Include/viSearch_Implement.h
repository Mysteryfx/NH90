#ifndef viSearch_Implement_H
#define viSearch_Implement_H

#include "viSearch.h"

class EDOBJECTS_API viSearch_Implement : public viSearchCounted
{
protected:
	viFlag flags;
	ClipVolume *volume;

	viObject **store;
	unsigned int count, store_size;

	viSearchHandler handler;	
	void *data;
	bool finished;

public:
	viSearch_Implement(ClipVolume *_volume, viSearchHandler _handler, void *_data, viObject **_store, unsigned int _size, viFlag _flags);

	// типы объектов для поиска
	virtual viFlag Types() const;
	inline viFlag inTypes() const {return flags;}
	
	// объем
	virtual ClipVolume *Volume() const {return volume;}

	// возвращает true если можно продолжать поиск (false - окончание поиска)
	virtual bool TestAndInsert(viObject *item, bool contains);

	// преждевременное окончание поиска - для вызова из обработчика
	virtual void Finish(void);
	inline bool IsFinished() const {return finished;}

	// возврат результатов
	virtual viObject **Store(void);
	virtual unsigned int Count(void) const;
};

#endif // viSearch_Implement_H

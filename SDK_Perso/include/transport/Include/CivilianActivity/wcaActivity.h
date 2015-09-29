#ifndef wcaActivity_H
#define wcaActivity_H
#include "Registry/Registered.h"

// Интерфейс для мирных деятельностей
class wcaActivity
{
public:
	virtual ~wcaActivity() {};

	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void OnRoadObjectDestroyed(ObjectID bridgeId) {};
	
	virtual int Id() = 0;
	virtual void SetId(int) = 0;
};

#endif // wcaActivity_H
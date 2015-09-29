#ifndef wcaCar_H
#define wcaCar_H

#include "Transport.h"
#include "wsType.h"
#include "wMovingObject.h"
#include "Paths\lPath.h"
#include "viSearch_Implement.h"
#include "WorldSound.h"
#include "wActivityManager.h"
#include "CivilianActivity/wcaCarState.h"

// деятельность также является пространственным хранилищем для своих объектов - миражей
#include "HierarchyStorage\HierarchyNode.h"

class wcaCarState;
class wcaMirageActivity;
struct wsCollisionData;
// Машинки для использования в гражданских деятельностях
// Это - миражи, они не живут в мире, но могут выдать свое положение на любой момент времени
// При попадании на маршрут препятствия - либо остановятся, либо превратятся в полноценные машины
// При попадании по ним - должны превратится в нормальные обломки
class wcaCar : public MovingObject, public HierarchyNode
{
protected:
	wcaMirageActivity *route;	// мирная деятельность
	double initpar;		// начальный сегментный параметр на маршруте
	wcaCarState* _state;

	Position3 pos3;
	Graphics::Model *model;
	Graphics::DParamList draw_args;

	// для правильной отрисовки необходима некоторая дополнительная информация
	float X_gear_f; // Координаты нижней кромки переднего и заднего колеса
	float X_gear_b;
	float wheel_l;	// длина обода колеса

	float life;

	wActivityManager activities;
	Sound::Source beepSnd;
	wModelTime nextBeepTime;

	void startBeeping();
	void beepInJam(wModelTime& dt);

public:
	wcaCar(wcaMirageActivity *_route, double _initpar, wsType _type);
	virtual ~wcaCar();

	wcaMirageActivity *Owner() const {return route;}

	int	 Class(void) {return woClass_Vehicle;}
	
	// выдаст текущую позицию (по времени)
	virtual cPosition &Position(wModelTime t);
	
	// для информационной строки
	virtual	const float Hbar();
	virtual const float Course();

	// рассчет скорости
	virtual	float ScalarVelocity() const;
	virtual const cVector &VectorVelocity() const;
	virtual const float Omy();
	double GetPathPar(wModelTime t) const;

	// для отрисовки
	virtual const Position3& GetPosition();
	virtual const Graphics::DParamList& GetDrawArguments();
	virtual Graphics::Model *GetModel();

	TRANSPORT_API void removeFromWorld();

	virtual IntersectionType TestVolume(ClipVolume *volume);

	// парсинг (отрисовка, проверка столкновений и т.д.)
	virtual void Parse(Graphics::ModelParser& parser, const Position3& pos, Graphics::effectState* effects);

	// поддержка интерфейса объекта мира
	virtual void PostCollisionProcessing(wsCollisionData *cdata, unsigned int cid);

	// поддержка интерфейса wVulnerable
	virtual float Life() const;
	virtual float Life0() const;

	// поддержка интерфейса HierarchyNode
	// добавление/удаление объектов
	virtual bool AddObject(HierarchyNode *object) {return false;}
	virtual bool RemoveObject(HierarchyNode *object) {return false;}

	// получение статистики
	virtual unsigned GetCount() {return 1;}
	
	// сбор объектов
	virtual bool GetObjects(viSearch_Implement *search, IntersectionType it) {return true;}

	virtual void stop(wModelTime t);
	void update();

protected:
	// отдать концы
	virtual void procDeath(wModelTime at, ObjectID hitman);

private:
	friend class wcaManager_Implement;
	friend class wcaCarMoveState;
	friend class wcaCarStopState;

	// принять на грудь (пару ракет)
	void procDamage(float dmg, ObjectID hitman);

}; // class wcaCar

#endif // wcaCar_H

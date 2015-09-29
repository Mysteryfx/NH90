#ifndef viObject_H
#define viObject_H

#include "edObjects.h"

#include "cPosition.h"
#include "Renderer/VolumeEffect.h"

#include "wTime.h"

#include "Registry\Registered.h"

struct gBox;

namespace Graphics{
	class ModelParser;
}

class ClipVolume;
enum IntersectionType;

// Интерфейс для объектов в пространстве
class EDOBJECTS_API viObject : public Registered
{
private:
	// для поиска (чтобы избежать попадания одного объекта в список два раза)
	int searchID;

protected:
	// регистрация в пространственных структурах
	virtual bool viRegister();
	virtual bool viUnregister();

	virtual bool Register();
	virtual bool Unregister();

public:
	viObject(viFlag _type = viUnknown);
	virtual ~viObject();

	// эти функции в последствии должны быть заменены на ClipVolume
	virtual cPosition &Position(wModelTime t) = 0;
	virtual const gBox &LocalBox(wModelTime t) = 0;
	
	// парсинг (отрисовка, проверка столкновений и т.д.)
	virtual void Parse(
		Graphics::ModelParser& parser,
		const Position3& pos,
		Graphics::effectState* effects=0);


    // получить точную координату объекта
    virtual dVector GetPoint()
    {
        return Position(wTime::GetModelTime()).p;
    }

	// для поиска
	inline void SetSearchID(int sid) {searchID = sid;}
	inline bool IsSearch(int sid) {return (searchID == sid);}

	// тестируем на включение в объем
	virtual IntersectionType TestVolume(ClipVolume *volume);
	
	// необходимо перекрыть также как и Register()
	virtual bool ForceID(viFlag _type, ObjectID _id);
	virtual void		serialize(Serializer &serializer);
};

#endif // viObject_H

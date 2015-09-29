#ifndef __VI_OBJECT_H
#define __VI_OBJECT_H

#include "private/Base.h"
#include "ClipVolumes/IntersectionType.h"
#include "wTime.h"
#include "ClipVolumes/ClipVolumeBox.h"
#include "Modeler/gGeometry.h"
#include "viObjectFlatShape.h"

namespace vi
{
class Object;

// Обработчик объектов
// должен возвращать true если надо еще и вставить объект в список ???
typedef bool (*QueryHandler)(vi::Object& item, viSearch* search, void* data);

// поиск объектов по объемам (возвращают количество найденных объектов)
// by box:
inline int queryObjects( const osg::BoundingBox& sw, vi::QueryHandler query, void *data, viFlag flags = viAny);
// by frustum:
inline int queryObjects( const osg::Matrixd& cam, double len, double angle, vi::QueryHandler query, void *data, viFlag flags = viAny, double aspect = float(4.f/3.f));
// by segment:
inline int queryObjects( const osg::Vec3d& a, const osg::Vec3d& b, vi::QueryHandler query, void *data, viFlag flags = viAny);
// by abstract:
inline int queryObjects( ClipVolume *volume, vi::QueryHandler query, void *data, viFlag flags = viAny);


class Object : public vi::Base<viObject>
{
public:
	inline bool isValid();
	inline ObjectID ID();
	inline IntersectionType TestVolume(ClipVolume *volume);

// Устаревшие. Оставлены для совместимости
public:
	cPosition &Position(wModelTime t);
	const gBox &LocalBox(wModelTime t);
	// парсинг (отрисовка, проверка столкновений и т.д.)
	inline void Parse(
		Graphics::ModelParser& parser,
		const Position3& pos,
		Graphics::effectState* effects=0
		);

// inhouse data
protected:
	Object(viObject* instancedobject);
	Object(vi::StaticObject::STORAGE storage, uint32_t id);

	friend class vi::Manager;
	friend class viObjectManager_Implement;
};

}
#include "private/Manager.h"
#include "inl/Object.inl"

#endif

#ifndef ED_RENDER_ITEM_H
#define ED_RENDER_ITEM_H

#include "edtg.h"
#include "IGeometry.h"
#include "ed_log.h"

namespace edtg
{
struct IMaterial;

// Базовый класс для элемента отрисовки
// Каждая реализация IMaterial работает со структурой порожденной от RenderItem
// IMaterial ее создает в методе IMaterial::BindGeometry и рендерит в методе IMaterial::Render
//
struct RenderItem
{
	edtg::IMaterial* material;

	edtg::Geometry geometry;	// для проверки загружености
	ed::string geomstring;		// GeometryItem

	int32_t queue;              // очередь
	int32_t layer;				// слой отрисовки

	int32_t semantic;			// семантика
	int32_t level;				// 0-10 1-10 2-20 ...

	osg::BoundingBoxf boxWS;		// бокс в WS

	// typical data
	render::PRIMTYPE_ENUM primtype;
	int startprim, facecount;	
	int usedInFrame;

#ifdef DUMPRENDER
	ed::string matngeom_debug;	// имя материала и геометрии
#endif
	//...
	RenderItem()
	{
//printf("ri ");	// check forceloading
//ED_INFO("create render item");	// check forceloading
		layer=-1;
		material=0;
		startprim = facecount = -1;
		primtype = render::PT_NONE;
		usedInFrame = -1;
	}

	bool isLoaded()
	{
		return geometry.isLoaded();
	}

	// обязательно перекрывать в потомках!!!
	virtual ~RenderItem() {};
};

typedef ed::vector<RenderItem> RenderItemLayer;
}
#define EMPTYGEOMETRY ((RenderItem*)~0L)

#endif

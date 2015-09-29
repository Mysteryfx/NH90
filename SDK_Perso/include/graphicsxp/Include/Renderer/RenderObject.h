#ifndef RenderObject_H
#define RenderObject_H

#include "graphicsxp.h"
#include "position.h"
#include "DynamicParams.h"
#include "VolumeEffect.h"

//////////////////////////////////////
namespace Graphics
{

class Material;
class Geometry;
class Context;

typedef struct GeometryKey_t* GeometryKey;
typedef struct MaterialKey_t* MaterialKey;

#pragma pack(push, 4)

// Экземпляр куска с наложенным материалом для отправки на отрисовку
class RenderObject{
public:
	struct DistCompare
	{
		inline bool operator ()(const RenderObject *obj1, const RenderObject *obj2) const
		{
			return obj1->dist > obj2->dist;
		};
	};

	struct KeyCompare
	{
		inline bool operator ()(const RenderObject *obj1, const RenderObject *obj2) const
		{
			if(obj1->key.mkey == obj2->key.mkey) return obj1->key.gkey < obj2->key.gkey;
			return obj1->key.mkey < obj2->key.mkey;
		};
	};

	Geometry	*geometry;
	DParamList	geomParams;

	Material	*material;
	DParamList	matParams;
	
	effectState *estate;

	Position3	*pos;

	Context *context;
	//для правильного смешивания прозрачных объектов с облаками, если они могут это делать по-человечески
	float	radius;

	/// user data connected with this piece.
	void* data;

	union
	{
		float dist;
		struct{
			MaterialKey mkey;
			GeometryKey gkey;
		} key;
	};	
public:
	/// Creates render object in frame heap. You should not delete it.
	GRAPHICSXP_API static RenderObject* createInFrameHeap(Context *context);
	/// You must delete object after usage.
	GRAPHICSXP_API static RenderObject* createDynamically(Context *context);

	GRAPHICSXP_API RenderObject(Context *context = 0, bool frame_scope = false);
	GRAPHICSXP_API RenderObject(const RenderObject& arg);
			
	GRAPHICSXP_API virtual ~RenderObject();

	GRAPHICSXP_API void Prepare();
};



#pragma pack(pop)

}

#endif // RenderObject_H

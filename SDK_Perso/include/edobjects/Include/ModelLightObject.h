#ifndef ED_OBJECTS_MODEL_LIGHT_OBJECT_H
#define ED_OBJECTS_MODEL_LIGHT_OBJECT_H

#include "viLight.h"

namespace Graphics {
	class ModelInstance;
	class Model;
	class ModelLight;
}

class viObjectShape;

#define IID_IModelLightObject Common::identify<ModelLightObject>()

class ModelLightObject : public viLight
{
private:
	viObjectShape *host;
	Graphics::Model *model;
	Graphics::ModelLight *modelLight;
	unsigned int lightIndex;

	float size;
	dVector pos_p;
public:
    EDOBJECTS_API ModelLightObject();

	EDOBJECTS_API bool Init(viObjectShape *host, unsigned int lightIndex);

	// после выставления параметров
	EDOBJECTS_API virtual void Update();

	virtual void SetPosition(const Position3& pos){}
	// интерполированная позиция в МСК
	EDOBJECTS_API virtual const Position3& GetPosition();

    // получить точную координату объекта
    EDOBJECTS_API virtual dVector GetPoint();

	// размер с учетом припуска
	EDOBJECTS_API virtual float GetSize();


	EDOBJECTS_API virtual void PrepareFrame(const dVector& cam_origin);

};

#endif

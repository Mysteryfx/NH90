#ifndef __MODEL_LIGHTS_H__
#define __MODEL_LIGHTS_h__

#include "ed/vector.h"

class viObjectShape;

class ModelLightObject;

class ModelLights
{
private:
	ed::vector<ModelLightObject*> lights;
public:
	~ModelLights();

	void init(viObjectShape &host);
	void update();
	void destroy();	
};

#endif

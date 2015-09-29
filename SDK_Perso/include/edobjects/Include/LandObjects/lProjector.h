#pragma once
#include "lLandObject.h"
#include "viLight.h"

#define IID_lProjector Common::identify<lProjector>()
#define IID_lOmniLight Common::identify<lOmniLight>()

class EDOBJECTS_API lProjector : public lLandObject
{
protected:
	viLight* light;

	void UpdateLightParams();

public:
	lProjector();
	~lProjector();

    virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);

	void serialize(Serializer &);
};

class EDOBJECTS_API lOmniLight : public lLandObject
{
protected:
	viLight* light;

	void UpdateLightParams();

public:
	lOmniLight();
	~lOmniLight();

    virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);

	void serialize(Serializer &);
};

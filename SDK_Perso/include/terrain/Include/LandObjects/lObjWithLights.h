#pragma once

#include "Terrain.h"
#include "lStandartStructure.h"
#include "lLightSource.h"

#define IID_lObjWithLights Common::identify<lObjWithLights>()

// Прожектор
// ищет хелперы с именем:
// light [proj|omni] <RGB> <attenuation> [<angle>]
class TERRAIN_API lObjWithLights : public lStandartStructure
{
public:
	static lLandObject* CreateObject();

	~lObjWithLights();

    virtual void Init(wShape *_type, const cPosition& _pos, viFlag status = viUnknown, void* context=NULL,double _scale = 1.0);

	// если объект получил достаточно повреждений, чтобы деградировать
	virtual void Degrade(bool silent);

	// восстановить исходное состояние (если создавалась dyndata)
	void ReinitState();

protected:
	class Descriptor : public lLightSource::Callback
	{
	public:
		Descriptor(lObjWithLights *self, Graphics::ModelConnector *connector);
		~Descriptor();

		void UpdateLight();
		virtual void onGetLuminance(lLightSource *light);
		virtual void onPrepareFrame(lLightSource *light);

	protected:
		wModelTime update_t_;
		lObjWithLights *self_;
		Graphics::ModelConnector *connector_;
		Link<lLightSource> light_;
	};

	typedef ed::vector<Descriptor *> lightList;
	lightList lights_;
	Graphics::DParamList dparams_;

	void CreateLights();
	void DestroyLights();
};

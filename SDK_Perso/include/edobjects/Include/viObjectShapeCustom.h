#pragma once

#include "viObjectFlatShape.h"
#include "ClipVolumes/ClipVolumeBox.h"

// Интерфейс для рисуемых объектов
class EDOBJECTS_API viObjectShapeCustom : public viObjectFlatShape
{
private:
	cPosition pos;
	Position3 pos3;
	ClipVolumeBox flatshape;

	// создание flatshape по позиции и gShapeObject
	virtual void ConstructFlatShape();

public:
	viObjectShapeCustom(
		const ed::string& modelname);

    ~viObjectShapeCustom();

	void Init(viFlag _type = viUnknown);
	// изменить положение объекта, вызовет перерегистрацию (невозможно для viLandObject)
	virtual void SetPos(const cPosition& _pos);

	// поддержка интерфейса ModelInstance
	virtual const Position3& GetPosition() {return pos3;}

	// поддержка интерфейса viObject
	virtual cPosition &Position(wModelTime t){return pos;};
	virtual ClipVolume *FlatShape();
};

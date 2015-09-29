#ifndef viLight_H
#define viLight_H

#include "viObject.h"
#include "Renderer/VolumeEffect.h"
#include "MovingObjects\MovingHandle.h"
#include "GraphMath/Box.h"
#include "Modeler/gGeometry.h"

// Объект - источник света
class EDOBJECTS_API viLight : public viObject, public MovingHandle, public LinkHost
{
protected:
	Graphics::VolumeEffect *light;
	Position3 pos3;
	Box box;

	gBox gbox;
	cPosition cpos;

public:
	static viLight *Create(const ed::string& lightClass = "");

	viLight();

	// Передается технологический тип источника света
	// "Light" - стандартная повертексная освещенка
	// "ProjLight" - проекционная текстура - прожектор
	// "OmniLight" - попиксельная освещенка от точечного источника света (GF3+)
	virtual bool Init(const ed::string& lightClass = "Light");
	~viLight();

	// получить степень освещения точки этим источником света
	virtual Vector3 GetLuminanceAt(const Vector3& p);

	// возвращает VolumeEffect 
	Graphics::VolumeEffect* GetVolumeEffect();
	// возвращает интерфейс для настройки параметров источника света
	Graphics::Light *QueryEditor();

	// после выставления параметров
	virtual void Update();
	
	// при попадании этого эффекта в кадр
	// передается начало координат отрисовки
	virtual void PrepareFrame(const dVector& cam_origin);

	// выставить позицию
	virtual void SetPosition(const Position3& pos);
	virtual const Position3& GetPosition();

	// дописать в efffectState
	virtual void AddToEffects(Graphics::effectState *effects);

	// интерфейс viObject
	virtual cPosition &Position(wModelTime t);
	virtual const gBox &LocalBox(wModelTime t);
	virtual IntersectionType TestVolume(ClipVolume *volume);

	virtual float GetSize();

	virtual void serialize(Serializer &);
};

#endif // viLight_H

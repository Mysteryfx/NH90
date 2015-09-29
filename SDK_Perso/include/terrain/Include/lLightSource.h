#ifndef lLightSource_H
#define lLightSource_H

//#include "renderer/IRenderAPI.h"
#include "Terrain.h"
#include "viLight.h"

// Light source for static objects
// Can be animated on demand, and has some movement threshold
class TERRAIN_API lLightSource : public viLight
{
public:
	// on-demand animation callbacks
	class Callback
	{
	public:
		virtual void onGetLuminance(lLightSource *light) = 0;
		virtual void onPrepareFrame(lLightSource *light) = 0;
	};

	static lLightSource *Create(const ed::string& lightClass, float moveThreshold);

	lLightSource();
	~lLightSource();

	// setup callback
	virtual void SetCallback(Callback *cb);
	virtual Callback *GetCallback();

	// on/off
	virtual void Turn(bool on);

	// Передается технологический тип источника света
	// "Light" - стандартная повертексная освещенка
	// "ProjLight" - проекционная текстура - прожектор
	// "OmniLight" - попиксельная освещенка от точечного источника света (GF3+)
	// moveThreshold - бокс расширяется на это значение, позволяя двигать объект в этих пределах on demand
	virtual bool Init(const ed::string& lightClass, float moveThreshold);

	// получить степень освещения точки этим источником света
	virtual Vector3 GetLuminanceAt(const Vector3& p);

	// после выставления параметров
	virtual void Update();

	// при попадании этого эффекта в кадр
	// передается начало координат отрисовки
	virtual void PrepareFrame(const dVector& cam_origin);

	// выставить позицию
	virtual void SetPosition(const Position3& pos);

	// дописать в efffectState
	virtual void AddToEffects(Graphics::effectState *effects);

	// viObject interface
	virtual IntersectionType TestVolume(ClipVolume *volume);

	// MovingHandle interface
	virtual float GetSize();

	// Serializable interface
	virtual void serialize(Serializer &serializer);

protected:
	Vector3 anchor_;
	float moveThreshold_;
	Callback *callback_;
	bool on_;
};

#endif // lLightSource_H

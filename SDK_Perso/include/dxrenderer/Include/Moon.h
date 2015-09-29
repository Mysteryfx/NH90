#ifndef DXRenderer_Moon_H
#define DXRenderer_Moon_H

#include "DXLight.h"

namespace Graphics
{

// Луна, как источник света
class Moon : public DXLight
{
protected:
	void Update();

public:
	// фабрика данного ресурса
	static Resource *Create(ResourceType type, const ed::string& className);

	// конструктор/деструктор
	Moon();

	// Интерфейс Effect
	virtual void UpdatePos(const Position3& _pos);
	virtual float GetRadius()	{return 1e10f;}
	virtual void Apply(RenderObject *robject);

	// Интерфейс Resource
	virtual const ed::string& ClassName();
};

} // namespace Graphics

#endif // DXRenderer_Moon_H
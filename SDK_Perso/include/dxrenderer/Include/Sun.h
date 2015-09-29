#ifndef DXRenderer_Sun_H
#define DXRenderer_Sun_H

#include "DXLight.h"
#include "Modeler\Animation.h"

namespace Graphics
{
// Солнце, как источник света
// цвет берется из текстового файла и вычисляется по высоте над горизонтом
// Динамические параметры:
// 0. Множитель для Diffuse
// 1. Множитель для Specular
class Sun : public DXLight
{
protected:
	
	// анимация цвета солнца в зависимости от высоты над горизонтом
	AnimationVector  ambient_anim, diffuse_anim;

	void ReadColorData(const char *filename);

	// получить цвет общей освещенности
	Vector3 GetDiffuse();
	Vector3 GetAmbient();
	
	// обновить информацию о Солнце
	void UpdateSun();

public:
	// фабрика данного ресурса
	static Resource *Create(ResourceType type, const ed::string& className);

	// конструктор/деструктор
	Sun();
	virtual ~Sun();

	// Интерфейс Effect
	virtual void UpdatePos(const Position3& _pos);
	virtual float GetRadius()	{return 1e10f;}
	virtual void Apply(RenderObject *robject);

	// Интерфейс Resource
	virtual const ed::string& ClassName();
};

} // namespace Graphics

#endif // DXRenderer_Sun_H

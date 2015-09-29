#ifndef DrawPrimitive_H
#define DrawPrimitive_H

#include "ExpImpSpec.h" 

#include "Renderer\RenderObject.h"
#include "Renderer\Color.h"
#include "vector.h"

// Примитив для отрисовки из интерфейса
// Обертка для упрощения использования
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR DrawPrimitive : public Graphics::RenderObject
{
public:
	DrawPrimitive(bool screen_space = true);
	~DrawPrimitive();

	// задание геометрии
	void CreateBox(float x1,float y1,float  x2,float y2);
	void CreateBoxRotated(float x1,float y1,float  x2,float y2,float rotation);
	void CreateLines(const ed::vector<Vector3>& points);
	void CreateCircle(int cx, int cy, int radius, int nSeg = 16);
	void Rotate(float cx, float cy, float radians);

	// задание вида поверхности
	void SetColor(const Graphics::Color& color);
	void SetColor(float r, float g, float b, float a = 1);
	void SetTexture(const ed::string& texname);
	void SetTexCoords(float u1, float v1, float u2, float v2);
	void SetLinearTextureFiltration(bool val);
	bool IsLinearTextureFiltration() const;
	void DeleteTexture();
	
	// получение параметров
	const ed::vector<Vector3>& GetPoints();
	const ed::vector<Vector3>& GetTexCoords();

	// операции
	void Load();	// полностью загрузить текстуру
	void Draw();	// отправить на отрисовку
};

#endif // DrawPrimitive_H
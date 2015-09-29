#ifndef RenderText_H
#define RenderText_H

#include "graphicsxp.h"
#include "Box.h"
#include "Color.h"
#include "position.h"
#include <ed/string.h>

namespace Graphics
{

class RenderFont;

// текст для отправки на отрисовку
class RenderText
{
public:

    GRAPHICSXP_API RenderText();

	// выравнивание
	enum Justify
	{
		tjLower  = 1,
		tjCenter = 2,
		tjUpper  = 4,
	};

	// расположение относительно точки p
	Justify horJust, vertJust;

	//смещение между строками
	int interval;

	// текст для вывода
	ed::string text;

	// ссылка на логический шрифт
	RenderFont *font;

	// точка, от которой рисуется текст, использую Justify
	Vector3 p;

    // для текста в произвольной системе кординат
    Position3 pos;

	// регион, в котором виден текст
	::Box   clip;

    Color color;
    
    bool  inWorldSpace;
};

}

#endif // RenderText_H

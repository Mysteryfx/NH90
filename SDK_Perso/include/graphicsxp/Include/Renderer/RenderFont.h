#ifndef RenderFont_H
#define RenderFont_H

#include "Resourcer\Resource.h"
#include "vector.h"


namespace Graphics
{

class RenderText;

// Интерфейс шрифта
class RenderFont : public Resource
{
public:
	// свойства шрифта
	enum Flags
	{
		ffItalic		= 1,
		ffBold			= 2,
		ffUnderlined	= 4,
		ffStrikeOut		= 8,
	};


	RenderFont()	{rtype = rtFont;}
	virtual ~RenderFont(){}

	// установка параметры шрифта
	virtual void SetFontName(const ed::string& fontName) = 0;
	virtual void SetSize(float size) = 0;
	virtual void SetFlags(unsigned flags) = 0;

	// получение параметров
	virtual const ed::string& GetFontName() = 0;
	virtual float GetSize() = 0;
	virtual unsigned GetFlags() = 0;

	// получить габариты строки
    virtual void GetTextExtent(const char       * text, size_t n ,Vector3& extent) = 0;

	// Вывести текст данным шрифтом
	virtual void DrawRenderText(RenderText *rtext, bool bDontUseGlobalVP=false) = 0;
};

}

#endif // RenderFont_H

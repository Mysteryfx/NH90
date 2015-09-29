#ifndef DrawText_H
#define DrawText_H

#include "Renderer\RenderText.h"

// Обертка для объекта - текста
// Для интерфейса
class DrawUIText : public Graphics::RenderText
{
public:
	DrawUIText();
	~DrawUIText();
	
	// инициализация всевдонимов шрифтов
	static bool InitFonts(const ed::string& filename = ".\\FUI\\Fonts.cfg");
	
	// настройка параметров
	void SetText(const ed::string& text);
	void SetColor(const Graphics::Color& color);
	void SetFont(const ed::string& type, float size, unsigned flags);	// flags - enum Graphics::RenderFont::Flags (перечислены в конце этого файла)
	void SetHorJust(Justify horJust);
	void SetVertJust(Justify verJust);
	void SetDrawRect(int x1, int y1, int x2, int y2);
	void SetClipRect(int x1, int y1, int x2, int y2);

	// получение информации
	const ed::string& GetText();
	void GetDrawRect(int &x1, int &y1, int &x2, int &y2);
	void GetClipRect(int &x1, int &y1, int &x2, int &y2);

	// вернуть размер текста без учета выравнивания
	void GetTextExtent(float &x, float &y);

	// операции
	int	 Draw();
	
private:
	int _x1, _y1, _x2, _y2;
};
//ffItalic		= 1
//ffBold		= 2
//ffUnderlined	= 4
//ffStrikeOut	= 8

#endif // DrawText_H
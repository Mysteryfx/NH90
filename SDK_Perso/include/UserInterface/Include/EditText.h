#pragma once

#include <ed/vector.h>
#include <ed/string.h>
#include <utility>
#include "EditString.h"
#include "IntPair.h"

class EditText : public EditString
{
protected:
	ed::vector<int>		stringsEnds;
	float					maxStringPixelLength;

protected:
	
	DrawUIText*				getDrawText() { return draw; }

public:
	EditText(const char* inText = 0, int inTextLength = -1);
	~EditText();

	float					getCharWidth(unsigned int symbol); // NOTE: resets this->draw

	void					setMaxPixelLength(float _maxStringPixelLength);
	float					getMaxPixelLength() const {return maxStringPixelLength;}
	int						wordWrap(int fromLine = 0);

//По умолчанию берется весь текст	
	int						getFirstSymbolNum(int line) const;//Номер символа в оригинальном тексте, соответствующий первому символу строки
	int						getLastSymbolNum(int line) const; //Номер символа в оригинальном тексте, соответствующий следующему за последним символу строки
	int						getWrappedLinesNumber() const;
	ed::string				getWrappedLine(int line) const;
	ed::string				getWrappedText(int firstLine = -1, int lastLine = -1) const;
	void					deleteWrappedLine(int line);
	
	int						columnRowToPos(int column, int row) const;
	IntPair					posToColumnRow(int pos, bool afterSymbol = false) const;

	virtual bool			refresh();

	float					getStringPixelHeight();
};

#pragma once

#include "MultiStatic.h"

struct MultilineEditBoxParameters: public MultiStaticParameters
{
public:
	MultilineEditBoxParameters(int preset = 0);
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(MultilineEditBoxParameters)
	virtual void serialize(Serializer &);

	typedef MultiStaticParameters Base;	

	int				maxBodySize;// максимальный размер текста
	int				caretWidth;
	int				caretAdditionalHeight;
	unsigned		caretColor;
	void			read(const char*, int);
};

typedef TPoint<int>		IPoint;

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MultilineEditBox : public MultiStatic
{
public:
	typedef MultiStatic Base;
	typedef MultilineEditBoxParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()
	
	void					thisArrange(const Parameters& prms);

protected:
	int						maxBodySize;

	int						caretWidth;
	int						caretAdditionalHeight;
	unsigned				caretColor;
	int						caretBlinkDelay;
	int						lastTimeOfCaretDrawing;
	bool					needToFocusOnCaret;

	int						caretPos;
	bool					caretAfterLastSymbol;
	DrawPrimitive*			caret;

	bool					needToUpdateCaret;

	// Инициализация
	void					init();	

	// Обработка событий
	void					onLButtonDown(int keys, int x, int y);	
	void					onLButtonUp(int keys, int x, int y);
	void					onLButtonDblclk(int keys, int x, int y);
	void					onMouseMove(int keys, int x, int y);	
	void					onKeyDown(int virtKey, int keyData);
	void					onKeyUp(int virtKey, int keyData);
	void					onChar(int charCode, int keyData);
	void					onPaint();
	void					onMove(int dx, int dy);
	void					onCreate(Parameters* prms);
	void					onSetFocus(Control* losingFocus);
	void					onKillFocus(Control* receivingFocus);

	virtual UndoRedoAction	undoRedo(int mode, int invertMode);

protected:
	virtual void			drawCaret() const;
	void					updateCaret();
	void					setNeedToUpdateCaret(bool need);
	virtual bool			refreshText();

public:
	MultilineEditBox(const Parameters& params) : Base(params) { init(); thisArrange(params); }
	MultilineEditBox(int tag, float x = 0.f, float y = 0.f, float X = 0.f, float Y = 0.f);
	virtual	~MultilineEditBox();

	void					setFont(const ed::string& fontType, int fontSize, int flags);

	// Cursor
	void					setCaretAdditionalHeight(float newHeight);
	int						getCaretAdditionalHeight() const {return caretAdditionalHeight;}
	void					setCaretWidth(float newWidth);
	int						getCaretWidth() const {return caretWidth;}
	void					setLastTimeOfCaretDrawing(unsigned lastTime);
	void					setCurrentTimeAsLastTimeOfCaretDrawing();
	unsigned				getLastTimeOfCaretDrawing() const {return lastTimeOfCaretDrawing;}
	void					setNeedToFocusOnCaret(bool need);
	int						getCaretRow() const;
	void					shiftCaretRow(int rowsNum);

	void					setCaretPos(int inPos, bool afterLastSymbol = false);
	int						getCaretPos() const;
	bool					getCaretAfterLastSymbol() const {return caretAfterLastSymbol;}
	void					focusOnCaret();

	// Оформление
	void					arrange(const MultilineEditBoxParameters& prms);
	void					arrange(const ed::string& name) NAMEARRAGNGE(Parameters)
	virtual void			setCoordinates(float rx, float ry, float rX, float rY);

	void					setMaxTextSize(int inMaxSize = 0x7fff) { maxBodySize = inMaxSize; }
	int						getMaxTextSize() const{ return maxBodySize; }

	virtual void			deleteSelection();
		
	void					cut();	// Cut selection to Clipboard.
	void					paste();// Insert text from Clipboard.
	void					insertToCaretPos(const char* text);
};

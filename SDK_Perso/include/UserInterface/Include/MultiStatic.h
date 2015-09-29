#ifndef __USERINTERFACE_MULTISTATIC_H__
#define __USERINTERFACE_MULTISTATIC_H__

#include "ControlWithScrollBar.h"
#include "IntPair.h"
#include "UI/Editor.h"

#include <stack>

struct MultiStaticParameters: public ControlWithScrollBarParameters
{
public:
	MultiStaticParameters(int preset = 0);
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(MultiStaticParameters)

	virtual void	serialize(Serializer &);
	void			read(const char* configText, int configSize);

	ed::string		text;

	ed::string		fontType;
	float			fontSize;
	unsigned		fontFlags;
	unsigned int	fontColor;

	unsigned int	selectionFontColor;
	unsigned int	selectionColor;

	typedef ControlWithScrollBarParameters Base;	
};

class DrawUIText;
class DrawPrimitive;
class EditText;

typedef ed::list<DrawUIText*> DrawStrings;
typedef ed::list<DrawPrimitive*> DrawBoxes;

struct UndoRedoAction
{
	enum
	{
		actionNone = -1,
		actionInsert = 0,
		actionRemove
	};
	
	UndoRedoAction()
	{
		actionType = actionNone;
	}

	int actionType;
	int pos;
	ed::string removedString;
	int size;
};

typedef std::stack<UndoRedoAction> ActionStorage;

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR MultiStatic : public ControlWithScrollBar, public UI::Editor
{
public:
	typedef ControlWithScrollBar Base;
	typedef MultiStaticParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()

protected:
	
	bool					needToUpdate;//Произошло событие, после которого надо обновить отображение строк
	EditText*				text;
	DrawStrings				allText;
	DrawBoxes				selectionBoxes;

	int						lineHeight;
	int						numberOfVisibleStrings;

	int						selectionBeginPos;
	int						selectionEndPos;

	int						firstSelectedPos;
	bool					selectionOn;
	
	unsigned int			selectionFontColor;
	unsigned int			selectionColor;

	enum
	{
		undoRedoMode_none = 0,
		undoRedoMode_undo,
		undoRedoMode_redo
	};

	ActionStorage			undoActions;
	ActionStorage			redoActions;
	int						undoRedoMode;

	void					thisArrange(const Parameters& params);
	void					init();	

	void					calculateNumberOfVisibleStrings();
	DrawUIText*				createDrawString(const ed::string& text = ed::string("")) const;
	void					createDrawStrings();
	void					clearDrawStrings();

	void					setNeedToUpdate(bool need = true);
	void					noNeedToUpdate();

	virtual bool			refreshText();

	float					getNextLineY(float currentLineY) const;

	void					setFirstSelectedPos(int pos);
	int						getFirstSelectedPos() const {return firstSelectedPos;}
	void					deselect();
	void					tuneSelection();

	void					setSelectionOn(bool selection);
	bool					getSelectionOn() const {return selectionOn;}

	void					choseCursor() const;

	void					setUndoRedoMode(int undoRedoMode);
	int						getUndoRedoMode() {return undoRedoMode;}
	void					undoAction(const UndoRedoAction& action);
	virtual UndoRedoAction	undoRedo(int mode, int invertMode);
	void					addAction(const UndoRedoAction& action);
	void					addRemoveAction(int pos, const ed::string& removedString);
	void					addInsertAction(int pos, int size);
	ActionStorage*			getCurrentActionStorage();

public:
	MultiStatic(const Parameters& params) : Base(params)
	{
		init();
		thisArrange(params);
	}

	MultiStatic(int tag, float x = 0.f, float y = 0.f, float X = 0.f, float Y = 0.f);

	virtual					~MultiStatic();

	void					setContent(const char* inContent);
	void					addString(const char* inString);
	void					delString(int index);
	ed::string				getString(int index) const;

//Интерфейс EditString
//---------------------------------------
	virtual void			clear();
	virtual void			assign(const char* inString, int inStringLength = -1);
	virtual void			insert(int inPosBefore, const char* inSubString, int inStringLength = -1);
	virtual void			insert(int inPosBefore, char inChar);
	virtual void			add(char inChar);
	virtual void			add(const char* inSubString, int inStringLength = -1);
	virtual void			remove(int inPosRemove, int inNumberChars = 1);
	virtual void			removeFromTo(int firstSymbol, int lastSymbol)
								{remove(firstSymbol, lastSymbol - firstSymbol + 1);}
	virtual ed::string		getSubstring(int pos, int length) const;
	virtual ed::string		getSubstringFromTo(int first, int last) const;
//---------------------------------------
    virtual void			scroll(int numOfStr);

	int						getNumberOfStrings();
	int						getNumberOfVisibleStrings() const { return numberOfVisibleStrings; }
	int						getFirstVisibleStringNum() const;
	int						getLastVisibleStringNum() const;
	int						getTextLength() const;
	int						getLineHeight() const {return lineHeight;}

	void					empty() { clear(); }

	void					setFontColor(unsigned int color);
	void					setFont(const ed::string& fontType, float fontSize, unsigned flags);

	virtual void			setCoordinates(float rx, float ry, float rX, float rY);
	virtual void			shift(float dx, float dy);
	void					arrange(const Parameters& params)
	{
		Base::arrange(params);
		thisArrange(params);
	}
	virtual void			inflateSelf();

	void					setSelectionBegin(int pos);
	void					setSelectionEnd(int pos);
	void					setSelection(int begin, int end);
	void					noSelection();
	int						getSelectionBegin() const {return selectionBeginPos;}
	int						getSelectionEnd() const {return selectionEndPos;}
	ed::string				getSelection() const;
	virtual void			deleteSelection();
	bool					hasSelection() const;
	
	void					setSelectionFontColor(unsigned int selectionFontColor);
	unsigned int			getSelectionFontColor() const {return selectionFontColor;}

	void					setSelectionColor(unsigned int selectionColor);
	unsigned int			getSelectionColor() const {return selectionColor;}

	IntPair					xyToColumnRow(float x, float y, bool* after = 0) const;
	int						xToColumn(float x, int row, bool* after = 0) const;
	int						xyToPos(float x, float y, bool* after = 0) const;
	int						xyToSelectionPos(float x, float y, bool* after = 0) const;
	bool					posToXY(int pos, float& x, float& y, bool afterSymbol = false) const;

	void					copy() const;
protected:
	void					onKeyDown(int virtKey, int keyData);
	void					onSetFocus(Control* losingFocus);
	void					onKillFocus(Control* receivingFocus);
	void					onVScroll(int scrollCode, int scrollBoxPos, Control* scrollBar);
    void					onLButtonDown(int keys, int x, int y);
	void					onMouseMove(int keys, int x, int y);
	void					onLButtonUp(int keys, int x, int y);
    void					onMove(int dx, int dy);
	void					onPaint();
	void					onMouseWheel(short keys, short zDelta, int x, int y);
	void					onClose();

	void					undo();
	void					redo();
	void					enableUndo();
	void					disableUndo();
	void					clearRedoStorage();
	void					clearUndoStorage();

public:
	// UI::Editor interface
	virtual void					setText(const UI::Editor::Text&);
	virtual const UI::Editor::Text	getText() const;
	virtual void					freeze(bool = true);
	virtual void					disable(bool);
};

#endif // __USERINTERFACE_MULTISTATIC_H__

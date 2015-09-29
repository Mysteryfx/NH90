#ifndef __USERINTERFACE_EDITSTRING_H__
#define __USERINTERFACE_EDITSTRING_H__

class DrawUIText;
class EditString
{
protected:
	ed::string				text;
	DrawUIText*				draw;
	int						modifiedPos;
	int						deletionPos;
	
public:	
	
	ed::string				fontType;
	float					fontSize;
	unsigned				fontFlags;

//--------------- Старая графика --------------------------------------		
	float					x;
	float					y;

	float					px;
	float					py;
//---------------------------------------------------------------------

	int						caretPosition;

//--------------- Старая графика --------------------------------------		
	int						font;
	int						pixelLength;
	int						pixelHeight;
//---------------------------------------------------------------------

	float					fontPSize;
	unsigned int			fontColor;
public:
	EditString(const char* txt = 0, int inStringLength = -1);
  EditString(const EditString& other);
	~EditString();

	const ed::string&		getText() const { return text; }
	int						getTextLength() const { return (int) text.size(); }
	bool					empty() const { return text.empty(); }

	DrawUIText*				getDrawText() { return draw; }
	
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
	
	static	unsigned		setFlags(unsigned flagsToSet, bool set, unsigned& flags);
	unsigned				setFlags(unsigned flagsToSet, bool set);
	
	char					operator[] (int inIndex) { 	return text[inIndex]; }
	
	unsigned				setFontFlags(unsigned flagsToSet);
	unsigned				getFontFlags() const {return fontFlags;}

	void					setFontType(ed::string fontType);
	const ed::string&		getFontType() const {return fontType;}

	void					setFontSize(float size);
	float					getFontSize() const {return fontSize;}

	void					setFont(const ed::string& fontType, float fontSize, unsigned flags);

	virtual void			setModified(int modifiedPos = 0);
	virtual void			setNoModifications();
	virtual void			setDeletionPos(int deletionPos = 0);
	virtual void			setNoDeletions();

	virtual bool			refresh(){return true;}

	bool					isTextModified() const {return (modifiedPos >= 0);}
	bool					wasDeletion() const {return (deletionPos >= 0);}
};

#endif // __USERINTERFACE_EDITSTRING_H__
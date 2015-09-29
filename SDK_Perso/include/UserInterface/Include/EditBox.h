#ifndef __USERINTERFACE_EDITBOX_H__
#define __USERINTERFACE_EDITBOX_H__

#include "Static.h"
#include "UI/Editor.h"
#include "utilities.h"

struct EditBoxParameters: public StaticParameters
{
public:
	EditBoxParameters(int preset = 0);
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(EditBoxParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;	

	int				m_body_type;	// тип вводимой строки (string - любые символы, int_number, real_number)
	int				m_max_body_size;// максимальный размер вводимой строки
	double			m_minValue, m_maxValue;
	unsigned		digits;
  unsigned password;

	void			read(const char*, int);
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR EditBox : public Static, public UI::Editor
{
public:
	typedef Static Base;
	typedef EditBoxParameters Parameters;

private:
	DECLARE_MESSAGE_TABLE()
	
	void					thisArrange(const EditBoxParameters& prms);

protected:
	int						m_body_type;
	int						m_max_body_size;
	double					m_minValue, m_maxValue;
	unsigned				digits;
  unsigned        password;

	class Implementation;
	friend class Implementation;

	Implementation*			self;

	// Инициализация
	void					init();	
	virtual void			fitPCaption();

	// Обработка событий
	void					onLButtonDown(int keys, int x, int y);	
	void					onLButtonUp(int keys, int x, int y);
	void					onLButtonDblclk(int keys, int x, int y);
	void					onMouseWheel(short keys, short zDelta, int x, int y);
	void					onMouseMove(int keys, int x, int y);	
	void					onKeyDown(int virtKey, int keyData);
	void					onKeyUp(int virtKey, int keyData);
	void					onChar(int charCode, int keyData);
	void					onPaint();
	void					onMove(int dx, int dy);
	void					onCreate(EditBoxParameters* prms);
	void					onSetFocus(Control* losingFocus);
	void					onKillFocus(Control* receivingFocus);
protected:
	virtual void			showCursor();

	virtual bool			isAvailableChars(int inChar);
	template <class T> bool checkValue(T inMin, T inMax)
	{
		T aValue;
		ed::istringstream aInput(caption->getText());
		aInput >> aValue;
		bool aRetCode = (!aInput.fail() && inside(aValue, inMin, inMax) == true);

		ed::ostringstream aOutput;
		aValue = ::clamp(aValue, inMin, inMax);
		aOutput << aValue;
		setText(aOutput.str().c_str());

		return aRetCode;
	}
	template <class T> bool tGetValue(T& value);
public:
	EditBox(const Parameters& params) : Base(params) { init(); thisArrange(params); }
	EditBox(int tag, float x = 0.f, float y = 0.f, float X = 0.f, float Y = 0.f);
	virtual	~EditBox();

	// Caption
	const char*				getCaption() const;	
	void					setCaption(const char* text, int pos = caption_left);
	void					empty();

	// Cursor
	void					setCaretSize(int newWidth, int newHeight);
	Point					getCaretSize();
	void					setCaret(int inPos, bool inSelect = false);
	int						getCaret() const;
	int						getSelSize() const;

	// Оформление
	void					arrange(const EditBoxParameters& prms);
	void					arrange(const ed::string& name) NAMEARRAGNGE(Parameters)
	virtual void			setCoordinates(float rx, float ry, float rX, float rY);

	void					setBodyType(int inType = edit_string);
	int						getBodyType() const { return m_body_type; }
	void					setMaxTextSize(int inMaxSize = 256) { m_max_body_size = inMaxSize; }
	int						getMaxTextSize() const{ return m_max_body_size; }

	double					getMinValue() const { return m_minValue; }
	double					getMaxValue() const { return m_maxValue; }
	void					setRange(double inMun, double inMax) { m_minValue = inMun; m_maxValue = inMax; }

	void					setDigits(unsigned digits);
	unsigned				getDigits() const {return digits;}
	void					updateDigits();

  void					setPassword(unsigned password);
	unsigned				getPassword() const { return password; }

	/* UI::Editor interface */
	virtual void			setText(const UI::Editor::Text&);
	virtual const UI::Editor::Text	getText() const;
	virtual void			activate();
	virtual void			freeze(bool = true);
	virtual void			disable(bool);
	virtual bool			checkText();
	
	void					getValue(int& value){tGetValue(value);}
};

template <class T> bool EditBox::tGetValue(T& value)
{
	T aValue;
	ed::istringstream aInput(caption->getText());
	aInput >> aValue;
	bool aRetCode = (!aInput.fail() && inside(aValue, (T)(getMinValue()), (T)(getMaxValue())) == true);
	
	ed::ostringstream aOutput;
	aValue = ::clamp(aValue, (T)(getMinValue()), (T)(getMaxValue()));
	aOutput << aValue;
	setText(aOutput.str().c_str());
	value = aValue;
	
	return aRetCode;
}

#endif // __USERINTERFACE_EDITBOX_H__

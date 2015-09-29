#pragma once

#include <ed/string.h>
#include <ed/vector.h>
#include "ui_Dialog.h"
#include "UI/Button.h"
#include "Signal/Notifier.h"

//------------------------------------------------------------------------
// тип строки
typedef enum
{
	k_Delimiter,
	k_Line

} LineType;
//------------------------------------------------------------------------
typedef ed::vector<ed::string>	StringsVector;	

struct  ControlData	
{
	ed::string			Name;
	int					Tag;
	StringsVector		Data;
public:
	ControlData()
	{
	}
	ControlData(const char* inName, int inTag, 
		const char* inFirst = 0, const char* inSecond = 0) : Name(inName), Tag(inTag)
	{
		if (inFirst)
			Data.push_back(inFirst);
		if (inSecond)
			Data.push_back(inSecond);
	}
	void	push_back(const char* inDataMember)
	{
		Data.push_back(inDataMember);
	}
};
typedef ed::vector<ControlData>	ControlDataVector;
//------------------------------------------------------------------------
struct ControlTagsAndXLeft
{
	int				m_Tag;
	int				m_XLeft;
public:
	ControlTagsAndXLeft(int inTag, int inXleft, bool inTagSortOn = true)
		: m_Tag(inTag), m_XLeft(inXleft)
	{
	}
	friend bool			operator < (const ControlTagsAndXLeft& inA, 
									const ControlTagsAndXLeft& inB)
	{
			return (inA.m_XLeft < inB.m_XLeft);
	}
	friend bool			operator == (const ControlTagsAndXLeft& inA, 
									const ControlTagsAndXLeft& inB)
	{
		return (inA.m_Tag == inB.m_Tag);
	}
};
typedef ed::vector<ControlTagsAndXLeft>	TagsAndXLeftVector;
//------------------------------------------------------------------------

class Control;
class ComboBoxForDBGTaskDlg;
class TableLine : public Dialog
{
public:
	typedef Signal::Notifier<TableLine*> Event;
	enum TableLineTags
	{
		tagEditCount		= 1,
		tagComboboxType		= 2,
		tagComboboxBase		= 3,
		tagComboboxTask		= 4,
		tagComboboxTarget	= 5,
		tagComboboxSide		= 6,
		tagEditStart		= 7,
		tagButtonLeft		= 10,
		tagButtonRight		= 11
	};
private:
	DECLARE_MESSAGE_TABLE()

	ComboBoxForDBGTaskDlg*			m_ComboBoxKbrdFocus;
private:
	LineType						m_LineTypeID;

	TagsAndXLeftVector				m_SortedCtrlTags;
	TagsAndXLeftVector::iterator	m_SortedCtrlTagsIt;

	UI::Ptr<UI::Button>				m_LeftButton;
	UI::Ptr<UI::Button>				m_RightButton;

	int								m_NumberLineInPage;

	Event							m_sig_activated;
protected:
	bool		setParamToControl	(const ControlData& inParam, Control* ioControl);
	bool		getParamFromControl	(ControlData& outParam, Control* inControl);

	void		onClick				();
	void		onComboBoxKillFocus	(int inTag);
	void		onComboBoxSetFocus	(int inTag);
	void		onEditCountSetFocus	();
	void		onEditStartSetFocus	();
	void		onKeyDown			(int virtKey, int keyData);
public:
	TableLine(LineType inLineTypeID, int inNumberLineInPage);
	~TableLine();

	LineType		getType			() { return m_LineTypeID; }
	
	bool			setParameters	(const ControlDataVector& inParams);
	bool			getParameters	(ControlDataVector& outParams);
	
	int				getLineNumber	() { return m_NumberLineInPage; }
	void			setLineNumber	(int inNumberLine) { m_NumberLineInPage = inNumberLine; }

	void			Deactivate		();
	void			Activate		(int inActiveControlTag);
	Event&			activated		() { return m_sig_activated; }

	Control*		getActiveControl() { return getChild(m_SortedCtrlTagsIt->m_Tag); }

	virtual void	disable			(bool inStatus);
	void			show			(bool inStatus);

};
//------------------------------------------------------------------------
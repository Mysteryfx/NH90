#pragma once

#include "ui_Control.h"
#include "TableLine.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ComplexTableParameters : public ControlParameters
{
public:
	ComplexTableParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(ComplexTableParameters)
	virtual void serialize(Serializer &);

	typedef ControlParameters Base;

	void			read(const char* configText, int configSize);
};

class GroupDesc
{
	int			m_MinBottomValue;
	int			m_MaxBottomValue;
	int			m_Bottom;
	int			m_LineHeight;

	int			m_FirstLine;
	int			m_LastLine;
protected:
	int		CheckBottom(int inBottom)
	{
		if (inBottom >= m_MaxBottomValue - m_LineHeight) return m_MinBottomValue;
		else
		{
			if (inBottom < m_MinBottomValue) return m_MaxBottomValue - m_LineHeight;
			else
				return inBottom;
		}
	}
public:
	GroupDesc(int inMinBottomValue, int inMaxBottomValue, int inBottom, 
			int inLineHeight, int inFirst, int inLast = -1)
		: m_MinBottomValue(inMinBottomValue), m_MaxBottomValue(inMaxBottomValue), 
		  m_Bottom(inBottom), m_LineHeight(inLineHeight),
		  m_FirstLine(inFirst), m_LastLine(inLast)
	{
		m_Bottom = CheckBottom(m_Bottom);
		if (m_LastLine == -1)
			m_LastLine = m_FirstLine;
	}
	void		AddLine()
	{
		m_LastLine++;
		m_Bottom += m_LineHeight;
		m_Bottom = CheckBottom(m_Bottom);
	}
	bool		IsActive(int inNumberLine)
	{
		return (inNumberLine >= m_FirstLine && inNumberLine <= m_LastLine);
	}
	bool		IsEmpty()
	{
		return (m_LastLine == m_FirstLine);	// группа содержит один заголовок
	}
	void		Move(int inNumberLine, int inShift)
	{
		if (m_FirstLine >= inNumberLine)
			m_FirstLine += inShift;
		if (m_LastLine >= inNumberLine)
		{
			m_LastLine += inShift;
			m_Bottom += (m_LineHeight*inShift);
			m_Bottom = CheckBottom(m_Bottom);
		}
	}
	friend bool		operator == (const GroupDesc& inA, const GroupDesc& inB)
	{
		return (inA.m_FirstLine == inB.m_FirstLine && inA.m_LastLine == inB.m_LastLine);
	}
	int			GetBottom		() { return m_Bottom; }
	int			GetLineHeight	() { return m_LineHeight; }
	int			GetLastLine		() { return m_LastLine; }
	int			GetFirstLine	() { return m_FirstLine; }

	void		SetLineHeight	(int inLineHeight)
	{
		m_LineHeight = inLineHeight;
		m_Bottom = CheckBottom(m_Bottom);
	}
	void		SetBottom		(int inBottom) { m_Bottom = inBottom; }
};

typedef	ed::vector<TableLine*>		TableLineVector; 
typedef ed::vector<GroupDesc>		GroupDescVector;
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ComplexTable : public Control
{
public:
	typedef Control Base;
	typedef ComplexTableParameters Parameters;
private:
	DECLARE_MESSAGE_TABLE()
	
	int								m_StartLineY;
	TableLineVector					m_Body;
	TableLineVector::iterator		m_ActiveLineIt;
	Control*						m_ActiveControl;
	GroupDescVector					m_GroupsDesc;
	GroupDesc*						m_FillingGroup;

	TableLineVector					m_BackGround;

	int								m_FirstVisibleLine;
	int								m_PageSize;
	int								m_CurLineNumber;
	int								m_CurHeight;
protected:
	GroupDesc*	GetActiveGroup		(int inNumberLine = -1);
	bool		IsLineVisible		(int inLineNumber);	
	void		FillBackGround		();

	void		MoveLines			(TableLineVector::iterator inFirstLine, int inShift);
	int			DeleteLine			(TableLineVector::iterator aDeletedLineIt);
	void		SelectNext			();
	void		SelectPrev			();

	void		onPaint				();
	void		onKeyDown			(int virtKey, int keyData);
	void		onLineActivated		(TableLine* inActiveLine);
public:
	ComplexTable(const ControlParameters& inParams);
	~ComplexTable();

	int			AddLine				(LineType inType, const ed::vector<ControlData>& inParams, 
									 bool inInsertInActiveOn = false);
	int			AddGroup			(const ed::vector<ControlData>& inParams, 
									 const ed::vector<ControlData>* inLineParams = 0);

	int			Delete				(int inNumberLine = -1);

	bool		SetLineParameters	(const ed::vector<ControlData>& inParams, int inLineIndex);
	bool		GetLineParameters	(ed::vector<ControlData>& inParams, int inLineIndex);

	bool		onScrollPageDown	();
	bool		onScrollPageUp		();

	int			GetNumberPages		();
};

#pragma once

#include "ComboBox.h"


class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ComboBoxForDBGTaskDlg : public ComboBox
{
	DECLARE_MESSAGE_TABLE()

	bool				m_ShowListOn; 

protected:
	void				FormAndSendMessage(short wParlWord, short wParhWord, void* lPar);

	void				onKeyDown(int virtKey, int keyData);
	void				onKillFocus(Control* inReceivingFocus);
	void				onSetFocus(Control* inReceivingFocus);
	void				onEditClick();
	void				onEditSetFocus();
	void				onEditKillFocus();
	void				onPaint();
public:
	ComboBoxForDBGTaskDlg(const ComboBox::Parameters& params);
	~ComboBoxForDBGTaskDlg();

	virtual void		addString(const char* inCaption, int inIconNumber = -1);
};

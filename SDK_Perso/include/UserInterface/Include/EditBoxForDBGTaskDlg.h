#pragma once

#include "EditBox.h"

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR EditBoxForDBGTaskDlg : public EditBox
{
public:
	typedef EditBox Base;
private:
	DECLARE_MESSAGE_TABLE()

	int						m_NumberMouseClicks;
	bool					m_IfComboboxChild;
protected:
	//Обработка событий
	void					onLButtonDown(int keys, int x, int y);	
	void					onKeyDown(int virtKey, int keyData);
	void					onPaint();
	void					onSetFocus(Control* losingFocus);
	void					onKillFocus(Control* receivingFocus);
public:
	EditBoxForDBGTaskDlg(const EditBoxParameters& params, bool inDrawBorderStatus = false);
	virtual					~EditBoxForDBGTaskDlg();
};

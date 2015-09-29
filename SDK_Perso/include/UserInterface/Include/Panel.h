#ifndef __Panel__
#define __Panel__

#include "ui_Dialog.h"
#include "MessageTable.h"

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Panel :	public Dialog
{
	DECLARE_MESSAGE_TABLE()
public:
	Panel(const char * res);
	virtual ~Panel();
	virtual void init();
	
	virtual void show(bool modal = false);
	virtual void hide();
	virtual void close(int code);

	virtual void setPos(float x, float y);
	virtual void onMove(int dx, int dy);	
	
	virtual bool toggle();
	virtual bool toggle(bool on);
protected:
	virtual void onBtnCloseClk();
private:
	int					shiftDx, shiftDy;
};

class MultiColumnListBox;
void USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR addStringToListBox(const ed::string & str, int color, MultiColumnListBox* mlist, unsigned int maxRowCount);

#endif __Panel__
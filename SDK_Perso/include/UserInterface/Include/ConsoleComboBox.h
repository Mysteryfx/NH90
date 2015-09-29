#ifndef __USERINTERFACE_CONSOLE_COMBOBOX_H__
#define __USERINTERFACE_CONSOLE_COMBOBOX_H__

#include "ComboBox.h"

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ConsoleComboBox : public ComboBox
{
private:
	DECLARE_MESSAGE_TABLE();
public:
	typedef ed::list<ed::string> FullList;
protected:	
	static const unsigned int historyLengthMax;
	static const unsigned int listItemsMax;

	FullList				fullList;

	void onChar(int charCode, int keyData);
	void onKeyDown(int virtKey, int keyData);
	
	void buildList(); // Перезаполнение списка отобранными и сортированными элементами из fullList
	void chooseItem(int row); //Выбрать элемент ListBox'а и скопировать его строку в EditBox, но не закрывать ListBox
public:
	ConsoleComboBox(const Parameters & params);
	virtual	~ConsoleComboBox();
	virtual void addString(const char* inCaption, int inIconNumber = -1);
	virtual void delString(int index);
	void empty();
	inline const FullList & getFullList() const { return fullList; }
	void loadHistory();
	void saveHistory();
};

#endif __USERINTERFACE_CONSOLE_COMBOBOX_H__
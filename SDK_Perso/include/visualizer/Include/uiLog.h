#ifndef __uiLog__
#define __uiLog__

#include "Visualizer.h"

#include "Panel.h"
#include "MessageTable.h"
#include <regex>

class Static;
class MultiColumnListBox;
class ConsoleComboBox;

class VISUALIZER_API uiLog : public Panel
{
	DECLARE_MESSAGE_TABLE()
public:
	uiLog(const char * caption, const char * regex, const char * logFileName);
	~uiLog();
	void free();
	void clear();
	void onEDLogMessage(const char *module, int level, const char *msg);
	void in(const ed::string & text);	
protected:
	void onBtnCloseClk();
	void onEditEndSession();
private:
	void setRegexp_(const char * regexp);
	void freeRegexp_();
	bool				enabled;
	Static*				pCaption;
	Static*				pRegexpLabel;
	int					regexLabelFontColor;
	ConsoleComboBox*	pRegexComboBox;
	MultiColumnListBox*	pLog;
	std::regex*         preg;
	bool				pregIsCompiled;
	FILE*				file;
	static const int	logMaxSize;	
};

#endif
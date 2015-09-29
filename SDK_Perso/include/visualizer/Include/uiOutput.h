#ifndef __uiOutput__
#define __uiOutput__

#include "Visualizer.h"

#include "Panel.h"
#include "MessageTable.h"

class Static;
class MultiStatic;
namespace Common
{
	class Printer;
};

class VISUALIZER_API uiOutput : public Panel
{
	DECLARE_MESSAGE_TABLE()
public:
	uiOutput(Common::Printer * pPrinter_);
	~uiOutput();	
	void free();	
	void in(const char * text_);	
protected:
	virtual void onBtnCloseClk();
	void onBtnStopClk();
	void onPaint();
private:
	bool			stopped;
	Common::Printer*pPrinter;
	Static*			pCaption;
	ed::string		caption;
	MultiStatic*	pText;
};

#endif
#pragma once

#include "Visualizer.h"
#include <ed/string.h>

class ProgressBar;
class WaitCursor;
struct lua_State;
class VISUALIZER_API ProgressBarDialog
{
public:
	ProgressBarDialog();
	~ProgressBarDialog();

	void setValue(float value, bool update = true);
	void setText(const ed::string& text, bool update = true);
	void show();
	void updateWindowSize();

protected:
	void setBackground_();
	void kill_();

	ed::string bkgFilename_;
	lua_State* L_;
};

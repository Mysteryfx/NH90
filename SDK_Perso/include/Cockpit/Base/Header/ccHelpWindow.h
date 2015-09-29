#pragma once
#include "ui_Dialog.h"
#include "MultiColumnListBox.h"
#include "Actions/BanSwitcher.h"
#include "Common/WindowsManager.h"
#include "Common/UpdateTarget.h"
#include "ccClickable.h"
struct lua_State;

class CursorOwner;
namespace cockpit
{
class Clickable ;
class ccHelpWindow
{
public:
	ccHelpWindow(Clickable* parent);
	~ccHelpWindow();

	static ccHelpWindow * create(Clickable* parent);
	static void destroy(ccHelpWindow * win);

	void update(double time);
	void onMouseMove(int x,int y);
	void onMouseWheel(int x, int y, int clicks);
	void onLButtonDown(int x, int y);
	void onLButtonUp(int x, int y);
	void onRButtonDown(int x, int y);
	void onRButtonUp(int x, int y);
	void onLButtonDblclk(int x, int y);
	void onRButtonDblclk(int x, int y);
	void onMiddleButtonDblclk(int x, int y);

	void setVisible(bool visible);
	bool isVisible() const { return visible_; }
	void setCursorMode(CursorMode mode);
	void setCursorX(float x);
	void setCursorY(float y);
	void pingCursor();

private:
	void dispatchAnalog_(int command,double value);
	void dispatchDigital_(int command);
	void checkAutoHideCursor_();
	void mouseButtonCommand_(int command);
	void updateHint_();
	void updateCursor_();
	void showCursor_();
	void hideCursor_();
	void updateVisible_(bool visible);

	Clickable* cockpitInterface_;
	MouseBanSwitcher mouseBanSwitcher_;
	gui::WindowsManager::MouseListener* mouseListener_;
	gui::UpdateTarget* updateTarget_;
	bool visible_; // окно видимо
	bool cursorVisible_;  // курсор виден
	bool cursorAutoHided_; // курсор спрятам по таймауту
	bool cursorModeVisible_; // кокпит в режиме, когда курсор виден
	double lastCursorTime_;
	CursorOwner* cursorOwner_;
};
}

#ifndef __USERINTERFACE_CONTROLMANAGER_H__
#define __USERINTERFACE_CONTROLMANAGER_H__

#include "ui_Control.h"
#include "ExpImpSpec.h"

#include "UI/Manager.h"
#include "ui_Point.h"
#include "Utilities.h"
#include "Common.h"

#include <ed/map.h>
#include <assert.h>

class Rect;
class DescriptorTable;

class gShapeObject;

class cPosition;
class Dialog;
struct lua_State;
namespace UI { class Sound; }



struct customMainViewData
{
    bool   valid;
    float  UL_X;
    float  UL_Y;
    float  SZ_X;
    float  SZ_Y;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlManager : public Control, public UI::Manager
{
private:
	typedef ed::map<ed::string, Dialog*, StringWithoutCaseLess> PrecreatedDialogs;

protected:
	DescriptorTable*		needToUnregistrationControls;
	
	Control*				mousePointing;
	Control*				hasKbrdFocus;
	Control*				mouseDragging;
	Control*				modal;
	Control*				activeControl;
	Dialog*					ownerOfActive;

	bool					isStarted;
	bool					inputEnabled;
	bool					finished;
	
	bool					lButtonPressed;

    UI::Sound*              _sound; // NEVER NULL, uses stub instead

	void					onWindowPosChanged();
	bool					IsActivatingMessage(int inMessageID);
	void					finalRelease();
    customMainViewData      customMainViewData_;
public:
	ControlManager();
	virtual	~ControlManager();

	Control*				getFirstControl	();
	Control*				getNextControl	();

	void					enableInput(bool enable) {inputEnabled = enable;}
	bool					isInputEnabled(){return inputEnabled;}
	
	Control*				getMousePointing() const		{
		return mousePointing;
	}
	void					setMousePointing(Control* mp)	{
		mousePointing = mp;
	}

	Control*				getHasKbrdFocus() const			{return hasKbrdFocus;}
	void					setHasKbrdFocus(Control* kf)	{hasKbrdFocus = kf;}
//	устанавливает фокус ввода новому контолу, посылая при этом сообщения WM_KILLFOCUS и
//	WM_SETFOCUS
	void					ChangeHasKbrdFocus(Control* HasKbrdFocus);

	Control*				getMouseCapture() const			{return mouseDragging;}
	void					setMouseCapture(Control* md)	{mouseDragging = md;}

	Control*				getModal() const				{return modal;}
	void					setModal(Control* mod)			{modal = mod;}

	void					setActive(Control* newActive);
	Control*				getActive(){return activeControl;}
	Dialog*					getOwnerOfActive()				{return ownerOfActive;}

	// FIXME: временный костыль до удаления старого gui
	bool hasActiveDialogs();

	void					setInflateCoeffs(float kx, float ky);
	void					setResolution(float resX, float resY);

    void                    setMainViewPosition(float uppet_left_x,
                                                float uppet_left_y,
                                                float size_x,
                                                float size_y);

	void					sortIncomingMail();
	bool					interceptMessages(int messageID, int lParam, int wParam);
							
    void					start(HWND);
// для действий по очистке, выполнять которые на деструкторе слишком поздно
	virtual	void			finish(); 
	void					draw();


    const customMainViewData * getMainViewPositionData() const;

 	void					closeControl(Control* control);
	virtual int				registerControlAsChild(Control* control);
	virtual Control*		unregisterControlAsChild(Control* control, bool autodel=false);

	virtual Point			getScreenSize();
	bool					isFinished(){return finished;}

	bool					isLButtonPressed() const {return lButtonPressed;}

	virtual void			beginFrame(void);
	virtual void			endFrame  (void);

	virtual void			updatePaintCoordinates();

	/* UI::Manager interface */
	virtual UI::Element* create(const ed::string& name, const ed::string& param, UI::Group* grp, int id);
	virtual void destroy(UI::Element*);

    /* UI::Sound */
    void                    setSound(UI::Sound *sound); // NULL == no sound
    // NEVER NULL
    UI::Sound&              getSound() const { assert(_sound); return *_sound; }
};

extern USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ControlManager *globalControlManager;

#endif // __USERINTERFACE_CONTROLMANAGER_H__ 

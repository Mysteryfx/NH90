#ifndef __USERINTERFACE_MESSAGETABLE_H__
#define __USERINTERFACE_MESSAGETABLE_H__

class Control;
typedef void (Control::*PFNMSG)(void);

struct MSGTABLE_ENTRY
{
	int						message;
	int						notifyCode;
	int						controlID;
	int						lastControlID;
	int						functSignature;
	PFNMSG					pfn;
};

struct MSGTABLE
{
	const MSGTABLE*			baseTable;
	const MSGTABLE_ENTRY*	entries;
};

#define DECLARE_MESSAGE_TABLE() \
private: \
	static const MSGTABLE_ENTRY \
							messageEntries[]; \
protected: \
	static const MSGTABLE	messageTable; \
	virtual const MSGTABLE* getMessageTable() const; \
private: \

#define BEGIN_MESSAGE_TABLE(theClass, baseClass) \
const MSGTABLE* theClass::getMessageTable() const \
	{return &theClass::messageTable;} \
const MSGTABLE theClass::messageTable = \
	{&baseClass::messageTable, &theClass::messageEntries[0]}; \
const MSGTABLE_ENTRY theClass::messageEntries[] = \
	{ \
 
#define END_MESSAGE_TABLE() \
	{0, 0, 0, 0, sig_end, 0} \
	}; \

// first - parts of wParam (from low word), next - parts of lParam (from low word)
union MessageTableFunctions
{
	PFNMSG					pfn;		// v_v	
	void					(Control::*pfn_v_wi_lii)(int, int, int);
	void					(Control::*pfn_v_wss_lii)(short, short, int, int);
	void					(Control::*pfn_v_wi_li)(int, int);
	void					(Control::*pfn_v_lii)(int, int);
	void					(Control::*pfn_v_wii_lpCntrl)(int, int, Control*);
	void					(Control::*pfn_v_wpCntrl)(Control*);
	void					(Control::*pfn_v_wi_lpCntrl)(int, Control*);
	void					(Control::*pfn_v_wi)(int);
	void					(Control::*pfn_v_wb_li)(bool, int);
	void					(Control::*pfn_v_wid)(int);
	void					(Control::*pfn_v_li)(int);
};

enum Sig
{
	sig_end = 0,			// the last record
	sig_v_wi_lii,
	sig_v_wss_lii,
	sig_v_lii,
	sig_v_wi_li,
	sig_v_wii_lpCntrl,
	sig_v_wpCntrl,
	sig_v_wi_lpCntrl,
	sig_v_wi,
	sig_v_wb_li,
	sig_v_wid,
	sig_v_v,
	sig_v_li
};

#define ONM_CONTROL(notifyCode, id, memberFxn) \
	{WM_COMMAND, (int)(notifyCode), (int)(id), (int)(id), sig_v_v, \
		(PFNMSG)&memberFxn},
#define ONM_CONTROL_MOUSEMOVE(id) \
	{WM_MOUSEMOVE, 0, (int)id, (int)id, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onMouseMove},
#define ONM_CONTROL_RANGE(notifyCode, id, idLast, memberFxn) \
	{WM_COMMAND, (int)notifyCode, (int)id, (int)idLast, sig_v_wid, \
		(PFNMSG)(void (Control::*)(int))&memberFxn },

#define ONM_BNCLICK(id, memberFxn) ONM_CONTROL(BN_CLICKED, id, memberFxn)

#define ONM_COMMAND(id, memberFxn) \
	{WM_COMMAND, 0, (int)id, (int)id, sig_v_v, (PFNMSG)&memberFxn},
#define ONM_COMMAND_RANGE(id, idLast, memberFxn) \
	{WM_COMMAND, 0, (int)id, (int)idLast, sig_v_wid, \
		(PFNMSG)(void (Control::*)(int))&memberFxn},

#define ONM_WM_MOUSEMOVE() \
	{WM_MOUSEMOVE, 0, 0, 0, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onMouseMove},
#define ONM_WM_LBUTTONDOWN() \
	{WM_LBUTTONDOWN, 0, 0, 0, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onLButtonDown},
#define ONM_WM_LBUTTONDBLCLK() \
	{WM_LBUTTONDBLCLK, 0, 0, 0, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onLButtonDblclk},
#define ONM_WM_LBUTTONUP() \
	{WM_LBUTTONUP, 0, 0, 0, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onLButtonUp},
#define ONM_WM_RBUTTONDOWN() \
	{WM_RBUTTONDOWN, 0, 0, 0, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onRButtonDown},
#define ONM_WM_RBUTTONUP() \
	{WM_RBUTTONUP, 0, 0, 0, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onRButtonUp},
#define ONM_WM_RBUTTONDBLCLK() \
	{WM_RBUTTONDBLCLK, 0, 0, 0, sig_v_wi_lii, \
	(PFNMSG)(void (Control::*)(int, int, int))&onRButtonDblclk},

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A  // message that will be supported
                                        // by the OS 
#endif

#define ONM_WM_MOUSEWHEEL() \
	{WM_MOUSEWHEEL, 0, 0, 0, sig_v_wss_lii, \
		(PFNMSG)(void (Control::*)(short, short, int, int))&onMouseWheel},

#define ONM_WM_CHAR() \
	{WM_CHAR, 0, 0, 0, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&onChar},
#define ONM_WM_KEYDOWN() \
	{WM_KEYDOWN, 0, 0, 0, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&onKeyDown},
#define ONM_WM_KEYUP() \
	{WM_KEYUP, 0, 0, 0, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&onKeyUp},
#define ONM_WM_SYSKEYDOWN() \
	{WM_SYSKEYDOWN, 0, 0, 0, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&onSysKeyDown},
#define ONM_WM_SYSKEYUP() \
	{WM_SYSKEYUP, 0, 0, 0, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&onSysKeyUp},

#define ONM_WM_SETFOCUS() \
	{WM_SETFOCUS, 0, 0, 0, sig_v_wpCntrl, \
		(PFNMSG)(void (Control::*)(Control*))&onSetFocus},
#define ONM_WM_KILLFOCUS() \
	{WM_KILLFOCUS, 0, 0, 0, sig_v_wpCntrl, \
		(PFNMSG)(void (Control::*)(Control*))&onKillFocus},

#define ONM_WM_PAINT()
/*	{WM_PAINT, 0, 0, 0, sig_v_v, \
		(PFNMSG)(void (Control::*)())&DPaint},
*/
#define ONM_WM_MOVE() \
	{WM_MOVE, 0, 0, 0, sig_v_lii, \
		(PFNMSG)(void (Control::*)(int, int))&onMove},

#define ONM_WM_ACTIVATE() \
	{WM_ACTIVATE, 0, 0, 0, sig_v_wi_lpCntrl, \
		(PFNMSG)(void (Control::*)(int, Control*))&onActivate},

#define ONM_WM_VSCROLL() \
	{WM_VSCROLL, 0, 0, 0, sig_v_wii_lpCntrl, \
		(PFNMSG)(void (Control::*)(int, int, Control*))&onVScroll},
#define ONM_WM_HSCROLL() \
	{WM_HSCROLL, 0, 0, 0, sig_v_wii_lpCntrl, \
		(PFNMSG)(void (Control::*)(int, int, Control*))&onHScroll},

#define ONM_WM_CLOSE() \
	{WM_CLOSE, 0, 0, 0, sig_v_v, \
		(PFNMSG)(void (Control::*)())&onClose},
#define ONM_WM_CREATE() \
	{WM_CREATE, 0, 0, 0, sig_v_v, \
		(PFNMSG)(void (Control::*)())&onCreate},

#define ONM_WM_WINDOWPOSCHANGING() \
	{WM_WINDOWPOSCHANGING, 0, 0, 0, sig_v_lii, \
		(PFNMSG)(void (Control::*)(int, int))&onWindowPosChanging},

#define ONM_WM_STYLECHANGING() \
	{WM_STYLECHANGING, 0, 0, 0, sig_v_wb_li, \
		(PFNMSG)(void (Control::*)(bool, int))&onStyleChanging},

#define ONM_WM_SIZE() \
	{WM_SIZE, 0, 0, 0, sig_v_v, \
		(PFNMSG)(void (Control::*)())&onSize},

#define ONM_WM_MENUITEMSELECT() \
	{WM_MENUSELECT, 0, 0, 0, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&onMenuItemSelect},

#define ONM_WM_MENUITEMSELECT_FN(id, idLast, memberFxn) \
	{WM_MENUSELECT, 0, (int)id, (int)idLast, sig_v_wi_li, \
		(PFNMSG)(void (Control::*)(int, int))&memberFxn},

#define ONM_WM_KEYDOWNCHILD(id, idLast) \
	{WM_IME_KEYDOWN, 0, (int)id, (int)idLast, sig_v_wi_lii, \
		(PFNMSG)(void (Control::*)(int, int, int))&onKeyDownChild},

#endif // __USERINTERFACE_MESSAGETABLE_H__
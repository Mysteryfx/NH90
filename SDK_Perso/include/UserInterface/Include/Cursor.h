#ifndef __USERINTERFACE_CURSOR_H__
#define __USERINTERFACE_CURSOR_H__

#include "ExpImpSpec.h"
#include "ui_point.h"
#include "DrawPrimitive.h"

//--------------------------------------------------------
//	shapes of cursor
//-------------------------
enum
{
	cursor_normal = 0,		
	cursor_wait,			
	cursor_edit,			
	cursor_hand_push,	
	cursor_hand_move,	
	cursor_arrow_rotate,	
	cursor_arrow_zoom,	
	cursor_arrow_meter,	
	cursor_arrow_inactive,
	cursor_arrow_weapon,	
	cursor_empty_group,	
	cursor_edit_group,	
	cursor_point,
	cursor_airplane,		
	cursor_helicopter,	
	cursor_ship,			
	cursor_sam,			
	cursor_tank,			
	cursor_radar,		
	cursor_static,		
	cursor_cancel1,		
	cursor_cancel2,		
	cursor_cancel3,		
	cursor_goal,			
	cursor_zoom_in,
	cursor_zoom_out,
	cursor_goal_inactive,
	cursor_target_enemy,	
	cursor_target_allies,
	cursor_cross,
	cursor_cockpit_default,
	cursor_cockpit_button,
	cursor_cockpit_switch,
	cursor_cockpit_rotary
};
//--------------------------------------------------------
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Cursor
{
protected:
	static HMODULE			m_DllHandle;
	static HWND				m_MainWnd;

	static HCURSOR			m_Cursor;
	static int				m_StoredShape;
	static int				m_Shape;
    static bool             m_IsAutonomous;
    static Point            m_Pos;

    static DrawPrimitive*   m_SoftCursor;
    static void             setSoftCursor(int inShape);

public:

    // cursor will disregard user input, for AVI rendering
    static void             setAutonomousMode(bool on);

    // cursor will be rendered through DirectX, instead of Windows API
    static void             setSoftwareMode (bool on);
	static bool				getSoftwareMode () {
		return bool(m_SoftCursor);
	};

    static void				initCursor		(HWND inMainWnd, int inShape = cursor_normal);

	static int				getCursor		() 	{ return m_Shape; }
	static int				setCursor		(int inShape);

	static int				setAndStoreCursor(int storedShape);
	static int				storeCursor(int storedShape = -1);
	static int				restoreCursor();
	static int				getStoredCursor(){return m_StoredShape;}

	static Point			getPosition		();
	static osg::Vec2i		getPosition		(bool) { return osg::Vec2i((int)m_Pos.x, (int)m_Pos.y); }
	static void				setPosition		(const Point& pos);
	static void				setPosition		(const osg::Vec2i& pos) { setPosition(Point(pos.x(), pos.y())); }

    static void             drawCursor      (); // in software mode
};
//--------------------------------------------------------
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR WaitCursor
{
	int			m_PrevCursor;
public:
	WaitCursor(int inRestoredCursorShape = -1);	// id курсора, который будет установлен после 
												// WaitCursor'а. При -1 будет востановлен предыдущий
												// курсор
	~WaitCursor();

	void		End();
};
//--------------------------------------------------------
#endif // __USERINTERFACE_CURSOR_H__
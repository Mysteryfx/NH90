#ifndef	__PAINTLIST_H_
#define	__PAINTLIST_H_

#include <ed/list.h>

class Control;

typedef ed::list<Control*>::iterator					PaintListIterator;
typedef ed::list<Control*>::reverse_iterator			PaintListRIterator;

typedef ed::list<Control*>::const_iterator				PaintListCIterator;
typedef ed::list<Control*>::const_reverse_iterator		PaintListCRIterator;


class PaintList : public ed::list<Control*>
{
public:
	PaintListIterator		findInPaintList		(Control* control, bool optimize = true);
	PaintListIterator		addToPaintList		(Control* control);
	PaintListIterator		deleteFromPaintList (Control* control);
	
	PaintListIterator		makeChildTopMost    (PaintListIterator control);
	PaintListIterator		makeChildBottomMost (PaintListIterator control);

	PaintListIterator		makeChildTopMost    (Control* control);
	PaintListIterator		makeChildBottomMost (Control* control);
};

#endif	//	__PAINTLIST_H__
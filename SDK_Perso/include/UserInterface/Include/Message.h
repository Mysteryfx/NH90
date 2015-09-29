#ifndef __USERINTERFACE_MESSAGE_H__
#define __USERINTERFACE_MESSAGE_H__

#include <stdint.h>

union DWord
{
	struct
	{
		short lWord;
        short hWord;
	};

    intptr_t dblWord;

    DWord() {}
    DWord(int v) { dblWord = v; }
    DWord(void* p) { dblWord = reinterpret_cast<intptr_t>(p); }
};

struct Message
{
	int						message;

	DWord					wParam;
	DWord					lParam;

							Message(int message, 
									DWord wParam, 
									DWord lParam)			{this->message = message, 
															 this->wParam = wParam, 
															 this->lParam = lParam;}
							Message(int message, 
									intptr_t wParam, 
									intptr_t lParam)				{this->message = message, 
															 this->wParam.dblWord = wParam, 
															 this->lParam.dblWord = lParam;}
							Message(int message)			{this->message = message,
							                                 wParam.dblWord = 0,
															 lParam.dblWord = 0;}
};

#endif // __USERINTERFACE_MESSAGE_H__

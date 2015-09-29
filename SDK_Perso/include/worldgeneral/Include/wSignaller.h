//                    file wSignaller.h
#ifndef __wSignaller_h__
#define __wSignaller_h__

#include "WorldGeneral.h"
#include "Serializer\Serializer.h"
#include "mail\stream.h"

class WORLDGENERAL_API wSignaller
{
public:

	wSignaller();
	~wSignaller();

	void	updateState();
	void	setState(bool currentState_) {currentState = currentState_;}
    bool    getState()const{return currentState;}//by S.W.Chistoff
	bool	processBegin() { return beginProcess;}
	bool	processEnd()   { return endProcess;}

	WORLDGENERAL_API friend Serializer& operator << (Serializer& serializer, wSignaller& data);
    WORLDGENERAL_API friend Mail::Stream& operator >> (Mail::Stream& stream, wSignaller& data);

protected:

	bool	currentState;
	bool	lastState;
	bool	beginProcess;
	bool	endProcess;
};

#endif

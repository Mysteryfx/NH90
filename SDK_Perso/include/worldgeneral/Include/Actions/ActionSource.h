#ifndef __ACTION_SOURCE_H__
#define __ACTION_SOURCE_H__

#include <ed/vector.h>

#include "InputAction.h"

#include "WorldGeneral.h"

class TimedActionBuffer;
class TrackController;
class ActionDispatcher;
struct ActionEntry;

// Забирает сообщения из инпута и пихает их в timedActionBuffer
class WORLDGENERAL_API  ActionSource
{
public:
	ActionSource(TimedActionBuffer* actionBuffer, ActionDispatcher* actionDispatcher);

	// Обновиться (вызывается каждый фрейм)
	void update();

	// Synchronize
	void synchronizeControls()                 { controls_need_to_be_synchronized_ = true;};
	void preventControlsSynchronization(bool value) {prevent_controls_synchronization_ = value; }

	// отключить клавиатуру/мышь
	void banKeyboard(bool ban);
	bool isKeyboardBanned() const { return keyboardBanned_; }

	void banMouse(bool ban);
	bool isMouseBanned() const { return mouseBanned_; }

	void banInput(bool ban);
	bool isInputBanned() const { return mouseBanned_; }

private:
	void processInput_();
	void fillAction_(const InputAction& inputAction, ActionEntry& actionEntry);

	void feedToBuffer_(const ed::vector<InputAction>& actions);
	void feedToDispatcher_(const ed::vector<InputAction>& actions);

private:
	TimedActionBuffer* actionBuffer_;
	ActionDispatcher* actionDispatcher_;

	bool keyboardBanned_;
	bool mouseBanned_;
	bool inputBanned_;

	bool controls_need_to_be_synchronized_;
	bool prevent_controls_synchronization_;//useful for training mission to get trigger conditions same for all
};

#endif
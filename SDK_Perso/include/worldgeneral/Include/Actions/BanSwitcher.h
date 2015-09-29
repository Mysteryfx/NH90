#ifndef _BAN_SWITCHER_H
#define _BAN_SWITCHER_H

#include "WorldGeneral.h"

#include "Utilities/Locker.h"

// Keyboard

class WORLDGENERAL_API KeyboardLocker
{
public:
	void lock();
	void unlock();
};

typedef CountLocker<KeyboardLocker> KeyboardBanner;

extern WORLDGENERAL_API KeyboardBanner keyboardBanner;

class KeyboardBanSwitcher : public LockSwitcher<KeyboardBanner>
{
public:
	KeyboardBanSwitcher() : LockSwitcher<KeyboardBanner>(keyboardBanner) {}
};

//Mouse

class WORLDGENERAL_API MouseLocker
{
public:
	void lock();
	void unlock();
};

typedef CountLocker<MouseLocker> MouseBanner;

extern WORLDGENERAL_API MouseBanner mouseBanner;

class MouseBanSwitcher : public LockSwitcher<MouseBanner>
{
public:
	MouseBanSwitcher() : LockSwitcher<MouseBanner>(mouseBanner) {}
};

// Input
class WORLDGENERAL_API InputLocker
{
public:
	void lock();
	void unlock();
};

typedef CountLocker<InputLocker> InputBanner;

extern WORLDGENERAL_API InputBanner inputBanner;

class InputBanSwitcher : public LockSwitcher<InputBanner>
{
public:
	InputBanSwitcher() : LockSwitcher<InputBanner>(inputBanner) {}
};

#endif _BAN_SWITCHER_H
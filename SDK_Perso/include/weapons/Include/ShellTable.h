#pragma once

#include "Weapons.h"

class wShellDescriptor;

// Это должно умереть
class WEAPONS_API ShellTable
{
private:
	ShellTable();
	~ShellTable();

public:
	static void init();

    //static wShellDescriptor* GetGunShell(unsigned char A_Name, bool armour_piercer = false);
	static wShellDescriptor* GetGunPodShell(unsigned char A_Name);
};
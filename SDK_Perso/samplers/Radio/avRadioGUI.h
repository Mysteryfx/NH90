#ifndef _avRadioGUI_h
#define _avRadioGUI_h

#include "CockpitBase.h"

#include "ICommandDialogsPanel.h"

namespace Lua
{
class Config;
}

namespace cockpit
{

void COCKPITBASE_API loadInterphone(Lua::Config & config, ICommandDialogsPanel::Interphone & interphone);

void COCKPITBASE_API loadRadio(Lua::Config & config, ICommandDialogsPanel::Radio & radio);

}

#endif _avRadioGUI_h
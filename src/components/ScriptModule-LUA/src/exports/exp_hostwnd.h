#pragma once

#ifndef _EXP_HOSTWND_H
#define _EXP_HOSTWND_H
#include <windows.h>

extern "C"
{
#include "lua.h"
#include "lstate.h"
}

BOOL ExpLua_HostWnd(lua_State *L);

#endif
#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

typedef void (*registerLuaTable)(lua_State*);
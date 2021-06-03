#pragma once

#include "core/Service.hpp"
#include "lua.hpp"

class ScriptService : public Service {
private:
    lua_State *globalState;

public:
    ScriptService(ObjectPtr r);

    lua_State* newThread();
};
#pragma once

#include "core/Object.hpp"
#include "services/ScriptService.hpp"

class Script : public Object {
private:
    lua_State *state = nullptr;
    ScriptService *scrSrvc = nullptr;

    // custom events
    void onParentRemove();
    void onParentAdd();

public:
    Script();

    void run(std::string script);
};
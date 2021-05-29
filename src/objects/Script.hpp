#pragma once

#include "core/Object.hpp"

class Script : public Object {
private:
    lua_State *state;

public:
    Script();

    void run(std::string script);
};
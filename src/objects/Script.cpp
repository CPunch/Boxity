#include "core/Object.hpp"
#include "objects/Script.hpp"

Script::Script() {
    name = "Script";

    state = luaL_newstate();
    luaopen_base(state);
    Object::addBindings(state);
}

// temp
void Script::run(std::string script) {
    // push root and set the global
    root->pushLua(state);
    lua_setglobal(state, "root");

    luaL_loadstring(state, script.c_str());
    lua_call(state, 0, 0);
}
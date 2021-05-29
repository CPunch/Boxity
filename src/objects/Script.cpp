#include "core/Object.hpp"
#include "objects/Script.hpp"

Script::Script() {
    state = luaL_newstate();
    Object::addBindings(state);
}

// temp
void Script::run(std::string script) {
    // push root and set the global
    Object::pushObj(state, root);
    lua_setglobal(state, "root");

    luaL_loadstring(state, script.c_str());
}
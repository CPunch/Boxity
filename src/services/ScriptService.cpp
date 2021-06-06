#include "services/ScriptService.hpp"

#include "core/Object.hpp"
#include "core/Entity.hpp"
#include "core/ObjectFactory.hpp"
#include "types/Type.hpp"
#include "types/Vec2.hpp"

ScriptService::ScriptService(ObjectPtr r): Service(r) {
    globalState = luaL_newstate();

    luaopen_base(globalState);
    ObjectFactory::addBindings(globalState);
    Object::addBindings(globalState);
    Entity::addBindings(globalState);
    Type::addBindings(globalState);
    Vec2::addBindings(globalState);

    // push root and set the global
    root->pushLua(globalState);
    lua_setglobal(globalState, "root");
}

lua_State* ScriptService::newThread() {
    return lua_newthread(globalState);
}
#include "services/ScriptService.hpp"
#include "services/TaskService.hpp"

#include "core/Object.hpp"
#include "core/Entity.hpp"
#include "core/Root.hpp"
#include "core/ObjectFactory.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"
#include "objects/Script.hpp"
#include "types/Type.hpp"
#include "types/Vec2.hpp"

template <> ScriptService *Singleton<ScriptService>::singleton = nullptr;

void ScriptService::wait_task(void* ud, uint64_t currTime, uint64_t realDelta) {
    lua_State *L = (lua_State*)ud;

    lua_pushnumber(L, (float)realDelta/1000);
    ScriptService::getSingleton().yieldCall(L, 1);
}

int ScriptService::luaWait(lua_State *L) {
    lua_Number seconds = luaL_optnumber(L, 1, 0);
    TaskService::getSingleton().pushTask(ScriptService::wait_task, (void*)L, 1000*seconds, false);

    return lua_yield(L, 0);
}

ScriptService::ScriptService() {
    gL = luaL_newstate();

    luaopen_base(gL);
    ObjectFactory::addBindings(gL);
    Object::addBindings(gL);
    Entity::addBindings(gL);
    Ball::addBindings(gL);
    Box::addBindings(gL);
    Script::addBindings(gL);
    Type::addBindings(gL);
    Vec2::addBindings(gL);

    // push root and set the global
    Root::getSingleton().pushLua(gL);
    lua_setglobal(gL, "root");

    lua_pushcfunction(gL, luaWait);
    lua_setglobal(gL, "wait");
}

ScriptService::~ScriptService() {
    ScriptService::close(gL);
}

void ScriptService::close(lua_State *L) {
    lua_close(L);
}

lua_State* ScriptService::newThread() {
    return lua_newthread(gL);
}

lua_State* ScriptService::newCoroutine(lua_State *pL) {
    return lua_newthread(pL);
}

int ScriptService::yieldCall(lua_State *L, int args) {
    int results;
    int ret = lua_resume(L, NULL, args, &results);

    if (ret == LUA_OK) {
        // thread didn't call wait() or anything, close the state!
        close(L);
    } else if (ret != LUA_YIELD) { // its not LUA_YIELD or LUA_OK
        // for now, just print the error to std::cerr
        std::cerr << lua_tostring(L, -1) << std::endl;

        // then close the dead state
        close(L);
    }

    return results;
}
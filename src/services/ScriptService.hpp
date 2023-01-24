#pragma once

#include "core/Service.hpp"
#include "core/Singleton.hpp"
#include "lua.hpp"

#include <map>

class ScriptService : public Service, public Singleton<ScriptService> {
private:
    lua_State *gL; // global state

    static void wait_task(void* ud, uint64_t currTime, uint64_t realDelta);
    static int luaWait(lua_State *L);

public:
    ScriptService();
    ~ScriptService();

    void close(lua_State*);

    lua_State* newThread();
    lua_State* newCoroutine(lua_State*);

    int yieldCall(lua_State *L, int args);
};
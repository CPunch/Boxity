#pragma once

#include "core/Service.hpp"
#include "lua.hpp"

#include <map>

class ScriptService : public Service {
private:
    struct stateInfo {
        lua_State *L;
        stateInfo *parent;
        ScriptService *scrpSrvc;
        int ref;
        int children = 0;
        int events = 0;
        bool alive = true;
    };

    static std::map<lua_State*, stateInfo*> LStates;
    stateInfo *globalState;

    static void wait_task(void* ud, uint64_t currTime, uint64_t realDelta);
    static int luaWait(lua_State *L);

    stateInfo* addState(lua_State *L, stateInfo *parent, int ref);

public:
    ScriptService(ObjectPtr r);
    ~ScriptService();

    static void close(lua_State*);

    lua_State* newThread();
    lua_State* newCoroutine(lua_State*);

    static int yieldCall(lua_State *L, int args);
};
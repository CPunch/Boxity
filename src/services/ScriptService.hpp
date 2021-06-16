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
        int children;
        bool alive = true;
    };

    static std::map<lua_State*, stateInfo*> LStates;
    stateInfo *globalState;

    stateInfo* addState(lua_State *L, stateInfo *parent, int ref);

public:
    ScriptService(ObjectPtr r);
    ~ScriptService();

    void close(lua_State*);

    lua_State* newThread();
    lua_State* newCoroutine(lua_State*);
};
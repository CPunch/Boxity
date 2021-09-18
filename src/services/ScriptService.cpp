#include "services/ScriptService.hpp"
#include "services/TaskService.hpp"

#include "core/Object.hpp"
#include "core/Entity.hpp"
#include "core/ObjectFactory.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"
#include "objects/Script.hpp"
#include "types/Type.hpp"
#include "types/Vec2.hpp"

std::map<lua_State*, ScriptService::stateInfo*> ScriptService::LStates;

void ScriptService::wait_task(void* ud, uint64_t currTime, uint64_t realDelta) {
    lua_State *L = (lua_State*)ud;

    lua_pushnumber(L, (float)realDelta/1000);
    yieldCall(L, 1);
}

int ScriptService::luaWait(lua_State *L) {
    lua_Number secs = luaL_optnumber(L, 1, 0);

    auto iter = LStates.find(L);
    stateInfo *sI;

    // sanity check
    if (iter == LStates.end())
        return 0; // this state doesn't even exist?

    sI = (*iter).second;

    // grab the root from the state's script service, and from that grab the task service. then schedule a task to be called
    ((TaskService*)castObjPtr(sI->scrpSrvc->root, Root)->getService(TASKSRV))->pushTask(ScriptService::wait_task, (void*)L, 1000*secs, false);
    return lua_yield(L, 0);
}

ScriptService::stateInfo* ScriptService::addState(lua_State *L, stateInfo *parent, int ref) {
    stateInfo *newState = new stateInfo();
    newState->L = L;
    newState->parent = parent;
    newState->scrpSrvc = this;
    newState->ref = ref;
    newState->children = 0;
    newState->events = 0;
    newState->alive = true;

    LStates[L] = newState;

    if (parent != nullptr)
        parent->children++;

    return newState;
}

ScriptService::ScriptService(ObjectPtr r): Service(r) {
    lua_State *gL = luaL_newstate();
    globalState = addState(gL, nullptr, -1);

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
    root->pushLua(gL);
    lua_setglobal(gL, "root");

    lua_pushcfunction(gL, luaWait);
    lua_setglobal(gL, "wait");
}

ScriptService::~ScriptService() {
    close(globalState->L);
}

void ScriptService::close(lua_State *L) {
    auto iter = LStates.find(L);
    stateInfo *sI;

    // sanity check
    if (iter == LStates.end())
        return;

    sI = (*iter).second;
    sI->alive = false;

    // keep it alive until the children & events are dead!
    if (sI->children > 0 || sI->events > 0)
        return;

    // take care of the parent state
    if (sI->parent != nullptr) {
        sI->parent->children--;

        // remove it from the references
        if (sI->ref != -1) {
            luaL_unref(sI->parent->L, LUA_REGISTRYINDEX, sI->ref);
        }

        // try to close the parent
        if (sI->parent->children <= 0 || !sI->parent->alive) {
            close(sI->parent->L);
        }
    }

    // at this point, the state should have no children, no events, and the parent could already be free'd or non-existant (nullptr)
    // finally free it and remove it from the lookup table!
    lua_close(L);
    delete sI;
    LStates.erase(L);
}

lua_State* ScriptService::newThread() {
    // create the thread from the globalState and add it to the global registryglobalState
    lua_State *L = lua_newthread(globalState->L);
    addState(L, globalState, luaL_ref(globalState->L, LUA_REGISTRYINDEX));
    globalState->children++;

    return L;
}

lua_State* ScriptService::newCoroutine(lua_State *pL) {
    lua_State *L;
    stateInfo *parentSI;
    auto iter = LStates.find(pL);

    // sanity check
    if (iter == LStates.end())
        return nullptr;

    parentSI = (*iter).second;

    // create a new coroutine under this state
    L = lua_newthread(pL);
    addState(L, parentSI, luaL_ref(pL, LUA_REGISTRYINDEX));
    parentSI->children++;

    return L;
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
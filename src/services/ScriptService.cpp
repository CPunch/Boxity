#include "services/ScriptService.hpp"
#include "services/TaskService.hpp"

#include "core/Object.hpp"
#include "core/Entity.hpp"
#include "core/ObjectFactory.hpp"
#include "objects/Ball.hpp"
#include "types/Type.hpp"
#include "types/Vec2.hpp"

std::map<lua_State*, ScriptService::stateInfo*> ScriptService::LStates;

ScriptService::stateInfo* ScriptService::addState(lua_State *L, stateInfo *parent, int ref) {
    stateInfo *newState = new stateInfo();
    newState->L = L;
    newState->parent = parent;
    newState->scrpSrvc = this;
    newState->children = 0;
    newState->ref = ref;

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
    Type::addBindings(gL);
    Vec2::addBindings(gL);

    // push root and set the global
    root->pushLua(gL);
    lua_setglobal(gL, "root");
}

ScriptService::~ScriptService() {
    delete globalState;
}

void ScriptService::close(lua_State *L) {
    auto iter = LStates.find(L);
    stateInfo *sI;

    // sanity check
    if (iter == LStates.end())
        return;

    sI = (*iter).second;
    sI->alive = false;

    // keep it alive until the children are dead!
    if (sI->children > 0)
        return;

    // take care of the parent state
    if (sI->parent != nullptr) {
        sI->parent->children--;

        // we need to close the parent
        if (sI->parent->children <= 0 && !sI->parent->alive) {
            close(sI->parent->L);
        }
    }

    // at this point, the state should have no children, and the parent could already be free'd or non-existant (nullptr)
    // finally free it and remove it from the lookup table!
    delete sI;
    LStates.erase(L);
}

lua_State* ScriptService::newThread() {
    // create the thread from the globalState and add it to the global registryglobalState
    lua_State *L = lua_newthread(globalState->L);
    addState(L, globalState, luaL_ref(globalState->L, LUA_REGISTRYINDEX));

    luaopen_base(L);
    Object::addBindings(L);
    Entity::addBindings(L);
    Ball::addBindings(L);
    Type::addBindings(L);
    Vec2::addBindings(L);

    // push root and set the global
    root->pushLua(L);
    lua_setglobal(L, "root");

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

    return L;
}
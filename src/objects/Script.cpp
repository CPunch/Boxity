#include "core/Root.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"
#include "types/Type.hpp"
#include "types/Vec2.hpp"
#include "objects/Script.hpp"

Script::Script() {
    name = "Script";
}

void Script::onParentAdd() {
    if (root == nullptr) 
        goto _passSOPAEvnt;

    scrSrvc = (ScriptService*)castObjPtr(root, Root)->getService(SCRIPTSRV);
    state = scrSrvc->newThread();

_passSOPAEvnt:
    Object::onParentAdd();
}

void Script::onParentRemove() {
    // stubbed for now
    Object::onParentRemove();
}

// temp
void Script::run(std::string script) {
    if (state == nullptr)
        return;

    luaL_loadstring(state, script.c_str());
    lua_call(state, 0, 0);
}